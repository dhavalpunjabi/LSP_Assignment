#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#define BUFFSIZE 30
int main() {
  int fd1,fd2;
  char buff[BUFFSIZE];

  fd1 = open("foo.txt",O_RDONLY);                       // open foo.txt
  if (fd1 < 0) {                                        // check for error
    perror("foo.txt");
    printf("Unable to open with error %d\n",errno);
    }
  fd2=open("bar.txt",O_RDWR|O_CREAT,0664);            // open bar.txt
  if (fd2<0) {
    perror("bar.txt");
    printf("Unable to open with error %d\n",errno);
    }
  while (read(fd1,buff,30)) {                         // read from foo.txt
    write(fd2,buff,strlen(buff));                     // write into bar.txt
    }
  close(fd1);
  close(fd2);
}
