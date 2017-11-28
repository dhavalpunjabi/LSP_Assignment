/*===================================================================================*/
/*************************************************************************************/
/**
 * @file      pthtread.c
 * @brief     Simple thread creation via main thread.
 * @details   A creation of thread and joining it to
 *            main thread.
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
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

/*************************************************************************************
*                          LOCAL MACROS                                              *
*************************************************************************************/

/*************************************************************************************
*                          LOCAL TYPEDEFS                                            *
*************************************************************************************/

/*************************************************************************************
*                          LOCAL PROTOTYPES                                          *
*************************************************************************************/
static void *thread_action(void *argv);
/*************************************************************************************
*                          GLOBAL VARIABLES                                          *
*************************************************************************************/

/*************************************************************************************
*                          LOCAL VARIABLES                                           *
*************************************************************************************/

/*************************************************************************************
*                          PRIVATE FUNCTIONS                                         *
*************************************************************************************/
/********************************************//**
Name        : pthread.c
Input(s)    : No input(s).
Output(s)   : No output(s).
Description : A thread function , executed when thread is created by pthread_create().
              The function is exited when pthread_exit() is called in this function.
***********************************************/
static void *thread_action(void * argv) {
  printf("This is thread function\n");
  printf("its pid is %d\n",getpid());
  pthread_exit(NULL);
}
/*************************************************************************************
*                          PUBLIC FUNCTIONS                                          *
*************************************************************************************/

/************************************************************************************/
int main(void) {
  int exitStatus, res;
  pthread_t thread;
  printf("Creating a thread...\n");
  res = pthread_create(&thread, NULL, thread_action, "hello world");
  if (-1 == res) {
    perror("thread creation");
    exit(EXIT_FAILURE);
  }
  pthread_join(thread,(void **)&exitStatus);
  printf("thread exited with status %d\n",exitStatus);
  printf("return to main thread\n");
  printf("main thread pid = %d\n",getpid());
  sleep(2);
  exit(EXIT_SUCCESS);
}
