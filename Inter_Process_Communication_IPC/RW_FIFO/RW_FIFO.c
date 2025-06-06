#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define ERR_EXIT(m)         \
    do                      \
    {                       \
        perror(m);          \
        exit(EXIT_FAILURE); \
    } while (0)

int main()
{
    mkfifo("tp", 0666);
    int outfd;
    outfd = open("tp", O_WRONLY);
    if (outfd == -1)
        ERR_EXIT("open");
    char buf[1024];
    int n;
    while ((n = read(0, buf, 1024)) > 0)
    {
        write(outfd, buf, n);
        printf("write pipe\n");
    }
    close(outfd);
    return 0;
}