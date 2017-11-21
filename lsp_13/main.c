#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int i,j;

  i = argc;
  j = 1;
  while (i - 1) {
    write (1, argv[j], strlen(argv[j]) );
    ++j;
    i--;
  }
  return 0;
}
