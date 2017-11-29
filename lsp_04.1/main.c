#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc , char *argv[])
{
  int fd,size,count,i;
  char *buff;

  if (3 != argc) {
    printf("enter 2 arguments\n");
    exit(1);
  }
  fd = open("foo",O_WRONLY|O_CREAT|O_TRUNC,06000);
  if (fd < 0) {
    perror("foo");
    exit(1);
  }
  size  = atoi(argv[1]);
  count = atoi(argv[2]);
  buff  = malloc(size);
  for (i=0;i<count;i++) {
    write(fd,buff,size);
  }
  close(fd);
  return 0;
}
