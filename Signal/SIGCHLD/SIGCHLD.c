#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void handler(int signo)
{
    // 1.有非常多的子进程,在同一时刻退出了

    // waitpid(-1) -> while(1)
    // 2.有非常多的子进程,在同一时刻只有一部分退出了
    while (1)
    {
        pid_t ret = waitpid(-1, NULL, WNOHANG);
        if (ret <= 0)
            break;
    }

    printf("pid: %d, %d 号信号,正在被捕捉!\n", getpid(), signo);
}

void Count(int cnt)
{
    while (cnt)
    {
        printf("cnt: %2d\r", cnt);
        fflush(stdout);
        cnt--;
        sleep(1);
    }
    printf("\n");
}

int main()
{
    // 显示的设置对SIGCHLD进行忽略
    signal(SIGCHLD, SIG_IGN);
    // signal(SIGCHLD, SIG_DFL);
    // signal(SIGCHLD, handler);

    printf("我是父进程, %d, ppid: %d\n", getpid(), getppid());

    pid_t id = fork();
    if (id == 0)
    {
        printf("我是子进程, %d, ppid: %d，我要退出啦\n", getpid(), getppid());
        Count(5);
        exit(1);
    }
    while (1)
        sleep(1);
    return 0;
}