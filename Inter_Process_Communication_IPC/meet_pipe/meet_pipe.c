#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main()
{
    int fds[2];
    char buf[1024];
    int len;

    if (pipe(fds) == -1)
        perror("pipe error"), exit(1);

    // read from stdin
    while (fgets(buf, sizeof(buf), stdin))
    {
        len = strlen(buf);
        // write into pipe
        if (write(fds[1], buf, len) != len)
        {
            perror("write to pipe error");
            break;
        }
        memset(buf, 0x00, sizeof(buf));

        // read from pipe
        if ((len = read(fds[0], buf, sizeof(buf))) == -1)
        {
            perror("read from pipe error");
            break;
        }

        // write to stdout
        if (write(1, buf, len) != len)
        {
            perror("write to stdout error");
            break;
        }
    }
    return 0;
}