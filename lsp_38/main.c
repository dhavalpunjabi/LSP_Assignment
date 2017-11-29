/*===================================================================================*/
/*************************************************************************************/
/**
 * @file      lsp_38.c
 * @brief     Server program for UDP to broad caste.
 * @details   Server program for UDP broad caste and broad caste the updates.
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

/************************************************************************************
*                          LOCAL MACROS                                             *
*************************************************************************************/
#define BUF_SIZE 516
#define TFTP_OPCODE_RRQ    1
#define TFTP_OPCODE_WRQ    2
#define TFTP_OPCODE_DATA   3
#define TFTP_OPCODE_ACK    4
#define TFTP_OPCODE_ERROR  5
#define TFTP_TRANSFER_MODE "octet"

/*************************************************************************************
*                          LOCAL TYPEDEFS                                           *
*************************************************************************************/

/************************************************************************************
*                          LOCAL PROTOTYPES                                         *
*************************************************************************************/
void handle_tftp(int socketClient , struct sockaddr *server , int serverLength , char *fileName);

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
Name        : handle_tftp
Input(s)    : 1.int socketClient fd 2. structure address of server 3.int severLength 4. char pointer to filename
Output(s)   : No output(s).
Description : UDP broadcaste to broadcaste an update.
***********************************************/
void handle_tftp(int socketClient , struct sockaddr *server , int serverLength , char *fileName) {
  char buffer[BUF_SIZE];
  char *p = buffer;
  short int opcode;
  short int blockCount;
  struct sockaddr_in destServer;
  socklen_t destServerLength;
  int load, count;

  opcode = htons(TFTP_OPCODE_RRQ);
  memcpy(p, &opcode, sizeof(opcode));
  p = p + 2;
  p = strcpy(p, fileName);
  p = p + strlen(fileName) + 1;
  p = strcpy(p , TFTP_TRANSFER_MODE);
  p = p + strlen(TFTP_TRANSFER_MODE) + 1;
  load = p - buffer;

  if (sendto(socketClient, buffer, load, 0, server, serverLength) == -1) {
    perror("Sending Fault\n");
    exit(1);
  }
  do {
    destServerLength = sizeof(struct sockaddr_in);
    if ( (count = recvfrom(socketClient , buffer , BUF_SIZE , 0 , (struct sockaddr *)&destServer , &destServerLength)) == -1) { //Receiving Data from Destination Server
      perror("receiveing error\n");
      exit(1);
    }
    memcpy(&opcode , buffer , sizeof(short));
    memcpy(&blockCount , buffer + 2 , sizeof(short));
    opcode = ntohs(opcode);
    blockCount = ntohs(blockCount);
    switch (opcode) {
      case TFTP_OPCODE_ERROR :
        perror("ERROR in transmission of messages\n") ;
        exit(1) ;
      case TFTP_OPCODE_DATA :
        if (write(1 , buffer + 4, count-4 ) == -1) {
        perror("Writing error\n");
        exit(1) ;
        }
    }
  } while (count == BUF_SIZE);
  return ;
}
/*************************************************************************************
*                          PUBLIC FUNCTIONS                                          *
*************************************************************************************/

/************************************************************************************/
int main(void) {
  int socketClient, addInfo;
  struct addrinfo hints;
  struct addrinfo *results, *server, *rp;
  char *fileName = "bar.txt";

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = 0;
  if ( (addInfo = getaddrinfo("localhost", "tftp", &hints, &results)) == -1) {
    perror("Getting address\n") ;
    exit(1) ;
  }
  server = NULL;
  rp     = results;

  while (rp != NULL) {
    socketClient = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    if (-1 != socketClient) {
    server = rp;
    rp = NULL;
    }
    else {
    printf("Connection failed\n");
    rp = rp->ai_next;
    }
  }
  handle_tftp(socketClient, server->ai_addr, server->ai_addrlen, fileName) ;
  freeaddrinfo(results);
  return EXIT_SUCCESS;
}
