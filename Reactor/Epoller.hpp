#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/epoll.h>
#include "Err.hpp"
#include "Log.hpp"

const static int defaultepfd = -1;
const static int defaultsize = 128;

class Epoller
{
public:
    Epoller()
        : _epfd(defaultepfd)
    {
    }
    ~Epoller()
    {
        if (_epfd != defaultepfd)
            close(_epfd);
    }

public:
    void Create(int size = defaultsize)
    {
        _epfd = epoll_create(size);
        if(_epfd<0)
        {
            logMessage(FATAL,"epoll_create error, code: %d, errstring: %s",errno,strerror(errno));
            exit(EPOLL_CREATE_ERR);
        }
        logMessage(NORMAL, "epoll_create success, epfd= %d", _epfd);
    }

    bool AddEvent(int sock,uint32_t events)
    {
        struct epoll_event ev;
        ev.data.fd = sock;
        ev.events = events;
        if (epoll_ctl(_epfd, EPOLL_CTL_ADD, sock, &ev)!= 0)
        {
            logMessage(ERROR, "epoll socket add error, code: %d, errstr: %s",errno,strerror(errno));
            return false;
        }
        logMessage(NORMAL, "epoll add socket success");
        return true;
    }

    int Wait(struct epoll_event events[],int num,int timeout)
    {
        int n = epoll_wait(_epfd, events, num, timeout);
        return n;
    }

    bool Control(int sock,uint32_t event,int action)
    {
        int n = 0;
        if(action==EPOLL_CTL_MOD)
        {
            struct epoll_event ev;
            ev.data.fd = sock;
            ev.events = event;
            n = epoll_ctl(_epfd, action,sock, &ev);
        }
        else if(action==EPOLL_CTL_DEL)
        {
            n = epoll_ctl(_epfd, action, sock, nullptr);
        }
        else n=-1;
        return n == 0;
    }

    void Close()
    {
        if(_epfd!=defaultepfd)
            close(_epfd);
    }

private:
    int _epfd;
};