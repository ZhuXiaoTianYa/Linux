#include <iostream>
#include <unistd.h>
#include "Thread.hpp"
using namespace std;

void *prin(void *args)
{
    int *n = (int *)args;
    cout << "thread: " << pthread_self() << " args: " << *n << endl;
    sleep(1);
    return nullptr;
}

int main()
{
    int i = 1;
    Thread t(prin, &i, 1);
    t.join();
    return 0;
}