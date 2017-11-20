#include <stdio.h>
#include <stdlib.h>
#define BUFF_SIZE 30
int main(){
  FILE *fdin,*fdout;
  char buff[BUFF_SIZE];

  fdin = fopen("foo","r");
  if (NULL == fdin) {
    printf("error in open read file\n");
  }
  fdout = fopen("bar","w");
  if (NULL == fdout) {
    printf("error in open write file\n");
  }
  while (fread(buff,1,BUFF_SIZE,fdin)) {
    fwrite(buff,1,BUFF_SIZE,fdout);
  }
  fclose(fdin);
  fclose(fdout);
  return 0;
}
