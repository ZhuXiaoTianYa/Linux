#include <iostream>
#include <string>
#include <thread>
#include <functional>

namespace ThreadModule
{
    template <typename T>
    using func_t = std::function<void(T &)>;

    template <typename T>
    class Thread
    {

    private:
        void excute()
        {
            func_(thread_data_);
        }

    public:
        Thread(func_t<T> func, T &data, const std::string name = "none-name")
            : thread_name_(name), thread_data_(data), func_(func), stop_(false)
        {
        }

        static void *threadRoutine(void *args)
        {
            Thread<T> *self = static_cast<Thread<T> *>(args);
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
            if (!stop)
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
        T &thread_data_;
        bool stop_;
        func_t<T> func_;
    };
}