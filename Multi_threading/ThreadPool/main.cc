#include "ThreadPool.hpp"
#include "Task.hpp"
#include <memory>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    // 一大堆的代码...
    ThreadPool<Task> *threadpool = new ThreadPool<Task>(5);
    threadpool->run();

    srand(time(nullptr) % 0x76874328);
    while (1)
    {
        int x = rand() % 100;
        int y = rand() % 100;
        Task t(x, y, oper[rand() % 5], mymath);
        threadpool->push(t);
        std::cout << "录入了一个任务: " << t.toTaskString() << std::endl;
        sleep(1);
        // usleep(1000);
    }

    return 0;
}