/*===================================================================================*/
/*************************************************************************************/
/**
 * @file      lsp_39.c
 * @brief     Server program to handle multiple clients concurrently.
 * @details   Read at client and print the same.
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
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <time.h>

/************************************************************************************
*                          LOCAL MACROS                                             *
*************************************************************************************/
#define PORT_ADDRESS 8000
#define BUF_SIZE 1024

/*************************************************************************************
*                          LOCAL TYPEDEFS                                           *
*************************************************************************************/

/************************************************************************************
*                          LOCAL PROTOTYPES                                         *
*************************************************************************************/
int handle_connection(int input, int output) ;
void rot13(char *input, char *output, int length) ;

/*************************************************************************************
*                          GLOBAL VARIABLES                                         *
*************************************************************************************/

/*************************************************************************************
 *                          LOCAL VARIABLES                                          *
 *************************************************************************************/

/*************************************************************************************
*                          PRIVATE FUNCTIONS                                         *
*************************************************************************************/
/********************************************//**
Name        : handle_connection
Input(s)    : 1.input sock fd 2. output sockfd
Output(s)   : Returns 0 for success and -1 for failure
Description : Multiple or concurrent clients communicating with a
              server
***********************************************/
void handle_connection(int input , int output) {
  char inputBuffer[100] ;
  char outputBuffer[100] ;

  while (read(input , &inputBuffer , 1) > 0) {                   //Reading Byte by Byte
    rot13(inputBuffer , outputBuffer) ;
    if(*outputBuffer == 23 || *outputBuffer == 26 ) {
    continue ;
    }
    write(output , &outputBuffer , 1) ;       //Writing byte by byte
  }
}
/********************************************//**
Name        : rot13
Input(s)    : 1.input buffer 2. output buffer 3. length
Output(s)   : Non - return function
Description : The string characters are added with +13 value
              to each character so as to get a new string.
********************************************/
void rot13(char *input, char *output, int length) {
  int num;
  length = length - 2;

  while(length--) {
    num = (int) (*input + 13) ;
    if (num > 122) {
      num -= 26 ;
    }
    else if ( (num > 90) && (num < 97) ) {
      num -= 26 ;
    }
    *output = (char)num;
    input++;
    output++;
  }
  *output++ = *input++;      //addition of new line
  *output = *input;
}
/*************************************************************************************
*                          PUBLIC FUNCTIONS                                          *
*************************************************************************************/
int main(void) {
  int socketServer , fd;
  struct sockaddr_in server, client;

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(PORT_ADDRESS);

  if (socketServer = socket(AF_INET, SOCK_STREAM, 0) == -1) {
    perror("Error in creating socket\n");
    exit(1);
  }
  if (bind(socketServer, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("Error in binding\n");
    exit(1);
  }
  if (listen(socketServer, 5) == -1) {
    perror("Error in listening\n");
    exit(1);
  }
  printf("Listening to port %d\n", PORT_ADDRESS)
  signal(SIGCHLD , SIG_IGN);
  while (1) {
    if ( -1 == (fd = accept(socketServer , (struct sockaddr *)&client , (socklen_t *)&client)) ) {
      perror("Error in acceptance\n");
      exit(1) ;
    }
    if (fork() == 0) {
      close(socketServer) ;
      printf("Got a New Connection\n");
      handle_connection(fd, fd);
      close(fd);
      printf("Connection Terminated\n") ;
      return 0 ;
    }
  }
  return EXIT_SUCCESS ;
}
