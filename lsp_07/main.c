#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define FLAG_READWRITE O_RDWR | O_CREAT | O_TRUNC
#define PERMISSION S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
#define Employee_no 1

typedef struct member {
  int id;
  char name[80];
} member;

int main(void) {
  int fileReadWrite;
  member Employee[3] = {{1, "Dhaval"}, {2, "Utkarsh"}, {3, "Kapeel"}};
  off_t size ;
  ssize_t fileWrite ;
  fileReadWrite = open("fileSource.txt", FLAG_READWRITE, PERMISSION) ;

  if(fileReadWrite == -1) {
    perror("ERROR!!!!!\n");
    exit(1);
  }
  size = lseek(fileReadWrite, 0, SEEK_END) ;
  if(size == -1) {
    perror("ERROR!!!!\n");
    exit(1);
  }
  char *textMemory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fileReadWrite, 0) ;
  textMemory = "ELEAR";
  if (strlen(Employee[Employee_no].name) > strlen(textMemory)) {
  memcpy(&Employee[Employee_no].name, textMemory, strlen(Employee[Employee_no].name));
  }
  else {
  memcpy(&Employee[Employee_no].name, textMemory, strlen(textMemory));
  }
  fileWrite = write(fileReadWrite, &Employee[0], sizeof(Employee[0]));
  fileWrite = write(fileReadWrite, &Employee[1], sizeof(Employee[1]));
  fileWrite = write(fileReadWrite, &Employee[2], sizeof(Employee[2]));
  if (fileWrite == -1) {
    perror("ERROR!!!!\n") ;
    exit(1);
  }
  return EXIT_SUCCESS;
}
