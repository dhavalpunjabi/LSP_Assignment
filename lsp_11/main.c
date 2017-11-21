#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
int main(int argc , char *argv[]){
  DIR *d;
  struct dirent *info;
  struct stat file;
  long total_size=0;

  if (2 != argc) {
    perror("argv[1]");
    exit(EXIT_FAILURE);
  }
  d = opendir(argv[1]);
  while ((info = readdir(d)) != NULL) {
    stat(info->d_name,&file);
    total_size += file.st_size;
  }
  printf("total size is %ld\n",total_size);
  return 0;
}
