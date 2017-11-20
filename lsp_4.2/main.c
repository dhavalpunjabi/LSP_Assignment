#include <stdio.h>
#include <stdlib.h>

int main(int argc , char *argv[]){
  FILE *fd;
  char *buff;
  int size,count,i;

  if (3 != argc) {
    printf("no sufficient arguments\n");
    exit(1);
  }
  fd = fopen("foo","w");
  if (NULL == fd) {
    printf("cannot open file\n");
    exit(1);
  }
  size  = atoi(argv[1]);
  count = atoi(argv[2]);
  buff  = malloc(size);
  for (i=0;i<count;count++) {
    fwrite(buff,size,1,fd);
  }
  fclose(fd);
  return 0;
}
