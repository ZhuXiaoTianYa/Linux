#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t id = fork();
    if (id < 0)
    {
        perror("fork error :");
        return 1;
    }
    else if (id == 0)
    {
        // child
        printf("I am child, pid : %d\n", getpid());
        sleep(10);
    }
    else
    {
        // parent
        printf("I am parent, pid : %d\n", getpid());
        sleep(3);
        exit(0);
    }
    return 0;
}