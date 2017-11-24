/*===================================================================================*/
/*************************************************************************************/
/**
 * @file      lsp_27.c
 * @brief     Basic signal handler
 * @details   Handling signal SIGINT and
 *            ignoring signals SIGTERM and SIGQUIT
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
#include <signal.h>
#include <sys/wait.h>

/*************************************************************************************/

/*************************************************************************************
 *                          GLOBAL VARIABLES                                         *
 *************************************************************************************/
int number;

/*************************************************************************************
 *                          LOCAL PROTOTYPES                                         *
 *************************************************************************************/
void signal_handler(int sigNum);

/*************************************************************************************
*                          PRIVATE FUNCTIONS                                        *
*************************************************************************************/
/********************************************//**
 Name        : signal_handler
 Input(s)    : 1. Signal Number
 Output(s)   : None.
 Description : Resets the global variable number
***********************************************/
void signal_handler(int sigNum) {
  printf("signal occurred is %d\n",sigNum);
  number = 0; /*Handling a signal and again reseting the variable number*/
}

int main()
{
  signal(SIGINT, signal_handler);
  signal(SIGQUIT, SIG_IGN);
  signal(SIGTERM , SIG_IGN);

  while (1) {
    printf("pid = %d and number = %d\n", getpid(), number++);
    sleep(1);
  }
  return 0;
}
