/*===================================================================================*/
/*************************************************************************************/
/**
 * @file      lsp_32.c
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
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

/************************************************************************************
*                          LOCAL MACROS                                             *
*************************************************************************************/
#define SERVER_PORT 8000
#define C_BUFF *(buff+i)

/*************************************************************************************
*                          LOCAL TYPEDEFS                                           *
*************************************************************************************/

/************************************************************************************
*                          LOCAL PROTOTYPES                                         *
*************************************************************************************/
static void write_read(unsigned int *fd, unsigned int sock, unsigned char *buff);
static void rot_13(unsigned int *fd, unsigned char *buff, unsigned int count);

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
 Name        : write_read
 Input(s)    : 1. int fd 2. char *buff
 Output(s)   : void
 Description : Reads and write into socket
***********************************************/
static void write_read(unsigned int *fd, unsigned int sock, unsigned char *buff) {
  unsigned int count;
  while( (count = read(*fd, buff, 1024 )) > 0 ) {
    rot_13(fd, buff,count);
  }
}
/********************************************//**
 Name        : rot_13
 Input(s)    : 1. int fd 2. char *buff
 Output(s)   : void
 Description : Reads and write into socket
***********************************************/
static void rot_13(unsigned int *fd, unsigned char *buff, unsigned int count) {
  int i = 0;
  while (C_BUFF != '\0') {
    C_BUFF += 13;
    if ( (C_BUFF >=92 && C_BUFF <=96) || C_BUFF > 136 ) {
      C_BUFF -=26;
    }
    i++;
  }
  buff[count-1] = '\0';
  write(*fd, buff, count-1);
}
/*************************************************************************************
*                          PUBLIC FUNCTIONS                                          *
*************************************************************************************/


/************************************************************************************/
int main(void) {
  unsigned int sock, fd, client_len;
  unsigned char buffer[1024];
  struct sockaddr_in server,client;

  if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(SERVER_PORT);
  if (bind(sock, (struct sockaddr*)&server, sizeof(server)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  if (listen(sock,5) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  printf("listening...\n");

  while(1) {
    client_len = sizeof(client);
    if ( ( fd = accept(sock, (struct sockaddr*)&client, &client_len) ) == -1) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
    printf("got connection...\n");
    write_read(&fd, sock, buffer);
    close(fd);
  }
return 0;
}
