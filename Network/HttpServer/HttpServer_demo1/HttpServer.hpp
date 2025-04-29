#include <sys/wait.h>
#include <signal.h>
#include <sys/socket.h>
#include <functional>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Protocol.hpp"

namespace server
{
    enum
    {
        USAGE_ERR = 1,
        SOCKET_ERR,
        BIND_ERR,
        LISTEN_ERR
    };

    static const uint16_t gport = 8086;
    static const int gbacklog = 5;
    using func_t = std::function<bool(const HttpRequest &, HttpResponse &)>;

    class HttpServer
    {
    public:
        HttpServer(func_t func, const uint16_t &port = gport)
            : _func(func), _port(port)
        {
        }

        void initServer()
        {
            // 1.创建socket文件套接字对象
            _listensock = socket(AF_INET, SOCK_STREAM, 0);
            if (_listensock < 0)
            {
                exit(SOCKET_ERR);
            }
            // 2.bind绑定自己的网络信息
            struct sockaddr_in local;
            local.sin_family = AF_INET;
            local.sin_port = htons(_port);
            local.sin_addr.s_addr = INADDR_ANY;
            if (bind(_listensock, (struct sockaddr *)&local, sizeof(local)) < 0)
            {
                exit(BIND_ERR);
            }
            // 3.设置socket为监听状态
            if (listen(_listensock, gbacklog) < 0)
            {
                exit(LISTEN_ERR);
            }
        }

        void HandlerHttp(int sock)
        {
            // 1.读到完整的http请求
            // 2.反序列化
            // 3.httprequest,httpresponse,_func(req,resp)
            // 4.resp序列化
            // 5.send
            char buffer[4096];
            HttpRequest req;
            HttpResponse resp;
            size_t n = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if (n > 0)
            {
                buffer[n] = 0;
                req.InitHttpResquest(buffer);
                req.parse();
                _func(req, resp); // req->resp
                send(sock, resp.get_outbuffer().c_str(), resp.get_outbuffer().size(), 0);
            }
        }

        void start()
        {
            for (;;)
            {
                // 4.server 获取新链接
                // sock, 和client进行通信的fd
                struct sockaddr_in peer;
                socklen_t len = sizeof(peer);
                int sock = accept(_listensock, (struct sockaddr *)&peer, &len);
                if (sock < 0)
                {
                    continue;
                }

                // version2 多进程版(2)
                pid_t id = fork();
                if (id == 0)
                {
                    // child
                    close(_listensock);
                    if (fork() > 0)
                        exit(0);
                    HandlerHttp(sock);
                    close(sock);
                    exit(0);
                }
                close(sock);
                waitpid(id, nullptr, 0);
            }
        }

    private:
        int _listensock; // 不是用来数据通信的，它是用来监听链接到来，获取新链接的！
        uint16_t _port;
        func_t _func;
    };
}