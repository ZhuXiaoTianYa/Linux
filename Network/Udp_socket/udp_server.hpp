#pragma once
#include "udp_socket.hpp"

// C 式写法
// typedef void (*Handler)(const std::string& req,std::string* resp);
// C++ 11式写法,能够兼容函数指针，仿函数和lamda
#include <functional>
typedef std::function<void(const std::string &req, std::string *resp)> Handler;

class UdpServer
{
public:
    UdpServer()
    {
        assert(sock_.Socket());
    }

    ~UdpServer()
    {
        sock_.Close();
    }

    bool Start(const std::string &ip, uint16_t port, Handler handler)
    {
        // 1.创建 socket
        // 2. 绑定端口号
        bool ret = sock_.Bind(ip, port);
        if (!ret)
        {
            return false;
        }
        // 3. 进入事件循环
        for (;;)
        {
            // 4. 尝试读取请求
            std::string req;
            std::string resp;
            std::string remote_ip;
            uint16_t remote_port = 0;
            bool ret = sock_.RecvFrom(&req, &remote_ip, &remote_port);
            if (!ret)
            {
                continue;
            }
            // 5. 根据请求计算响应
            handler(req, &resp);
            sock_.SendTo(resp, remote_ip, remote_port);
        }
        sock_.Close();
        return true;
    }

private:
    UdpSocket sock_;
};