#include <signal.h>
#include <stdio.h>

void handler(int sig)
{
    printf("catch a sig : %d\n", sig);
}

int main()
{
    signal(2, handler); // 信号是可以被自定义捕捉的,signal函数就是用来进行信号捕捉的,提前了解一下
    while (1)
        ;
    return 0;
}