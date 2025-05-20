#include <iostream>
#include <unistd.h>
#include <memory>
#include <sys/select.h>
#include "selectServer.hpp"

// using namespace select_ns;

static void usage(std::string proc)
{
    std::cerr << "Usage:\n\t" << proc << " port" << "\n\n";
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
    std::unique_ptr<select_ns::SelectServer> server(new select_ns::SelectServer(transaction));
    server->initServer();
    server->start();
    return 0;
}