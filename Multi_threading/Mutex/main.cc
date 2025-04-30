#include "Mutex.hpp"
#include <iostream>

using namespace std;

int main()
{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    {
        LockGuard lockguard(&mutex);
    }
    return 0;
}