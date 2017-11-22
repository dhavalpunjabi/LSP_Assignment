#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  pid_t retVal;
  sleep(8);
    retVal = fork();
    switch (retVal) {
      case 0 :
        printf("the child pid is %d\n", getpid() );
        printf("the child ppid is %d\n", getppid() );
	sleep(5);
        exit(EXIT_SUCCESS);
        break;
      case -1 :
        printf("error with fork\n");
        exit(EXIT_FAILURE);
      default :
        printf("the parent pid is %d\n",getpid());
        printf("the parent ppid id is %d\n", getppid() );
	sleep(5);
        exit(EXIT_SUCCESS);
    }
  return 0;
}
