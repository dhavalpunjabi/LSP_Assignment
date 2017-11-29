#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
int main(void) {
  int fd;
  char buff[30]="Hello Dhaval !!";

  fd = open("hello.txt",O_RDWR|O_CREAT,0644);       // open and create the file
  if (fd < 0) {                                     // error condition check
    printf("the error number is %d\n",errno);     // print error number
    perror("file not created");
  }
  write(fd,buff,strlen(buff));                    // writing into file
  close(fd);                                      // closing the file
  return 0;
}
