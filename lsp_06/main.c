#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>
#include <>

int main(int argc, char *argv[])
{
  int fd_in,fd_out;
  size_t size;
  void *ptr;
  fd_in = open("foo.txt",O_RDONLY);
  if (fd_in < 0) {
    perror("foo.txt");
    printf("error via %d\n",errno);
    exit(1);
  }
  fd_out = open("bar.txt",O_RDWR|O_CREAT,0644);
  if (fd_out < 0) {
    perror("bar.txt");
    printf("error via %d\n",errno);
    exit(1);
  }
  size = lseek(fd_in,0,SEEK_END);
  ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd_in ,0);
  if (ptr == MAP_FAILED) {
    perror("mmap fails");
    printf("error via %d\n",errno);
    exit(1);
  }
  write(fd_out,ptr+,30);
  close(fd_in);
  close(fd_out);
  return 0;
}
