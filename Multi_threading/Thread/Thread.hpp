#pragma once

#include <pthread.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <functional>

class Thread;
class Context
{
public:
    Context()
        : _this(nullptr), _args(nullptr)
    {
    }
    ~Context()
    {
    }

public:
    Thread *_this;
    void *_args;
};

class Thread
{
    // typedef std::function<void*(void*)> func_t;
    using func_t = std::function<void *(void *)>;
    const int num = 1024;

private:
    // 在类内创建线程,想让线程执行对应的方法,需要将方法设置成为static
    static void *start_routine(void *args) // 类内成员,有缺省参数
    {
        Context *ctx = static_cast<Context *>(args);
        void *ret = ctx->_this->run(ctx->_args);
        delete ctx;
        return ret;
        // 静态方法不能调用成员方法或者成员变量
    }

    void *run(void *args)
    {
        return _func(args);
    }

public:
    Thread(func_t func, void *args = nullptr, int number = 0)
        : _func(func), _args(args)
    {
        char buffer[num];
        snprintf(buffer, num, "thread-%d", number);
        _name = buffer;

        // 异常 == if: 意料之外用异常或者if判断
        // assert: 意料之中用assert
        Context *ctx = new Context();
        ctx->_args = _args;
        ctx->_this = this;
        int n = pthread_create(&_tid, NULL, start_routine, ctx);
        assert(n == 0); // 编译debug的方式发布的时候存在,release方式发布时,assert就不存在了,n就是一个定义了,但是没有被使用的变量
        // 在有些编译器下会有warning
        (void)n;
    }

    void join()
    {
        int n = pthread_join(_tid, nullptr);
        assert(n == 0);
        (void)n;
    }

    ~Thread()
    {
        // do nothing
    }

private:
    std::string _name;
    func_t _func;
    void *_args;
    pthread_t _tid;
};