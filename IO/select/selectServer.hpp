#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <functional>
#include "sock.hpp"

namespace select_ns
{
    static const int defaultport = 8086;
    static const int defaultfd = -1;
    static const int fdnum = sizeof(fd_set) * 8;

    using func_t = std::function<std::string(const std::string &)>;

    class SelectServer
    {
    public:
        SelectServer(func_t func, int port = defaultport)
            : _port(port), _listensock(-1), _func(func), _fdarray(nullptr)
        {
        }

        void Print()
        {
            std::cout << "fd list: ";
            for (int i = 0; i < fdnum; i++)
            {
                if (_fdarray[i] != defaultfd)
                    std::cout << _fdarray[i] << " ";
            }
            std::cout << std::endl;
        }

        void initServer()
        {
            _listensock = Sock::Socket();
            Sock::Bind(_listensock, _port);
            Sock::Listen(_listensock);
            _fdarray = new int[fdnum];
            for (int i = 0; i < fdnum; i++)
            {
                _fdarray[i] = defaultfd;
            }
            _fdarray[0] = _listensock;
        }

        void Accepter(int listensock)
        {
            // 走到这里，accept函数不会阻塞
            // select 函数通知，listensock读事件就绪
            std::string clientip;
            uint16_t clientport = 0;
            int sock = Sock::Accept(listensock, &clientip, &clientport);
            // accept = 等 + 获取
            if (sock < 0)
                return;
            logMessage(NORMAL, "accept success [%s:%d]", clientip.c_str(), clientport);
            // sock我们不能直接recv/read
            // 整个代码,只有select有资格检测事件是否就绪
            // 将新的sock 托管给select
            // 将新的sock托管给select 的本质,就是将sock添加到_fdarray数组中即可!
            int i = 0;
            for (; i < fdnum; i++)
            {
                if (_fdarray[i] != defaultfd)
                    continue;
                else
                    break;
            }
            if (i == fdnum)
            {
                logMessage(WARNING, "server is full , please wait");
                close(sock);
            }
            else
            {
                _fdarray[i] = sock;
            }
            Print();
        }

        void Recver(int sock, int pos)
        {
            // 1.读取request
            // 这样读取是有问题的!
            char buffer[1024];
            ssize_t s = recv(sock, buffer, sizeof(buffer) - 1, 0);
            // 这里进行读取的时候不会被阻塞
            if (s > 0)
            {
                buffer[s] = 0;
                logMessage(NORMAL, "client# %s", buffer);
            }
            else if (s == 0)
            {
                close(sock);
                _fdarray[pos] = defaultfd;
                logMessage(NORMAL, "client quit");
                return;
            }
            else
            {
                close(sock);
                _fdarray[pos] = defaultfd;
                logMessage(ERROR, "client quit: %s", strerror(errno));
                return;
            }

            // 2.处理request
            std::string response = _func(buffer);

            // 3.返回response
            // write bug
            write(sock, response.c_str(), response.size());
            logMessage(NORMAL, "write success");
        }

        void HandlerReadEvent(fd_set &rfds)
        {
            for (int i = 0; i < fdnum; i++)
            {
                // 过滤掉非法的fd
                if (_fdarray[i] == defaultfd)
                    continue;
                // 正常的fd
                // 正常的fd不一定就绪了
                // 目前一定是listensock
                if (FD_ISSET(_fdarray[i], &rfds) && _fdarray[i] == _listensock)
                    Accepter(_listensock);
                else if (FD_ISSET(_fdarray[i], &rfds))
                    Recver(_fdarray[i], i);
                else
                {
                }
            }
        }

        void start()
        {
            while (1)
            {
                fd_set rfds;
                // fd_set wfds;
                FD_ZERO(&rfds);
                int maxfd = _fdarray[0];
                for (int i = 0; i < fdnum; i++)
                {
                    if (_fdarray[i] == defaultfd)
                        continue;
                    FD_SET(_fdarray[i], &rfds); // 将合法的fd全部添加到读文件描述符集中

                    if (maxfd < _fdarray[i])
                        maxfd = _fdarray[i]; // 更新所有fd中最大的fd
                }
                logMessage(NORMAL, "max fd is: %d", maxfd);
                // struct timeval timeout = {0, 0};
                int n = select(maxfd + 1, &rfds, nullptr, nullptr, nullptr);
                // 一般而言,要是用select,需要程序员自己维护一个保存所有合法fd的数组.

                switch (n)
                {
                case 0:
                    logMessage(NORMAL, "timeout...");
                    break;
                case -1:
                    logMessage(WARNING, "select err, code: %d,err string: %s", errno, strerror(errno));
                    break;
                default:
                    // 说明有事件就绪了,目前只有一个监听时间就绪了
                    logMessage(NORMAL, "have event ready!");
                    HandlerReadEvent(rfds);
                    break;
                }
            }
        }

    private:
        int _port;
        int _listensock;
        int *_fdarray;
        func_t _func;
    };
}