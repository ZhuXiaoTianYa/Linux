#define __LOCK_GUARD_HPP__

#ifdef __LOCK_GUARD_HPP__

#include <iostream>
#include <pthread.h>

class LockGuard
{
public:
    LockGuard(pthread_mutex_t *mutex)
        : mutex_(mutex)
    {
        pthread_mutex_lock(mutex_);
    }
    ~LockGuard()
    {
        pthread_mutex_unlock(mutex_);
    }

private:
    pthread_mutex_t *mutex_;
};

#endif