#include "Sock.hpp"
#include "Protocol.hpp"
#include <stdio.h>
int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8086);
    connect(sock, (struct sockaddr *)&server, sizeof(server));
    while (true)
    {
        int x, y;
        char op;
        scanf("%d %c %d", &x, &op, &y);
        class Request qe(x, y, op);
        std::string qes;
        qe.serialize(&qes);
        std::string sen = enLength(qes);
        std::cout << sen << std::endl;
        send(sock, sen.c_str(), sen.size(), 0);
        char buf[1024];
        int n = recv(sock, buf, sizeof(buf) - 1, 0);
        buf[n] = 0;
        std::cout << buf << std::endl;
    }
}