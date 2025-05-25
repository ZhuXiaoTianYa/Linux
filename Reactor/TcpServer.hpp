#pragma once

#include <iostream>
#include <cassert>
#include <functional>
#include <unordered_map>
#include "Epoller.hpp"
#include "Util.hpp"
#include "Log.hpp"
#include "Sock.hpp"
#include "Err.hpp"

namespace tcpserver
{
    class Connection;
    class TcpServer;
    static const int defaultport = 8086;
    static const int num = 64;
    static const int bufsize = 1024;

    using func_t = std::function<void(Connection *)>;

    class Connection
    {
    public:
        Connection(int sock, TcpServer *tsp)
            : _sock(sock), _tsp(tsp)
        {
        }

        void Register(func_t recver, func_t sender, func_t excepter)
        {
            _recver = recver;
            _sender = sender;
            _execpter = excepter;
        }

        ~Connection()
        {
        }

        void Close()
        {
            close(_sock);
        }

    public:
        int _sock;
        std::string _inbuffer;
        std::string _outbuffer;
        func_t _recver;
        func_t _sender;
        func_t _execpter;
        uint64_t _lasttime;
        TcpServer *_tsp;
    };

    class TcpServer
    {
    public:
        void AddConnection(int sock, uint32_t events, func_t recver, func_t sender, func_t excepter)
        {
            if (events & EPOLLET)
            {
                bool res = Util::SetNonBlock(sock);
                if (res == true)
                    logMessage(NORMAL, "set socket nonblock success");
                else
                    logMessage(ERROR, "set socket nonblock error");
            }
            Connection *conn = new Connection(sock, this);
            conn->Register(recver, sender, excepter);
            bool ret = _epoller.AddEvent(sock, events);
            assert(ret);
            _connections.insert(std::pair<int, Connection *>(sock, conn));
            logMessage(NORMAL, "add new sock : %d in epoll and unordered_map", sock);
        }

        void Recver(Connection *conn)
        {
            conn->_lasttime = time(nullptr);
            char buffer[bufsize];
            while (true)
            {
                ssize_t s = recv(conn->_sock, buffer, sizeof(buffer) - 1, 0);
                if (s > 0)
                {
                    buffer[s] = 0;
                    conn->_inbuffer += buffer;
                    logMessage(NORMAL, "\n%s", conn->_inbuffer.c_str());
                    _service(conn);
                }
                else if (s == 0)
                {
                    if (conn->_execpter)
                    {
                        conn->_execpter(conn);
                        return;
                    }
                }
                else
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                        break;
                    else if (errno == EINTR)
                        continue;
                    else
                    {
                        if (conn->_execpter)
                        {
                            logMessage(DEBUG, "recv execpter");
                            conn->_execpter(conn);
                            return;
                        }
                    }
                }
            }
        }

        void Sender(Connection *conn)
        {
            conn->_lasttime = time(nullptr);
            logMessage(DEBUG, "start sender");
            while (true)
            {
                logMessage(DEBUG, "outbuffer: %s", conn->_outbuffer.c_str());
                ssize_t s = send(conn->_sock, conn->_outbuffer.c_str(), conn->_outbuffer.size(), 0);
                logMessage(DEBUG, "s = %d, errcode = %d, errstr = %s", s, errno, strerror(errno));
                if (s > 0)
                {
                    if (conn->_outbuffer.empty())
                    {
                        // EnableReadWrite(conn, true, false);
                        break;
                    }
                    else
                        conn->_outbuffer.erase(0, s);
                }
                else
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        logMessage(DEBUG, "send EAGAIN | EWOULDBLOCK");
                        break;
                    }
                    else if (errno == EINTR)
                        continue;
                    else
                    {
                        if (conn->_execpter)
                        {
                            logMessage(DEBUG, "send execpter");
                            conn->_execpter(conn);
                            return;
                        }
                    }
                }
            }
            if (!conn->_outbuffer.empty())
                conn->_tsp->EnableReadWrite(conn, true, true);
            else
                conn->_tsp->EnableReadWrite(conn, true, false);
        }

        void Excepter(Connection *conn)
        {
            _epoller.Control(conn->_sock, 0, EPOLL_CTL_DEL);
            conn->Close();
            _connections.erase(conn->_sock);
            logMessage(DEBUG, "关闭%d 文件描述符的所有的资源", conn->_sock);
            delete conn;
        }

        void Accepter(Connection *conn)
        {
            for (;;)
            {
                std::string clientip;
                uint16_t clientport;
                int err = 0;
                int sock = _sock.Accept(&clientip, &clientport, &err);
                if (sock > 0)
                {
                    AddConnection(sock, EPOLLIN | EPOLLET, std::bind(&TcpServer::Recver, this, std::placeholders::_1), std::bind(&TcpServer::Sender, this, std::placeholders::_1), std::bind(&TcpServer::Excepter, this, std::placeholders::_1));
                    logMessage(NORMAL, "get a new link, info: [%s:%d]", clientip.c_str(), clientport);
                }
                else
                {
                    if (err == EAGAIN || err == EWOULDBLOCK)
                        break;
                    else if (err == EINTR)
                        continue;
                    else
                        break;
                }
            }
        }

        bool IsConnectionExists(int sock)
        {
            auto iter = _connections.find(sock);
            return iter != _connections.end();
        }

        void Loop(int timeout)
        {
            int n = _epoller.Wait(_revs, _num, timeout);
            for (int i = 0; i < n; i++)
            {
                int sock = _revs[i].data.fd;
                uint32_t event = _revs[i].events;

                if (event & EPOLLHUP)
                    event |= EPOLLIN | EPOLLOUT;
                if (event & EPOLLERR)
                    event |= EPOLLIN | EPOLLOUT;

                if ((event & EPOLLIN) && IsConnectionExists(sock) && _connections[sock]->_recver)
                    _connections[sock]->_recver(_connections[sock]);
                if ((event & EPOLLOUT) && IsConnectionExists(sock) && _connections[sock]->_sender)
                    {
                        logMessage(DEBUG, "Loop sender");
                        _connections[sock]->_sender(_connections[sock]);
                    }
            }
        }

    public:
        TcpServer(func_t service, uint16_t port = defaultport)
            : _port(port), _revs(nullptr), _num(num), _service(service)
        {
        }

        void InitServer()
        {
            _sock.Socket();
            _sock.Bind(_port);
            _sock.Listen();

            _epoller.Create();
            AddConnection(_sock.GetFd(), EPOLLIN | EPOLLET, std::bind(&TcpServer::Accepter, this, std::placeholders::_1), nullptr, nullptr);
            _revs = new struct epoll_event[_num];
        }

        void EnableReadWrite(Connection *conn, bool readable, bool writeable)
        {
            uint32_t event = ((readable ? EPOLLIN : 0) | (writeable ? EPOLLOUT : 0) | EPOLLET);
            bool ret = _epoller.Control(conn->_sock, event, EPOLL_CTL_MOD);
            if (!ret)
            {
                logMessage(ERROR, "epoll control error");
                return;
            }
            logMessage(NORMAL, "epoll control success");
        }

        void Dispatcher()
        {
            int timeout = -1;
            while (true)
            {
                Loop(timeout);
                // logMessage(NORMAL, "time out ...");
            }
        }

        ~TcpServer()
        {
            _sock.Close();
            _epoller.Close();
            if (_revs != nullptr)
                delete[] _revs;
        }

    private:
        uint16_t _port;
        Sock _sock;
        Epoller _epoller;
        func_t _service;
        struct epoll_event *_revs;
        std::unordered_map<int, Connection *> _connections;
        int _num;
    };
}