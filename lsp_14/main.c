#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(void) {
  char **p;                 // pointer to pointer for environment variable string

  for (p=environ; *p; p++) {
    printf("%s\n",*p);
  }
  return 0;
}
