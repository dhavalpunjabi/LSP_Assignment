/*===================================================================================*/
/*************************************************************************************/
/**
 * @file      lsp_28.c
 * @brief     Registering a signal by sigaction.
 * @details   Read block on pipe due to signal handler.
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
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
/************************************************************************************
*                          LOCAL MACROS                                             *
*************************************************************************************/


/*************************************************************************************
*                          LOCAL TYPEDEFS                                           *
*************************************************************************************/

/************************************************************************************
*                          LOCAL PROTOTYPES                                         *
*************************************************************************************/
static void signal_handler(int sigType);

/*************************************************************************************
 *                          GLOBAL VARIABLES                                         *
 *************************************************************************************/

 /*************************************************************************************
 *                          LOCAL VARIABLES                                          *
 *************************************************************************************/

 /*************************************************************************************
 *                          PRIVATE FUNCTIONS                                        *
 *************************************************************************************/
/********************************************//**
 Name        : signal_handler
 Input(s)    : 1. Signal Number
 Output(s)   : None.
 Description : Handles signal SIGINT.
***********************************************/
static void signal_handler(int sigType) {
  printf("Signal number : %d\n",sigType);
}
/*************************************************************************************
*                          PUBLIC FUNCTIONS                                         *
*************************************************************************************/

int main() {
struct sigaction action;
int num,pip[2];
char buff[1000];

if (pipe(pip)) {
  perror("pipe");
  exit(EXIT_FAILURE);
}
/* Defining attributes of sigaction structure */
action.sa_handler = signal_handler;
sigemptyset(&action.sa_mask);
action.sa_flags = 0;
sigaction(SIGINT, &action, NULL);
/* Continuous loop */
while (1) {
  num = read(pip[0],buff,1000);
  printf("read returned %d and error is %d\n", num, errno);
  }
  return 0;
}
