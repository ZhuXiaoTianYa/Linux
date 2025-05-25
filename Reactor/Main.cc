#include <iostream>
#include <memory>
#include "TcpServer.hpp"
#include "Protocol.hpp"

static void usage(std::string proc)
{
    std::cerr << "Usage:\n\t" << proc << " port"
              << "\n\n";
}

bool cal(Request &req, Response &resp)
{
    resp._exitcode = OK;
    resp._result = OK;
    switch (req._op)
    {
    case '+':
        resp._result = req._x + req._y;
        break;
    case '-':
        resp._result = req._x - req._y;
        break;
    case '*':
        resp._result = req._x * req._y;
        break;
    case '/':
    {
        if (req._y == 0)
            resp._exitcode = DIV_ZERO;
        else
            resp._result = req._x / req._y;
    }
    break;
    case '%':
    {
        if (req._y == 0)
            resp._exitcode = MOD_ZERO;
        else
            resp._result = req._x % req._y;
    }
    break;
    default:
        resp._exitcode = OP_ERROR;
        break;
    }
    return true;
}

void calculate(tcpserver::Connection *conn)
{
    std::string onePackage;
    while (ParseOnePackage(conn->_inbuffer, &onePackage))
    {
        std::string reqStr;
        if (!deLength(onePackage, &reqStr))
            return;
        std::cout << "去掉报头的正文: \n"
                  << reqStr << std::endl;
        Request req;
        if (!req.deserialize(reqStr))
            return;
        logMessage(DEBUG, "deserialize success");
        Response resp;
        cal(req, resp);
        std::string respStr;
        resp.serialize(&respStr);
        logMessage(DEBUG, "serialize success");
        conn->_outbuffer += enLength(respStr);
        std::cout << "-----------result: " << conn->_outbuffer << std::endl;
    }
    if (conn->_sender)
        conn->_sender(conn);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<tcpserver::TcpServer> tp(new tcpserver::TcpServer(calculate, port));
    tp->InitServer();
    tp->Dispatcher();
    return 0;
}