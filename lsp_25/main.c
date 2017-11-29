#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc , char *argv[]) {

  if (argc == 3) {
    execlp(argv[1], argv[1], argv[2], NULL);
  }
  if (argc == 4) {
    execlp("|", "|", NULL);
  }
  if (argc == 5) {
    execlp(argv[4], argv[4], argv[5], NULL) ;
  }
  return EXIT_SUCCESS;
}
