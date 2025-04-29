#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Util.hpp"

const std::string sep = "\r\n";
const std::string default_root = "./zhutian";
const std::string home_page = "index.html";

class HttpRequest
{
public:
    HttpRequest() {}
    ~HttpRequest() {}

    void InitHttpResquest(const char *buffer)
    {
        inbuffer = buffer;
        std::cout << inbuffer << std::endl;
    }

    const std::string &get_inbuffer() const
    {
        return inbuffer;
    }
    const std::string &get_method() const
    {
        return method;
    }
    const std::string &get_httpversion() const
    {
        return httpversion;
    }
    const std::string &get_path() const
    {
        return path;
    }
    const std::string &get_url() const
    {
        return url;
    }
    void parse()
    {
        // 1.从inbuffer中拿到第一行,请求行,分隔符\r\n
        std::string line = Util::getOneLine(inbuffer, sep);
        if (line.empty())
            return;
        // 2.从请求行中提取三个字段
        // std::cout<<"line: "<<line<<std::endl;
        std::stringstream ss(line);
        ss >> method >> url >> httpversion;

        // 3.添加web默认路径
        path = default_root; // ./zhutian
        path += url;         // ./zhutian/ , ./qwq/awa/ovo.html
        if (path[path.size()] == '/')
            path += home_page;
    }

private:
    std::string inbuffer;
    std::string reqline;
    std::vector<std::string> reqheader;
    std::string method;
    std::string url;
    std::string httpversion;
    std::string path;
};

class HttpResponse
{
public:
    std::string &get_outbuffer()
    {
        return outbuffer;
    }

private:
    std::string outbuffer;
};