#include <stdio.h>
#include <string.h>
int main()
{
    const char *msg = "hello fwrite\n";
    fwrite(msg, strlen(msg), 1, stdout);

    printf("hello printf\n");
    fprintf(stdout, "hello fprintf\n");
    return 0;
}