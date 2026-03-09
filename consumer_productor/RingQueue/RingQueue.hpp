#include <iostream>
#include <pthread.h>
#include <vector>
#include <semaphore.h>

namespace RingQueueModule
{
    template <class T>
    class RingQueue
    {
    private:
        void p(sem_t &sem)
        {
            sem_wait(&sem);
        }

        void v(sem_t &sem)
        {
            sem_post(&sem);
        }

    public:
        RingQueue(const size_t &cap = 10)
            : cap_(cap), ringQueue_(cap), product_step_(0), consum_step_(0)
        {
            pthread_mutex_init(&productor_mutex_, nullptr);
            pthread_mutex_init(&consumer_mutex_, nullptr);
            sem_init(&room_sem_, 0, cap_);
            sem_init(&data_sem_, 0, 0);
        }

        void enQueue(T &in)
        {
            p(room_sem_);
            pthread_mutex_lock(&productor_mutex_);
            ringQueue_[product_step_++] = in;
            product_step_ = product_step_ % cap_;
            pthread_mutex_unlock(&productor_mutex_);
            v(data_sem_);
        }

        void pop(T *out)
        {
            p(data_sem_);
            pthread_mutex_lock(&consumer_mutex_);
            *out = ringQueue_[consum_step_++];
            consum_step_ = consum_step_ % cap_;
            pthread_mutex_unlock(&consumer_mutex_);
            v(room_sem_);
        }

        ~RingQueue()
        {
            pthread_mutex_destroy(&productor_mutex_);
            pthread_mutex_destroy(&consumer_mutex_);
            sem_destroy(&room_sem_);
            sem_destroy(&data_sem_);
        }

    private:
        std::vector<T> ringQueue_;
        size_t cap_;
        size_t product_step_;
        size_t consum_step_;
        sem_t room_sem_;
        sem_t data_sem_;
        pthread_mutex_t productor_mutex_;
        pthread_mutex_t consumer_mutex_;
    };
}