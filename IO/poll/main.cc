#include <iostream>
#include <unistd.h>
#include <memory>
#include <sys/select.h>
#include "pollServer.hpp"
// using namespace select_ns;

static void usage(std::string proc)
{
    std::cout << "Usage:\n\t" << proc << " port" << "\n\n";
}

std::string transaction(const std::string &request)
{
    return request;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage(argv[0]);
        exit(USAGE_ERR);
    }
    std::unique_ptr<poll_ns::PollServer> svr(new poll_ns::PollServer(transaction));
    svr->InitServer();
    svr->start();
    return 0;
}