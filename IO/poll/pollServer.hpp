#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <poll.h>
#include "sock.hpp"

namespace poll_ns
{

    static const int defaultport = 8086;
    static const int num = 2048;
    static const int defaultfd = -1;

    using func_t = std::function<std::string(const std::string &)>;

    class PollServer
    {
    public:
        PollServer(func_t func, uint16_t port = defaultport)
            : _func(func), _port(port), _listensock(-1), _rfds(nullptr)
        {
        }
        void ResetItem(int pos)
        {
            _rfds[pos].fd = defaultfd;
            _rfds[pos].events = 0;
            _rfds[pos].revents = 0;
        }
        void InitServer()
        {
            _listensock = Sock::Socket();
            Sock::Bind(_listensock, _port);
            Sock::Listen(_listensock);
            _rfds = new struct pollfd[num];
            for (int i = 0; i < num; i++)
            {
                ResetItem(i);
            }
            _rfds[0].fd = _listensock;
            _rfds[0].events = POLLIN;
            logMessage(NORMAL, "Server init success");
        }

        void Print()
        {
            std::cout << "fd list: ";
            for (int i = 0; i < num; i++)
            {
                if (_rfds[i].fd != defaultfd)
                    std::cout << _rfds[i].fd << " ";
            }
            std::cout << std::endl;
        }

        void Recver(int pos)
        {
            char buffer[1024];
            size_t s = recv(_rfds[pos].fd, buffer, sizeof(buffer) - 1, 0);
            if (s > 0)
            {
                buffer[s] = 0;
                logMessage(NORMAL, "client# %s", buffer);
            }
            else if (s == 0)
            {
                close(_rfds[pos].fd);
                ResetItem(pos);
                logMessage(NORMAL, "client quit");
                return;
            }
            else
            {
                close(_rfds[pos].fd);
                ResetItem(pos);
                logMessage(ERROR, "client quit: %s", strerror(errno));
                return;
            }
            std::string response = _func(buffer);
            // write bug
            write(_rfds[pos].fd, response.c_str(), response.size());
        }

        void Accepter(int listensock)
        {
            std::string clientip;
            uint16_t clientport;
            int sock = Sock::Accept(listensock, &clientip, &clientport);
            if (sock < 0)
                return;
            logMessage(NORMAL, "accept success [%s:%d]", clientip.c_str(), clientport);
            int i = 0;
            for (; i < num; i++)
            {
                if (_rfds[i].fd != defaultfd)
                    continue;
                else
                    break;
            }
            if (i == num)
            {
                logMessage(WARNING, "server is full,please wait");
                close(sock);
            }
            else
            {
                _rfds[i].fd = sock;
                _rfds[i].events = POLLIN;
                _rfds[i].revents = 0;
            }
            Print();
        }
        void HandlerReadEvent()
        {
            for (int i = 0; i < num; i++)
            {
                if (_rfds[i].fd == defaultfd)
                    continue;
                if (!(_rfds[i].events & POLLIN))
                    continue;
                if (_rfds[i].fd == _listensock && (_rfds[i].revents & POLLIN))
                    Accepter(_listensock);
                else if (_rfds[i].revents & POLLIN)
                    Recver(i);
                else
                {
                }
            }
        }

        void start()
        {
            int timeout = -1;
            while (1)
            {
                int n = poll(_rfds, num, timeout);
                switch (n)
                {
                case 0:
                    logMessage(NORMAL, "timeout...");
                    break;
                case -1:
                    logMessage(WARNING, "poll error , code:%d ,err string: %s", errno, strerror(errno));
                    break;
                default:
                    logMessage(NORMAL, "have event ready!");
                    HandlerReadEvent();
                    // HandlerWriteEvent(wfds);
                    break;
                }
            }
        }

        ~PollServer()
        {
            if (_listensock > 0)
                close(_listensock);
            if (_rfds)
                delete[] _rfds;
        }

    private:
        int _listensock;
        int _port;
        struct pollfd *_rfds;
        func_t _func;
    };
}