/*************************************************
*              HEADERS                           *
**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <sys/types.h>
#include <time.h>

/*************************************************
*               MAIN                                  *
**************************************************/
int main() {
  time_t t;
  struct tm *bdtime;
  char buff[1000];

  setlocale(LC_ALL,"");
  t = time(NULL);
  bdtime = localtime(&t);
  strftime(buff,1000,"It is %H %M on %A %d %B %Y",bdtime);
  puts(buff);
  return 0;
}
