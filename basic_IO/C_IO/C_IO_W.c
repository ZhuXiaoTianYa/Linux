#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    FILE *fp = fopen("myfile", "w");
    if (!fp)
    {
        printf("fopen error!\n");
        exit(1);
    }
    const char *msg = "hello world!\n";
    int count = 5;
    while (count--)
    {
        fwrite(msg, strlen(msg), 1, fp);
    }
    fclose(fp);
    return 0;
}