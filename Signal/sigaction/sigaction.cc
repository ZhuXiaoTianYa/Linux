#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;

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

void handler(int signo)
{
    cout << "get a signo: " << signal << "正在处理中..." << endl;
    Count(20);
}

int main()
{
    struct sigaction act, oact;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigisemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, 3);
    sigaction(SIGINT, &act, &oact);
    while (1)
        sleep(1);
    return 0;
}