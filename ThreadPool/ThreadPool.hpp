#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include "Task.hpp"
#include "Thread.hpp"

namespace ThreadPoolModule
{
    using namespace ThreadModule;

    template <class T>
    class ThreadPool
    {
    private:
        void lockQueue()
        {
            pthread_mutex_lock(&mutex_);
        }

        void unlockQueue()
        {
            pthread_mutex_unlock(&mutex_);
        }

        void threadSleep()
        {
            pthread_cond_wait(&cond_, &mutex_);
        }

        void threadWakeup()
        {
            pthread_cond_signal(&cond_);
        }

        void threadWakeupAll()
        {
            pthread_cond_broadcast(&cond_);
        }

    public:
        ThreadPool(const size_t &thread_num = 5)
            : thread_num_(thread_num), is_running_(false), wait_num_(0)
        {
            pthread_mutex_init(&mutex_, nullptr);
            pthread_cond_init(&cond_, nullptr);
        }

        void handlerTask(std::string name)
        {
            while (true)
            {
                lockQueue();
                while (task_queue_.empty() && is_running_)
                {
                    wait_num_++;
                    threadSleep();
                    wait_num_--;
                }
                if (task_queue_.empty() && !is_running_)
                {
                    unlockQueue();
                    break;
                }
                T t = task_queue_.front();
                task_queue_.pop();
                unlockQueue();
                t();
            }
        }

        void initThreadPool()
        {
            for (int num = 0; num < thread_num_; num++)
            {
                std::string name("thread-" + std::to_string(num + 1));
                threads_.emplace_back(std::bind(&ThreadPool::handlerTask, this, std::placeholders::_1), name);
            }
        }

        void start()
        {
            is_running_ = true;
            for (auto &thread : threads_)
            {
                thread.start();
            }
        }

        void stop()
        {
            lockQueue();
            is_running_ = false;
            unlockQueue();
            threadWakeupAll();
        }

        bool enQueue(const Task &in)
        {
            bool ret = false;
            lockQueue();
            if (is_running_)
            {
                task_queue_.push(in);
                ret = true;
                if (wait_num_ > 0)
                    threadWakeup();
            }
            unlockQueue();
            return ret;
        }

        void wait()
        {
            for (auto &thread : threads_)
            {
                thread.join();
            }
        }

        ~ThreadPool()
        {
            pthread_mutex_destroy(&mutex_);
            pthread_cond_destroy(&cond_);
        }

    private:
        std::vector<Thread> threads_;
        std::queue<T> task_queue_;
        size_t thread_num_;
        size_t wait_num_;
        bool is_running_;
        pthread_mutex_t mutex_;
        pthread_cond_t cond_;
    };
}