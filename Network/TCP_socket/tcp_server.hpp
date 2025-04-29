#pragma once
#include "tcp_socket.hpp"
#include <iostream>
#include <functional>
#include <wait.h>

typedef std::function<void(const std::string &req, std::string *resp)> Handler;

class TcpServer
{
public:
    TcpServer(const std::string &ip, uint16_t port)
        : ip_(ip), port_(port)
    {
    }

    void Task(TcpSocket &new_sock, std::string &ip, uint16_t port, Handler handler)
    {
        // 6.进行循环读写
        for (;;)
        {
            std::string req;
            // 7.读取请求,读取失败则结束循环
            bool ret = new_sock.Recv(&req);
            if (!ret)
            {
                printf("[client %s:%d] disconnect!\n", ip.c_str(), port);
                //[注意!]需要关闭socket
                new_sock.Close();
                break;
            }
            // 8.计算响应
            std::string resp;
            handler(req, &resp);
            // 9.写回响应
            new_sock.Send(resp);
            printf("[%s:%d] req: %s, resp:%s\n", ip.c_str(), port, req.c_str(), resp.c_str());
        }
    }

    bool Start(Handler handler)
    {
        // 1.创建 socket
        CHECK_RET(listen_sock_.Socket());
        // 2.绑定端口号
        CHECK_RET(listen_sock_.Bind(ip_, port_));
        // 3.进行监听
        CHECK_RET(listen_sock_.Listen(5));
        // 4.进入事件循环
        for (;;)
        {
            // 5.进行accept
            TcpSocket new_sock;
            std::string ip;
            uint16_t port = 0;
            if (!listen_sock_.Accept(&new_sock, &ip, &port))
            {
                continue;
            }
            printf("[client %s:%d] connect!\n", ip.c_str(), port);
            pid_t pid = fork();
            if (pid == 0)
            {
                listen_sock_.Close();
                if (fork() > 0)
                    exit(0);
                Task(new_sock, ip, port, handler);
                new_sock.Close();
                exit(0);
            }
            new_sock.Close();
            pid_t ret = waitpid(pid, nullptr, 0);
            std::cout << "wait success: " << ret << std::endl;
        }
        return true;
    }

private:
    TcpSocket listen_sock_;
    std::string ip_;
    uint64_t port_;
};