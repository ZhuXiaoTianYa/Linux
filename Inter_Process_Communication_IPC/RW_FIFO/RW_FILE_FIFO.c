#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ERR_EXIT(m)         \
    do                      \
    {                       \
        perror(m);          \
        exit(EXIT_FAILURE); \
    } while (0)

int main()
{
    int outfd;
    outfd = open("qwq.bak", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outfd == -1)
        ERR_EXIT("open");
    int infd = open("tp", O_RDONLY);
    if (infd == -1)
        ERR_EXIT("open");

    char buf[1024];
    int n;
    while ((n = read(infd, buf, 1024)) > 0)
    {
        printf("%s\n", buf);
        printf("read pipe\n");
        write(outfd, buf, n);
    }
    close(infd);
    close(outfd);
    unlink("tp");
    return 0;
}