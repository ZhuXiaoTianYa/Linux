#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <assert.h>
#include <string.h>

#define DUBUG

#define NUM 1024
#define OPT_NUM 64
#define USER "USER"
#define MY_PWD "PWD"
#define HOSTNAME "HOSTNAME"

char lineCommand[NUM];
char *myargv[OPT_NUM]; // 指针数组
char delim[] = " ";
int lastCode = 0;
int lastSig = 0;

int main()
{
    while (1)
    {
        printf("[%s@%s %s ]$ ", getenv(USER), getenv(HOSTNAME), getenv(MY_PWD)); // 输出提示符
        fflush(stdout);
        char *s = fgets(lineCommand, sizeof(lineCommand) - 1, stdin); // 获取用户输入,输入时,输入\n
        assert(s);
        lineCommand[strlen(lineCommand) - 1] = 0;

        // 字符串切割
        //"ls -a -l -i" -> "ls" "-a" "-l" "-i" -> l->n
        myargv[0] = strtok(lineCommand, delim);
        int i = 1;
        if (myargv[0] != NULL && strcmp(myargv[0], "ls") == 0)
            myargv[i++] = "--color=auto";
        // 如果没有字串了,strtok->NULL,myargv[end]=NULL
        while (myargv[i++] = strtok(NULL, delim))
            ;

        // 如果是cd命令,不需要创建子进程,让shell自己执行对应的命令,本质就是执行系统接口
        // 像这种不需要让我们的子进程来执行,而是让shell自己执行的命令 --- 内建/内置命令
        if (myargv[0] != NULL && strcmp(myargv[0], "cd") == 0)
        {
            if (myargv[1] != NULL)
                chdir(myargv[1]);
            continue;
        }
        if (myargv[0] != NULL && myargv[1] != NULL && strcmp(myargv[0], "echo") == 0)
        {
            if (strcmp(myargv[1], "$?") == 0)
            {
                printf("%d, %d\n", lastCode, lastSig);
            }
            else
            {
                printf("%s\n", myargv[1]);
            }
            continue;
        }
        // 内建命令 ---> echo

        // 测试是否成功,条件编译
#ifdef DEBUG
        for (int n = 0; myargv[n]; n++)
        {
            printf("myargv[%d]: %s\n", n, myargv[n]);
        }
#endif

        // 执行命令
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork error:");
            exit(2);
        }
        if (pid == 0)
        {
            execvp(myargv[0], myargv);
            exit(1);
        }
        int status = 0;
        pid_t ret = waitpid(pid, &status, 0);
        if (ret == pid)
        {
            lastCode = ((status >> 8) & 0xFF);
            lastSig = (status & 0x7F);
        }
    }

    return 0;
}