#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>

int main( int argc, char *argv[]){
  struct stat file;
  struct passwd *pwd;
  struct group  *grp;

  pwd = getpwuid ( getuid());
  char *filetype[] ={"-","p","c","-","d",
                      "-","b","-","r","-","l","-","s"};
  if (2 != argc) {
    write(1,"no enough arguments",30);
    exit(EXIT_FAILURE);
  }
  stat("hello",&file);
  grp = getgrgid(file.st_gid);
  printf("%s",filetype[(file.st_mode >> 12) & 0x17]);
  printf("%c%c%c%c%c%c%c%c%c  ",
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
  printf("%d ",argc-1);
  printf("%s ", pwd->pw_name);
  printf("%s ", grp->gr_name);
  printf("%s\n",ctime(&file.st_atime));

  return 0;
}
