#include <iostream>
#include <pthread.h>
#include <queue>

namespace BlockQueueModule
{
    template <class T>
    class BlockQueue
    {
    private:
        bool isFull()
        {
            return block_queue_.size() >= cap_;
        }

        bool isEmpty()
        {
            return block_queue_.empty();
        }

    public:
        BlockQueue(const int &cap = 10)
            : cap_(cap)
        {
            pthread_mutex_init(&mutex_, nullptr);
            pthread_cond_init(&consumer_cond_, nullptr);
            pthread_cond_init(&productor_cond_, nullptr);
            productor_wait_num_ = 0;
            consumer_wait_num_ = 0;
        }

        void enQueue(T &in)
        {
            pthread_mutex_lock(&mutex_);
            while (isFull())
            {
                productor_wait_num_++;
                pthread_cond_wait(&productor_cond_, &mutex_);
                productor_wait_num_--;
            }
            block_queue_.push(in);
            pthread_mutex_unlock(&mutex_);
            if (consumer_wait_num_ > 0)
                pthread_cond_signal(&consumer_cond_);
        }

        void pop(T *out)
        {
            pthread_mutex_lock(&mutex_);
            while (isEmpty())
            {
                consumer_wait_num_++;
                pthread_cond_wait(&consumer_cond_, &mutex_);
                consumer_wait_num_--;
            }
            *out = block_queue_.front();
            block_queue_.pop();
            pthread_mutex_unlock(&mutex_);
            if (productor_wait_num_ > 0)
                pthread_cond_signal(&productor_cond_);
        }

        ~BlockQueue()
        {
            pthread_mutex_destroy(&mutex_);
            pthread_cond_destroy(&consumer_cond_);
            pthread_cond_destroy(&productor_cond_);
        }

    private:
        size_t cap_;                    // 总上线
        std::queue<T> block_queue_;     // 阻塞队列
        pthread_mutex_t mutex_;         // 保护block_queue的锁
        pthread_cond_t consumer_cond_;  // 专门给消费者提供的条件变量
        pthread_cond_t productor_cond_; // 专门给生产者提供的条件变量
        size_t productor_wait_num_;
        size_t consumer_wait_num_;
    };
}