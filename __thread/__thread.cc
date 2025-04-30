#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

using namespace std;

const int i = 5;
__thread int val = i;

void *print1(void *args)
{
    while (val--)
    {
        printf("thread_t: %x ,val: %d\n", pthread_self(), val);
        sleep(1);
    }
}

void *print2(void *args)
{
    while (val != 10)
    {
        printf("thread_t: %x ,val: %d\n", pthread_self(), val++);
        sleep(1);
    }
}

int main()
{
    pthread_t t1, t2;
    int ret = pthread_create(&t1, NULL, print1, nullptr);
    if (ret != 0)
    {
        printf("%s\n", strerror(ret));
        exit(1);
    }
    ret = pthread_create(&t2, NULL, print2, nullptr);
    if (ret != 0)
    {
        printf("%s\n", strerror(ret));
        exit(1);
    }
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    return 0;
}