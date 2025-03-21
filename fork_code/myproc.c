#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
//  pid_t id =fork();
//  if(id==0)
//  {
//    //child_pro
//    while(1)
//    {
//      printf("child,pid: %d,ppid: %d,id: %d,&id: %p\n",getpid(),getppid(),id,&id);
//      sleep(1);
//    }
//  }
//    else if(id>0)
//    {
//      //parent_pro
//      while(1)
//      {
//        printf("parent,pid: %d,ppid: %d,id: %d,&id: %p\n",getpid(),getppid(),id,&id);
//        sleep(2);
//      }
//    }
  
  pid_t id=fork();
  if(id==0)
  {
    //child
    while(1)
    {
      printf("I am child process, pid: %d,ppid: %d\n",getpid(),getppid());
      sleep(1);
      int *p=NULL;
      *p =100;
    }
  }
  else if(id>0)
  {
    //parent
    while(1)
    {
      printf("I am parent process, pid: %d,ppid: %d\n",getpid(),getppid());
      sleep(1);
    }
  }
  else
  {
    printf("fork error.\n");
  }

  return 0;
}
