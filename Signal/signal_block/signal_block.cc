#include <signal.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>

#define BLOCK_SIGNAL 2
#define MAX_SIGNUM 31

using namespace std;

static vector<int> sigarr = {2};

static void myhandler(int signo)
{
    cout << signo << " 号信号已经被递达!!" << endl;
}

static void show_pending(const sigset_t &pending)
{
    for (int signo = MAX_SIGNUM; signo >= 1; signo--)
    {
        if (sigismember(&pending, signo))
        {
            cout << "1";
        }
        else
            cout << "0";
    }
    cout << endl;
}

int main()
{
    for (const auto &sig : sigarr)
        signal(sig, myhandler);
    // 1.先尝试屏蔽指定信号
    sigset_t block, oblock, pending;
    // 1.1初始化
    sigemptyset(&block);
    sigemptyset(&oblock);
    sigemptyset(&pending);
    // 1.2添加要屏蔽的信号
    for (const auto &sig : sigarr)
        sigaddset(&block, sig);
    // 1.3开始屏蔽,设置进内核(进程)
    sigprocmask(SIG_SETMASK, &block, &oblock);

    // 2.遍历打印pending信号集
    int cnt = 10;
    while (true)
    {
        // 2.1初始化
        sigemptyset(&pending);
        // 2.2获取它
        sigpending(&pending);
        // 2.3打印它
        show_pending(pending);
        // 3.休眠
        sleep(1);
        if (cnt-- == 0)
        {
            sigprocmask(SIG_SETMASK, &oblock, &block); // 一旦对特定信号进行解除屏蔽,一般os要至少立马下达一个信号!
            cout << "恢复对信号的屏蔽,不屏蔽任何信号\n";
        }
    }
    return 0;
}