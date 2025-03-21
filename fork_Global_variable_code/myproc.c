#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int global_variable =100;

int main()
{

  pid_t id=fork();
  if(id<0)
  {
    printf("fork error.\n");
    return 1;
  }
  else if(id==0)
  {
    int cnt =0;
    while(1)
    {
      printf("I am child , pid: %d,ppid: %d | global_variable: %d, &global_variable: %p\n",getpid(),getppid(), global_variable,& global_variable);
      sleep(1);
      cnt++;
      if(cnt==10)
      {
        global_variable=300;
        printf("child process changed global_variable\n");
      }
    }
  }
  else
  {
    while(1)
    {
      
      printf("I am parent , pid: %d,ppid: %d | global_variable: %d, &global_variabl: %p\n",getpid(),getppid(), global_variable,& global_variable);
      sleep(2);
    }
  }
  sleep(1);
  return 0;
}
