/******************************************************
*                   HEADERS                           *
*******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/times.h>
#include <sys/fcntl.h>
#include <time.h>
/******************************************************
*                   MAIN                              *
*******************************************************/

int main()
{
  struct tms tBuff;
  long tempVar1;
  double tempVar2;

  /*To give the user space time for the process*/
  for (tempVar1 = 0; tempVar1 < 10000000; tempVar1++) {
    tempVar2 = sqrt( (double) tempVar1);
  }
  printf("the val is %f\n",tempVar2);
  times(&tBuff);
  printf("user time : %ld\n",tBuff.tms_utime);
  printf("sys time : %ld\n",tBuff.tms_stime);
  printf("user child time : %ld\n",tBuff.tms_cutime);
  printf("user system time : %ld\n",tBuff.tms_cstime);

  return 0;
}
