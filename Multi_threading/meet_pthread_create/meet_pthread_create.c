#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void *rout(void *arg)
{
    int i;
    for (;;)
    {
        printf("I am thread 1\n");
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    int ret;
    if ((ret = pthread_create(&tid, NULL, rout, NULL)) != 0)
    {
        fprintf(stderr, "pthread_create : %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    int i;
    for (;;)
    {
        printf("I am main thread\n");
        sleep(1);
    }
    return 0;
}