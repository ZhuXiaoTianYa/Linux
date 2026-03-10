#include <iostream>
#include <string>
#include <thread>
#include <functional>

namespace ThreadModule
{
    using func_t = std::function<void(std::string)>;
    class Thread
    {
    private:
        void excute()
        {
            func_(thread_name_);
        }

    public:
        Thread(func_t func, const std::string &name = "none-name")
            : thread_name_(name), func_(func), stop_(false)
        {
        }

        static void *threadRoutine(void *args)
        {
            Thread *self = static_cast<Thread *>(args);
            self->excute();
            return nullptr;
        }

        bool start()
        {
            int n = pthread_create(&pid_, nullptr, threadRoutine, this);
            if (!n)
            {
                return false;
            }
            stop_ = false;
            return true;
        }

        void join()
        {
            if (!stop_)
            {
                pthread_join(pid_, nullptr);
            }
        }

        void detach()
        {
            if (!stop_)
            {
                pthread_detach(pid_);
            }
        }

        void stop()
        {
            stop_ = true;
        }

        std::string name()
        {
            return thread_name_;
        }

    private:
        std::string thread_name_;
        pthread_t pid_;
        bool stop_;
        func_t func_;
    };
}