#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

void *thread_run(void *arg)
{
    pthread_detach(pthread_self());
    printf("%s\n", (char *)arg);
    return NULL;
}

int main()
{
    pthread_t tid;
    if ((tid = pthread_create(&tid, NULL, thread_run, "thread1 run ...")) != 0)
    {
        printf("create thread error\n");
        return 1;
    }
    int ret = 0;
    sleep(1); // 很重要,要先让线程分离,再等待线程退出

    if (pthread_join(tid, NULL) == 0)
    {
        printf("pthread wait success\n");
        ret = 0;
    }
    else
    {
        printf("pthread wait failed\n");
        ret = 1;
    }

    return ret;
}