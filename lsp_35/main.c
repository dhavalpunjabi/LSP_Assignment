/*===================================================================================*/
/*************************************************************************************/
/**
 * @file      lsp_34.c
 * @brief     Client connecting to server.
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
#include <sys/types.h>


/************************************************************************************
*                          LOCAL MACROS                                             *
*************************************************************************************/
#define BSIZE 500

/*************************************************************************************
*                          LOCAL TYPEDEFS                                           *
*************************************************************************************/

/************************************************************************************
*                          LOCAL PROTOTYPES                                         *
*************************************************************************************/

/*************************************************************************************
 *                          GLOBAL VARIABLES                                         *
 *************************************************************************************/

 /*************************************************************************************
 *                          LOCAL VARIABLES                                          *
 *************************************************************************************/

 /*************************************************************************************
 *                          PRIVATE FUNCTIONS                                        *
 *************************************************************************************/

/***********************************************/
/*************************************************************************************
*                          PUBLIC FUNCTIONS                                          *
*************************************************************************************/


/************************************************************************************/
int main(int argc, char *argv[]) {
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int sfd, s, j;
  ssize_t len;
  ssize_t nRead;
  char buff[BUFSIZ];

  if(argc < 3) {
    fprintf(stderr,"Usage : %s host port msg...\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  if ( (s = getaddrinfo(argv[1], argv[2], &hints, &result)) != 0) {
    fprintf(stderr,"%s : getaddrinfo \n",gai_strerror(s));
    exit(EXIT_FAILURE);
  }
  /*The getaddrinfo() return a list of address structures */
  /* Try each address until we get a successful connection */
  /*IF socket(2) or connect(2) fails, then we close the socket*/
  for (rp = result; NULL != rp; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (-1 == sfd) {
      continue;
    }
    if ( (connect(sfd, rp->ai_addr, rp->ai_addrlen)) == -1) {
      break;
      close(sfd);
    }
  }
  if (NULL == rp) {
    fprintf(stderr, "Could not connect");
    exit(EXIT_FAILURE);
  }
  freeaddrinfo(result);
  /* Send all other remaining arguments to the server ,
        then read and display the response */
  for (j = 3; j < argc; j++) {
    len = strlen(argv[j]) + 1;
    if (len + 1 > BUFSIZ) {
      fprintf(stderr, "ignoring message argument %d\n", j);
      continue;
    }
    if( (write(sfd,argv[j],len)) != len) {
      fprintf(stderr,"partial failed\n");
      exit(EXIT_FAILURE);
    }
    nRead = read(sfd, buff, BUFSIZ);
    if (-1 == nRead) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    printf("received %ld bytes : %s\n", (long)nRead, buff);
  }
return 0;
}
