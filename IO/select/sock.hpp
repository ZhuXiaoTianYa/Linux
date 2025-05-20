#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <memory.h>
#include "log.hpp"
#include "err.hpp"

class Sock
{
    const static int backlog = 32;

public:

    static int Socket()
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            perror("socket");
            logMessage(ERROR, "socket creat false");
            return false;
        }
        logMessage(NORMAL, "socket create successful");
        return sock;
    }

    static void Bind(int sock,uint16_t port)
    {
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = INADDR_ANY;
        if (bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            logMessage(FATAL, "bind false");
            exit(BIND_ERR);
        }
        logMessage(NORMAL, "bind socket success");
    }

    static void Listen(int sock,int num = backlog) 
    {
        if (listen(sock, num) < 0)
        {
            logMessage(FATAL,"listen socket false");
            exit(LISTEN_ERR);
        }
        logMessage(NORMAL, "listen socket success");
    }

    static int Accept(int listensock,std::string* clientip,uint16_t* clientport)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int sock = accept(listensock, (struct sockaddr *)&peer, &len);
        if(sock<0)
        {
            logMessage(ERROR, "accept error , next");
        }
        else
        {
            logMessage(NORMAL, "accept a new link success, get new sock: %d", sock);
            *clientip = inet_ntoa(peer.sin_addr);
            *clientport = ntohs(peer.sin_port);
        }
        return sock;
    }

};