#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    if (mkfifo("awa", 0644) == -1)
    {
        perror("mkfifo");
        return 1;
    }
    return 0;
}