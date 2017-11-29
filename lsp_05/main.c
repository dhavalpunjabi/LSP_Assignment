#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

struct record {
  int id;
  char name[20];
};
int main(){
  int fd,size = sizeof(struct record);
  fd = open("records",O_RDWR|O_CREAT,0664);
  if (fd < 0) {
    perror("records");
    printf("unable to open via error %d\n",errno);
  }
  struct record info;
  info.id=1;
  strcpy(info.name,"Dhaval");
  write(fd,&info,size);
  info.id=3;
  strcpy(info.name,"Kapil");
  write(fd,&info,size);
  lseek(fd,-size,SEEK_CUR);
  info.id=2;
  strcpy(info.name,"Utkarsh");
  write(fd,&info,size);
  close(fd);
  return 0;
}
