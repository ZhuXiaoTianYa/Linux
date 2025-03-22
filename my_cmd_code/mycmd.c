#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define USER "USER"
#define MY_ENV "MY_ENV"
#define MYPWD "PWD"

//"ls -a -b -c -d -e" ------>长字符串
//"ls" "-a" "-b" "-c" "-d"
// ./mycmd -a/-b/-c
// ./mycmd -ab/-ac/-bc

int main(int argc, char *argv[], char *env[])
{
    extern char **environ;
    for (int i = 0; environ[i]; i++)
    {
        printf("%d:%s\n", i, environ[i]);
    }

    // printf("%s\n", getenv("PATH"));

    // for (int i = 0; env[i]; i++)
    // {
    //     printf("env[%d]:%s\n", i, env[i]);
    // }

    // for (int i = 0; i < argc; i++)
    // {
    //     printf("argv[%d]:%s\n", i, argv[i]);
    // }
    // if (argc != 2)
    // {
    //     printf("Usage: \n\t%s [-a/-b/-c/-ab/-bc/-ac/-abc]", argv[0]);
    //     return 1;
    // }
    // if (strcmp("-a", argv[1]) == 0)
    // {
    //     printf("功能a\n");
    // }
    // if (strcmp("-b", argv[1]) == 0)
    // {
    //     printf("功能b\n");
    // }
    // if (strcmp("-c", argv[1]) == 0)
    // {
    //     printf("功能c\n");
    // }
    // if (strcmp("-ab", argv[1]) == 0)
    // {
    //     printf("功能ab\n");
    // }
    // if (strcmp("-bc", argv[1]) == 0)
    // {
    //     printf("功能bc\n");
    // }
    // if (strcmp("-ac", argv[1]) == 0)
    // {
    //     printf("功能ac\n");
    // }
    // if (strcmp("-abc", argv[1]) == 0)
    // {
    //     printf("功能abc\n");
    // }

    // printf("%s\n", getenv(MYPWD));
    // char *myenv = getenv(MY_ENV);
    // if (myenv == NULL)
    // {
    //     printf("%s, not found\n", MY_ENV);
    //     return 1;
    // }
    // printf("%s=%s\n", MY_ENV, myenv);

    // char *who = getenv(USER);
    // if (strcmp(who, "root") == 0)
    // {
    //     printf("user: %s\n", who);
    // }
    // else
    // {
    //     printf("权限不足!\n");
    // }

    return 0;
}