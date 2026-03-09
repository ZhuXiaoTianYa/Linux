#include <iostream>
#include <vector>
#include <unistd.h>
#include "Thread.hpp"
#include "LockGuard.hpp"

int g_ticket = 10000;

using namespace ThreadModule;

template <class T>
class ThreadData
{
public:
    ThreadData(T &data, const std::string &name, pthread_mutex_t &mutex)
        : data_(data), thread_name_(name), mutex_(mutex)
    {
    }

public:
    T &data_;
    int get_ticket_ = 0;
    std::string thread_name_;
    pthread_mutex_t &mutex_;
};

void handle(ThreadData<int> &data)
{
    // std::cout << &ticket << std::endl;
    // std::cout << &data.data_ << std::endl;
    while (true)
    {
        if (data.data_ > 0)
        {
            // sleep(1);
            usleep(1000);
            data.data_--;
            std::cout << data.thread_name_ << " get ticket " << "ticket: " << data.data_ << std::endl;
        }
        else
        {
            break;
        }
    }

    return;
}

int main()
{
    std::cout << "qwq" << std::endl;
    std::vector<Thread<ThreadData<int>> *> threads;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);

    for (int i = 0; i < 4; i++)
    {
        char name[10] = {0};
        snprintf(name, sizeof(name), "thread-%d", i);
        ThreadData<int> *data = new ThreadData<int>(g_ticket, name, mutex);
        Thread<ThreadData<int>> *thread = new Thread<ThreadData<int>>(handle, *data, name);
        threads.push_back(thread);
    }

    for (auto &e : threads)
    {
        e->start();
    }

    for (auto &e : threads)
    {
        e->join();
    }

    // for (auto &e : threads)
    // {
    //     e->;
    // }

    pthread_mutex_destroy(&mutex);
    return 0;
}