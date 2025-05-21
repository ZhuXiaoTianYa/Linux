#include <iostream>
#include <memory>
#include "epollServer.hpp"

static void usage(std::string proc)
{
    std::cerr << "Usage:\n\t" << proc << " port" << "\n\n";
}

std::string echo(const std::string &message)
{
    return "I am epollserver, " + message;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<epoll_ns::EpollServer> epollsvr(new epoll_ns::EpollServer(echo));
    epollsvr->InitServer();
    epollsvr->Start();
    return 0;
}