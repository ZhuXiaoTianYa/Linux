#pragma once

#include <iostream>
#include <sys/epoll.h>
#include <cstring>
#include <string>
#include <functional>
#include <cstring>
#include "log.hpp"
#include "sock.hpp"
#include "err.hpp"

namespace epoll_ns
{
    using func_t = std::function<std::string(const std::string &)>;
    static const int defaultport = 8086;
    static const int defaultvalue = -1;
    static const int defaultnum = 64;
    static const int size = 128;
    class EpollServer
    {
    public:
        EpollServer(func_t func, uint16_t port = defaultport, int num = defaultnum)
            : _port(port), _revs(nullptr), _listensock(defaultvalue), _epfd(defaultvalue), _num(num), _func(func)
        {
        }

        void InitServer()
        {
            _listensock = Sock::Socket();
            Sock::Bind(_listensock, _port);
            Sock::Listen(_listensock);
            _epfd = epoll_create(size);
            if (_epfd < 0)
            {
                logMessage(FATAL, "epoll create error: %s", strerror(errno));
                exit(EPOLL_CREATE_ERR);
            }
            struct epoll_event ev;
            ev.events = EPOLLIN;
            ev.data.fd = _listensock;
            epoll_ctl(_epfd, EPOLL_CTL_ADD, _listensock, &ev);
            _revs = new struct epoll_event[_num];
            logMessage(NORMAL, "init server success");
        }

        void HandlerEvent(int readyNum)
        {
            for (int i = 0; i < readyNum; i++)
            {
                uint32_t events = _revs[i].events;
                int sock = _revs[i].data.fd;
                if (sock == _listensock && (events & EPOLLIN))
                {
                    std::string clientip;
                    uint16_t clientport;
                    int sock = Sock::Accept(_listensock, &clientip, &clientport);
                    if (sock < 0)
                    {
                        logMessage(WARNING, "accept error");
                        continue;
                    }
                    struct epoll_event ev;
                    ev.data.fd = sock;
                    ev.events = EPOLLIN;
                    epoll_ctl(_epfd, EPOLL_CTL_ADD, sock, &ev);
                }
                else if (events & EPOLLIN)
                {
                    char buffer[1024];
                    int n = recv(sock, buffer, sizeof(buffer) - 1, 0);
                    if (n > 0)
                    {
                        buffer[n] = 0;
                        logMessage(NORMAL, "client# %s", buffer);
                        std::string response = _func(buffer);
                        send(sock, response.c_str(), response.size(), 0);
                    }
                    else if (n == 0)
                    {
                        epoll_ctl(_epfd, EPOLL_CTL_DEL, sock, nullptr);
                        close(sock);
                        logMessage(NORMAL, "client quit");
                    }
                    else
                    {
                        epoll_ctl(_epfd, EPOLL_CTL_DEL, sock, nullptr);
                        close(sock);
                        logMessage(ERROR, "recv error, code: %d, errstring:%s", errno, strerror(errno));
                    }
                }
                else
                {
                }
            }
        }

        void Start()
        {
            int timeout = -1;
            while (1)
            {
                int n = epoll_wait(_epfd, _revs, _num, timeout);
                switch (n)
                {
                case 0:
                    logMessage(NORMAL, "timeout...");
                    break;
                case -1:
                    logMessage(WARNING, "epoll_wait failed, code: %d, errstring:%s", errno, strerror(errno));
                    break;
                default:
                    logMessage(NORMAL, "have event ready!");
                    HandlerEvent(n);
                    break;
                }
            }
        }

    private:
        int _listensock;
        int _epfd;
        uint16_t _port;
        struct epoll_event *_revs;
        int _num;
        func_t _func;
    };
}