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
#include <unistd.h>

/*************************************************************************************/
int main(void) {
  static char *cmd[] = {"ps", "ls", "date", "goof"};
  int choice;

  if ( 0 != printf("0. ps\n 1. ls\n 2. date\n 3.goof\n") ) {
    if ( 0 != scanf("%d", &choice) ) {
        /* IF execlp returns any value,this means it has failed */
      if ( execlp (cmd[choice], cmd[choice], NULL) ) {
          printf("not a valid executable binary\n");
          exit(EXIT_FAILURE);
      } else {
          exit(EXIT_SUCCESS);
        }
    }
    else {
      exit(EXIT_FAILURE);
    }
  }
  else {
    exit(EXIT_FAILURE);
  }
  return 0;
}
