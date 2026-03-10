#include <iostream>
#include <vector>
#include <unistd.h>
#include "ThreadPool.hpp"

using namespace ThreadModule;
using namespace ThreadPoolModule;

int main()
{
    ThreadPool<Task> tp;
    tp.initThreadPool();
    tp.start();
    tp.stop();
    tp.wait();
    return 0;
}