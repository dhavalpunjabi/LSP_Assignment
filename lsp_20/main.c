#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  pid_t retVal;
  int i;

  retVal = fork();
  switch (retVal) {
  case 0 :
    for (i = 0; i < 1000; i++) {
        printf("child pid is %d\n", getpid());
    }
    break;
  case -1 :
    printf("error in fork\n");
    exit(EXIT_FAILURE);
  default :
    for (i = 0; i < 1000; i++) {
    printf("Parent pid is %d\n", getpid());
    }
  return 0;
  }
}
