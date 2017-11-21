#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
static void view_list(char *);

int main(int argc , char *argv[]){
  DIR *d;
  struct dirent *info;

  if (2 != argc) {
    perror("not enough arguments\n");
    exit(EXIT_FAILURE);
  }
  if (NULL !=(d=opendir(argv[1])) ) {
    while ((info = readdir(d)) != NULL) {
      view_list(info->d_name);
    }
  }
  return 0;
}

static void view_list (char *dir_name) {
  struct stat file;
  char *filetype[] ={"unknown","FIFO","character_device","unknown","directory",
                      "unknown","block_device","unknown","regular_file","unknown","symlink","unknown","socket"};
  if (stat(dir_name,&file) < 0) {
    perror("fatal directory");
    exit(EXIT_FAILURE);
  }
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
  printf("acess time %s\n",ctime(&file.st_atime));
  printf("change time %s\n",ctime(&file.st_ctime));
  printf("last modified time %s\n",ctime(&file.st_mtime));
}
