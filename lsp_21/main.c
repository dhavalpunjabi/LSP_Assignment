/*===================================================================================*/
/*************************************************************************************/
/**
 * @file      lsp_22.c
 * @brief     Common execl functions for general purpose use
 * @details   A collection of list of operations to be performed
 *            selected by user input
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
#include <sys/wait.h>
#include <unistd.h>

/*************************************************************************************/

int main(void) {
  int status;

  if (fork()) {
    printf("waiting for child...\n");
    wait (&status);
    if ( WIFEXITED (status)) {
      printf("child exited normally by status...%d\n", WEXITSTATUS(status));
    }
    if ( WIFSIGNALED (status)) {
      printf("child terminated by signal.. %d\n", WTERMSIG(status));
    }
  }
  else {
    printf("child pid is %d\n", getpid());
    sleep(15);
    exit(EXIT_SUCCESS);
  }
  return 0;
}
