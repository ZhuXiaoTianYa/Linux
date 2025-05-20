#pragma once

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <memory.h>
#include <unistd.h>
#include "log.hpp"
#include "err.hpp"


class Sock
{
    const static int backlog = 32;

public:
    static int Socket()
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock<0)
        {
            logMessage(FATAL, "create socket error");
            exit(SOCKET_ERR);
        }
        logMessage(NORMAL, "create socket success");
        int opt=1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        return sock;
    }

    static void Bind(int sock,uint16_t port)
    {
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port = htons(port);
        if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
        {
            logMessage(FATAL, "bind socket error");
            exit(BIND_ERR);
        }
        logMessage(NORMAL, "bind socket success");
    }

    static void Listen(int sock,int _backlog=backlog)
    {
        if (listen(sock, _backlog)<0)
        {
            logMessage(FATAL, "listen socket error");
            exit(LISTEN_ERR);
        }
        logMessage(NORMAL, "listen socket success");
    }

    static int Accept(int listensock,std::string *clientip,uint16_t * clientport)
    {
        struct sockaddr_in peer;
        socklen_t len=sizeof(peer);
        int sock = accept(listensock, (struct sockaddr *)&peer, &len);
        if(sock<0)
            logMessage(ERROR,"accept error , next");
        else
        {
            logMessage(NORMAL, "accept a new link success, get a new sock: %d", sock);
            *clientip = inet_ntoa(peer.sin_addr);
            *clientport = ntohs(peer.sin_port);
        }
        return sock;
    }
};