#include <iostream>
#include <functional>

using Task = std::function<void()>;

// class Task
// {
// public:
//     Task()
//     {
//     }

//     Task(int x, int y)
//         : a_(x), b_(y)
//     {
//     }

//     void Excute()
//     {
//         result_ = a_ + b_;
//     }

//     void operator()()
//     {
//         std::cout << a_ << " + " << b_ << " = " << a_ + b_ << std::endl;
//         result_ = a_ + b_;
//     }

// private:
//     int a_;
//     int b_;
//     int result_;
// };