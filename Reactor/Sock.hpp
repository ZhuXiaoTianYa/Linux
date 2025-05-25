#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Log.hpp"
#include "Err.hpp"

const static int backlog=32;
//const static int defaultport = 8086;
const static int defaultsock = -1;

class Sock
{
public:
    Sock()
    :_listensock(defaultsock)
    {}
    ~Sock()
    {
        if(_listensock!=defaultsock)
            close(_listensock);
    }
public:
    void Socket()
    {
        _listensock = socket(AF_INET, SOCK_STREAM, 0);
        if(_listensock<0)
        {
            logMessage(FATAL,"create socket error");
            exit(SOCKET_ERR);
        }
        logMessage(NORMAL, "create socket success: %d", _listensock);
        int opt = 1;
        setsockopt(_listensock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    }

    void Bind(int port)
    {
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port=htons(port);
        if(bind(_listensock,(struct sockaddr*)&local,sizeof(local))<0)
        {
            logMessage(FATAL, "bind socket error");
            exit(BIND_ERR);
        }
        logMessage(NORMAL, "bind socket success");
    }

    void Listen(int _backlog=backlog)
    {
        if (listen(_listensock, _backlog) < 0)
        {
            logMessage(FATAL,"listen socket error");
            exit(LISTEN_ERR);
        }
        logMessage(NORMAL, "listen socket success");
    }

    int Accept(std::string* clientip,uint16_t* clientport,int *err)
    {
        struct sockaddr_in peer;
        socklen_t len=sizeof(peer);
        int sock = accept(_listensock, (struct sockaddr *)&peer, &len);
        *err = errno;
        if (sock < 0)
        {
            logMessage(ERROR, "accept error, next");
        }
        else
        {
            logMessage(NORMAL, "accept a new link success, get new sock: %d", sock);
            *clientip = inet_ntoa(peer.sin_addr);
            *clientport = ntohs(peer.sin_port);
        }
        return sock;
    }

    int GetFd()
    {
        return _listensock;
    }

    void Close()
    {
        if(_listensock!=defaultsock)
            close(_listensock);
    }

private:
    int _listensock;
};