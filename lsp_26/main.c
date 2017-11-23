/*===================================================================================*/
/*************************************************************************************/
/**
 * @file      lsp_26.c
 * @brief     Common execl functions to list all the signals.
 * @details   To list all the signals available to the kernel
 *
 * @see
 * @author    Dhaval Punjabi, dhavalpunjabi@elear.solutions
 * @copyright Copyright (c) 2017 Elear Solutions Tech Private Limited. All rights
 *            reserved.
 * @license   To any person (the "Recipient") obtaining a copy of this software and
 *            associated documentation files (the "Software"):\n
 *            All information contained in or disclosed by this software is
 *            confidential and proprietary information of Elear Solutions Tech
 *            Private Limited and all rights therein are expressly reserved.
 *            By accepting this material the recipient agrees that this material and
 *            the information contained therein is held in confidence and in trust
 *            and will NOT be used, copied, modified, merged, published, distributed,
 *            sublicensed, reproduced in whole or in part, nor its contents revealed
 *            in any manner to others without the express written permission of
 *            Elear Solutions Tech Private Limited.
 */
/*************************************************************************************/
/*===================================================================================*/
/*                                  HEADERS                                          */
/*************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*************************************************************************************/

int main() {
  int status;

  if ( !fork() ) {
    execl ("/bin/kill", "kill", "-l", NULL);
    exit (status);
  }
  else {
    wait(&status);
    printf("the chils exit status is %d", WEXITSTATUS(status));
  }
  return 0;
}
