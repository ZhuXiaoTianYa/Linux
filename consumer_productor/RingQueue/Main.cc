#include <iostream>
#include <time.h>
#include <vector>
#include <unistd.h>
#include "RingQueue.hpp"
#include "Thread.hpp"
#include "Task.hpp"

using namespace ThreadModule;
using namespace RingQueueModule;

const int consumer_num = 3;
const int productor_num = 3;

using blockqueue_t = RingQueue<Task>;

void Print()
{
    std::cout << "qwq" << std::endl;
}

void consumer_func(blockqueue_t &bq)
{
    // std::cout << "qwq" << std::endl;
    while (true)
    {
        Task t;
        bq.pop(&t);
        // std::cout << "Consumer consum data is ";
        t();
        // std::cout << std::endl;
    }
}

void productor_func(blockqueue_t &bq)
{
    // std::cout << "awa" << std::endl;
    while (true)
    {
        // int a = rand() % 10 + 1;
        // int b = rand() % 10 - 1;
        // Task t(a, b);
        // bq.enQueue(t);
        // std::cout << "Productor product data is " << a << " + " << b << std::endl;
        // sleep(1);
        Task t = Print;
        bq.enQueue(t);
        sleep(1);
    }
}

int main()
{
    srand(time(nullptr));
    blockqueue_t *bq = new blockqueue_t();
    std::vector<Thread<blockqueue_t>> consumer_threads;
    std::vector<Thread<blockqueue_t>> productor_threads;
    for (int i = 0; i < consumer_num; i++)
    {
        std::string name = "Thread-consumer-" + std::to_string(i + 1);
        consumer_threads.emplace_back(Thread<blockqueue_t>(consumer_func, *bq, name));
    }
    for (int i = 0; i < productor_num; i++)
    {
        std::string name = "Thread-productor-" + std::to_string(i + 1);
        productor_threads.emplace_back(Thread<blockqueue_t>(productor_func, *bq, name));
    }
    for (auto &e : consumer_threads)
    {
        e.start();
    }
    for (auto &e : productor_threads)
    {
        e.start();
    }
    for (auto &e : consumer_threads)
    {
        e.join();
    }
    for (auto &e : productor_threads)
    {
        e.join();
    }
    std::cout << "QwQ" << std::endl;
    return 0;
}