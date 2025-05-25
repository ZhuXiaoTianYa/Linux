#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <jsoncpp/json/json.h>

#define SEP " "
#define SEP_LEN strlen(SEP)
#define LINE_SEP "\r\n"
#define LINE_SEP_LEN strlen(LINE_SEP)

enum
{
    OK = 0,
    DIV_ZERO,
    MOD_ZERO,
    OP_ERROR
};

std::string enLength(const std::string &text)
{
    std::string send_string = std::to_string(text.size());
    send_string += LINE_SEP;
    send_string += text;
    send_string += LINE_SEP;
    return send_string;
}

bool deLength(std::string &package, std::string *text)
{
    size_t pos = package.find(LINE_SEP);
    if (pos == std::string::npos)
        return false;
    std::string text_len_string = package.substr(0, pos);
    int text_len = std::stoi(text_len_string);
    *text = package.substr(pos + LINE_SEP_LEN, text_len);
    return true;
}

class Request
{
public:
    Request(int x, int y, char op)
        : _x(x), _y(y), _op(op)
    {
    }
    Request()
        : _x(0), _y(0), _op(0)
    {
    }

    bool serialize(std::string *out)
    {
        *out = "";
        std::string x_string = std::to_string(_x);
        std::string y_string = std::to_string(_y);
        *out += x_string;
        *out += SEP;
        *out += _op;
        *out += SEP;
        *out += y_string;

        return true;
    }

    bool deserialize(std::string &in)
    {
        logMessage(DEBUG, "start deserialize");
        size_t left = in.find(SEP);
        size_t right = in.rfind(SEP);
        if (left == std::string::npos || right == std::string::npos)
            return false;
        if (left == right)
            return false;
        if (right - (left + SEP_LEN) != 1) //?
            return false;
        std::string x_string = in.substr(0, left);
        std::string y_string = in.substr(right + SEP_LEN);
        if (x_string.empty())
            return false;
        if (y_string.empty())
            return false;
        _x = std::stoi(x_string);
        _y = std::stoi(y_string);
        _op = in[left + SEP_LEN];

        logMessage(DEBUG, "_x = %d, _y = %d, _op = %c", _x, _y, _op);

        return true;
    }

public:
    int _x;
    int _y;
    char _op;
};

class Response
{
public:
    Response()
        : _exitcode(0), _result(0)
    {
    }
    Response(int exitcode, int result)
        : _exitcode(exitcode), _result(result)
    {
    }
    bool serialize(std::string *out)
    {
        *out = "";
        std::string ec_string = std::to_string(_exitcode);
        std::string res_string = std::to_string(_result);
        *out += ec_string;
        *out += SEP;
        *out += res_string;

        return true;
    }

    bool deserialize(std::string &in)
    {
        size_t mid = in.find(SEP);
        if (mid == std::string::npos)
            return false;
        std::string ec_string = in.substr(0, mid);
        std::string res_string = in.substr(mid + SEP_LEN);
        _exitcode = std::stoi(ec_string);
        _result = std::stoi(res_string);

        return true;
    }

public:
    int _exitcode;
    int _result;
};

bool ParseOnePackage(std::string &inbuffer, std::string *text)
{
    logMessage(DEBUG, "ParseOnePackage in");
    *text = "";
    size_t pos = inbuffer.find(LINE_SEP);
    if (pos == std::string::npos)
        return false;
    logMessage(DEBUG, "find LINE_SEP");
    std::string text_len_string = inbuffer.substr(0, pos);
    int text_len = std::stoi(text_len_string);
    int total_len = text_len_string.size() + 2 * LINE_SEP_LEN + text_len;
    logMessage(DEBUG, "text_len = %d, total_len = %d.inbuffer.size() = %d", text_len, total_len, inbuffer.size());

    if (inbuffer.size() < total_len)
        return false;

    *text = inbuffer.substr(0, total_len);
    inbuffer.erase(0, total_len);
    logMessage(DEBUG, "ParseOnePackage success");
    return true;
}