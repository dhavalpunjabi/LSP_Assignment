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
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

/*************************************************************************************
*                          LOCAL MACROS                                              *
*************************************************************************************/

/*************************************************************************************
*                          LOCAL TYPEDEFS                                            *
*************************************************************************************/

/*************************************************************************************
*                          LOCAL PROTOTYPES                                          *
*************************************************************************************/
void process_file(char *name);
void process_dir(char *dirname);

/*************************************************************************************
*                          GLOBAL VARIABLES                                          *
*************************************************************************************/
int modtimehist[24];
int rFlag, aFlag;

/*************************************************************************************
*                          LOCAL VARIABLES                                           *
*************************************************************************************/

/*************************************************************************************
*                          PRIVATE FUNCTIONS                                         *
*************************************************************************************/
void process_file(char *name) {
  struct stat sb;
  struct tm* bdtime;

  printf("processing file : %s\n",name);
  if (stat(name, &sb) == -1) {
    perror(name);
    exit(EXIT_FAILURE);
  }
  bdtime = localtime(&sb.st_mtime);
  modtimehist[bdtime->tm_hour]++;
}
/**********************************************************/
void process_dir(char *dirname) {
  DIR *d;
  struct dirent *info;
  char fullFileName[PATH_MAX];
  struct stat sb;

  printf("processing directory %s\n",dirname);
  if ( (d = opendir(dirname)) == NULL) {
    perror(dirname);
    exit(EXIT_FAILURE);
  }
  while ( (info = readdir(d)) != NULL) {
    if (info->d_name[0] == '.' && !aFlag) {
      continue;
    }
    strcpy(fullFileName, dirname);
    strcpy(fullFileName, "/");
    strcat(fullFileName, info->d_name);
    stat(fullFileName, &sb);
    if (strcmp(info->d_name, ".") && strcmp(info->d_name, "..") && rFlag && S_ISDIR(sb.st_mode)) {
      process_dir(fullFileName);
    }
    else {
      process_file(fullFileName);
    }
    closedir(d);
  }
}
/*************************************************************************************
*                          PUBLIC FUNCTIONS                                          *
*************************************************************************************/

/************************************************************************************/
int main (int argc, char *argv[]) {
  int i,c;
  char dirName[PATH_MAX];

  for(i = 0; i < 24; i++) {
    modtimehist[i] = 0;
  }
  opterr = 0;
  while ( (c = getopt(argc, argv, "ar")) != EOF) {
    switch (c) {
    case 'a':
      aFlag = 1;
      break;
    case 'r':
      rFlag = 1;
      break;
    case '?':
      fprintf(stderr, "invalid option -%c",optopt);
    }
  }
  argv += optind;
  argc -= optind;
  if (argc != 1) {
    fprintf(stderr, "usage : modtimehist [-a] [-r] dirname \n");
    exit(EXIT_FAILURE);
  }
  if (argv[0][0] == '/') {
    strncpy(dirName, argv[0], PATH_MAX);
  }
  else {
    getcwd(dirName, PATH_MAX);
    strcat(dirName, "/");
    strcat(dirName, argv[0]);
  }
  process_dir(dirName);
  for ( i = 0; i < 24; i++) {
    printf("hour : %2d : count = %6d\n", i, modtimehist[i]);
  }
}
