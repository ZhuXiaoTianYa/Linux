#pragma once

#include "Thread.hpp"
#include "LockGuard.hpp"
#include <vector>
#include <queue>
#include <mutex>
#include <pthread.h>
#include <unistd.h>

using namespace ThreadNs;

const int gnum = 3;

template <class T>
class ThreadPool;

template <class T>
class ThreadData
{
public:
    ThreadData(ThreadPool<T> *threadpool, const std::string &name)
        : _name(name), _threadpool(threadpool)
    {
    }

public:
    std::string _name;
    ThreadPool<T> *_threadpool;
};

template <class T>
class ThreadPool
{
private:
    static void *handlerTask(void *args)
    {
        ThreadData<T> *td = static_cast<ThreadData<T> *>(args);
        while (true)
        {
            T t;
            {
                LockGuard lockguard(td->_threadpool->mutex());
                while (td->_threadpool->isQueueEmpty())
                {
                    td->_threadpool->threadWait();
                }
                t = td->_threadpool->pop();
            }
            std::cout << td->_name << " 获取了一个任务: " << t.toTaskString() << " 并处理完成, 结果是: " << t() << std::endl;
        }
        delete td;
        return nullptr;
    }

public:
    void locakQueue()
    {
        pthread_mutex_lock(&_mutex);
    }
    void unlockQueue()
    {
        pthread_mutex_unlock(&_mutex);
    }
    bool isQueueEmpty()
    {
        return _task_queue.empty();
    }
    void threadWait()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }

    T pop()
    {
        T t = _task_queue.front();
        _task_queue.pop();
        return t;
    }

    pthread_mutex_t *mutex()
    {
        return &_mutex;
    }

public:
    ThreadPool(const int &num = gnum)
        : _num(num)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
        for (int i = 0; i < num; i++)
        {
            _threads.push_back(new Thread());
        }
    }

    void run()
    {
        for (const auto &t : _threads)
        {
            ThreadData<T> *td = new ThreadData<T>(this, t->thread_name());
            t->start(handlerTask, td);
            std::cout << t->thread_name() << " start ..." << std::endl;
        }
    }

    void push(const T &in)
    {
        LockGuard Lockguard(&_mutex);
        _task_queue.push(in);
        pthread_cond_signal(&_cond);
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
        for (const auto &t : _threads)
            delete t;
    }

private:
    int _num;
    std::vector<Thread *> _threads;
    std::queue<T> _task_queue;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
};
