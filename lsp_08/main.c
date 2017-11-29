#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
int main(){
  struct stat fstatus;
  int fd;

  if ((fd = open("foo",O_RDONLY)) < 0) {
    perror("foo");
    printf("error via %d\n",errno);
    exit(EXIT_FAILURE);
  }
  if (0 == fstat(fd,&fstatus)) {
     printf("last access time is %s\n",ctime(&fstatus.st_atim));
     printf("last change time is %s\n",ctime(&fstatus.st_ctim));
     printf("last modified time is %s\n",ctime(&fstatus.st_mtim));
  }
  else {
    exit(EXIT_FAILURE);
  }
  close(fd);

  return 0;
}
