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
static void timeout_handler(int unUsedVar);
static int t_getnum(int timeout);

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
 Name        : timeout_handler
 Input(s)    : 1. int unUsedVar
 Output(s)   : None.
 Description : Does not perform action at all.
***********************************************/
static void timeout_handler(int unUsedVar) {
}
/*************************************************************************************/
/********************************************//**
 Name        : t_getnum
 Input(s)    : 1. int timeout
 Output(s)   : return an integer value
 Description : Creates alarm.
***********************************************/
static int t_getnum(int timeout) {
  int num;
  char line_buff[100];
  struct sigaction action;

  action.sa_handler = timeout_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  sigaction(SIGALRM, &action, NULL);

  alarm(timeout);
  num = read(0, line_buff, 100);
  alarm(0);
  if (num == -1 && errno == EINTR ) {
    return -1;
  }
  num = atoi(line_buff);
  return num;
}

/*************************************************************************************
*                          PUBLIC FUNCTIONS                                          *
*************************************************************************************/


/************************************************************************************/
int main() {
  int number;

  while (1) {
    printf("enter a number\n");
    if ((number=t_getnum(5) )== -1) {
      printf("timed out\n");
    }
    else {
      printf("you have entered %d\n",number);
    }
  }
  return 0;
}
