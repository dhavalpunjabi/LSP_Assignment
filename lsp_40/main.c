/*===================================================================================*/
/*************************************************************************************/
/**
 * @file      lsp_40.c
 * @brief     Server and Localhost connection.
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
#include <sys/select.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

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
int handle_connection(int input, int output) {
  char inputBuffer[BUF_SIZE];
  char outputBuffer[BUF_SIZE];
  int readSize;

  if( (readSize=read(input, inputBuffer, BUF_SIZE)) < 1) {
    return -1;
  }
  rot13(inputBuffer, outputBuffer, readSize);

  if(write(output, outputBuffer, readSize) < 1) {
    return -1;
  }
  return 0;
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
  int socServer, socClient, count;
  struct sockaddr_in server, client;
  fd_set activeClient, copyClient;
  socklen_t clientLength;

  FD_ZERO(&activeClient);
  FD_ZERO(&copyClient);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(PORT_ADDRESS);

  if ( (socServer = socket(AF_INET, SOCK_STREAM, 0) ) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  if (bind(socServer, (struct sockaddr*)&server, sizeof(server)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  if (listen(socServer, 5) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  FD_SET(socServer, &copyClient);
  while(1) {
    memcpy(&copyClient, &activeClient, sizeof(activeClient));
    select(FD_SETSIZE, &copyClient, NULL, NULL, NULL);
    if (FD_ISSET(socServer, &copyClient)) {
      clientLength = sizeof(client);
      if ( (socClient = accept(socServer, (struct sockaddr*)&client, &clientLength) ) == -1) {
        perror("error in acception");
        exit(EXIT_FAILURE);
      }
      FD_SET(socClient, &activeClient);
      printf("now got a connection\n");
    }
    for (count = 3; count < FD_SETSIZE ; count++) {
      if (count != socServer) {
        if (FD_ISSET(count , &copyClient)) {    //checking whether value matches clientCopy
          if (handle_connection(count , count) == -1) {
            printf("Conection Terminated\n") ;
            close(count) ;
            FD_CLR(count , &copyClient) ;
          }
        }
      }
    }
  }
return 0;
}
