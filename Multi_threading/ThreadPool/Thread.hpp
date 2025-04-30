#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <functional>
#include <pthread.h>

namespace ThreadNs
{
    typedef std::function<void *(void *)> func_t;
    const int num = 1024;

    class Thread
    {
    private:
        static void *start_routine(void *args)
        {
            Thread *_this = static_cast<Thread *>(args);
            return _this->callback();
        }

    public:
        Thread()
        {
            char namebuffer[num];
            snprintf(namebuffer, sizeof namebuffer, "thread-%d", threadnum++);
            _name = namebuffer;
        }

        void start(func_t fun, void *args = nullptr)
        {
            _func = fun;
            _args = args;
            int n = pthread_create(&_tid, NULL, start_routine, this);
            if (n != 0)
            {
                printf("phread_create: %s\n", strerror(n));
            }
        }

        void join()
        {
            int n = pthread_join(_tid, nullptr);
            if (n != 0)
            {
                printf("pthread_join: %s\n", strerror(n));
            }
        }

        std::string thread_name()
        {
            return _name;
        }

        ~Thread()
        {
        }

        void *callback()
        {
            return _func(_args);
        }

    private:
        std::string _name;
        func_t _func;
        void *_args;
        pthread_t _tid;
        static int threadnum;
    };
    int Thread::threadnum = 1;
} // end namespace ThreadNs
