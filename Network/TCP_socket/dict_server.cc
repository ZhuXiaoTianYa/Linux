#include <unordered_map>
#include "tcp_server.hpp"

std::unordered_map<std::string, std::string> g_dict;

void Translate(const std::string &rep, std::string *resp)
{
    std::unordered_map<std::string, std::string>::iterator it = g_dict.find(rep);
    if (it == g_dict.end())
    {
        *resp = "未找到";
        return;
    }
    *resp = it->second;
    return;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage ./dict_server [ip] [port]\n");
        return 1;
    }
    // 1.初始化字典
    g_dict.insert(std::make_pair("hello", "你好"));
    g_dict.insert(std::make_pair("world", "世界"));
    g_dict.insert(std::make_pair("C++", "最好的编程语言"));
    g_dict.insert(std::make_pair("zhutian", "烛天"));
    // 2.启动服务器
    TcpServer server(argv[1], atoi(argv[2]));
    server.Start(Translate);
    return 0;
}