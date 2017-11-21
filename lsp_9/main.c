#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
int main( int argc, char *argv[]){
  struct stat file;
  char *filetype[] ={"unknown","FIFO","character_device","unknown","directory",
                      "unknown","block_device","unknown","regular_file","unknown","symlink","unknown","socket"};
  if (2 != argc) {
    write(1,"no enough arguments",30);
    exit(EXIT_FAILURE);
  }
  stat("hello",&file);
  printf("the filetype is %s\n",filetype[(file.st_mode >> 12) & 0x17]);
  printf("permissions : %c%c%c%c%c%c%c%c%c\n",
         file.st_mode & S_IRUSR ? 'r' : '-',
         file.st_mode & S_IWUSR ? 'w' : '-',
         file.st_mode & S_IXUSR ? 'x' : '-',
         file.st_mode & S_IRGRP ? 'r' : '-',
         file.st_mode & S_IWGRP ? 'w' : '-',
         file.st_mode & S_IXGRP ? 'x' : '-',
         file.st_mode & S_IROTH ? 'r' : '-',
         file.st_mode & S_IWOTH ? 'w' : '-',
         file.st_mode & S_IXOTH ? 'x' : '-'
         );
  printf("acess time %s\n",ctime(&file.st_atim));
  printf("change time %s\n",ctime(&file.st_ctim));
  printf("last modified time %s\n",ctime(&file.st_mtim));
  return 0;
}
