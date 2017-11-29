/*===================================================================================*/
/*************************************************************************************/
/**
 * @file      lsp_12.c
 * @brief     Monitor first 100 files mentioned in monitor.conf file.
 * @details   A monitor files by
 *            inotify system call.
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
#include <sys/types.h>
#include <string.h>
#include <sys/inotify.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>

/*************************************************************************************
*                          LOCAL MACROS                                              *
*************************************************************************************/
#define BUFFSIZE (100 * sizeof(struct inotify_event) + NAME_MAX + 1)

/*************************************************************************************
*                          LOCAL TYPEDEFS                                            *
*************************************************************************************/

/*************************************************************************************
*                          LOCAL PROTOTYPES                                          *
*************************************************************************************/


/*************************************************************************************
*                          GLOBAL VARIABLES                                          *
*************************************************************************************/

/*************************************************************************************
*                          LOCAL VARIABLES                                           *
*************************************************************************************/

/*************************************************************************************
*                          PRIVATE FUNCTIONS                                         *
*************************************************************************************/

/*******************************************/
int main(void) {
  FILE *fconfig;
  FILE *fout;
  char watchname[PATH_MAX];
  struct stat sb;
  int notifyfd, watchfd;
  char eventBuff[BUFFSIZE];
  int n;
  char *p;
  struct inotify_event *event;
  char watchednames[100][NAME_MAX + 1];

  notifyfd = inotify_init();
  if ( (fconfig = fopen("monitor.conf", "r")) == NULL ) {
    perror("unable to open monitor.conf");
    exit(EXIT_FAILURE);
  }
  while (fgets (watchname, PATH_MAX, fconfig) ) {
    watchname[strlen(watchname) - 1] = '\0';
    if (stat(watchname, &sb) == -1) {
      perror("not a file\n");
      continue;
    }
    if (S_ISREG(sb.st_mode)) {
      if ( (watchfd = inotify_add_watch(notifyfd, watchname, IN_MODIFY | IN_DELETE_SELF)) == -1) {
        perror("error watching ,watchname");
      }
      else {
        printf("added %s to watch list for and descriptor is %d\n", watchname, watchfd);
      }
    }
    else {
      printf("%s not a regular files and ignored\n", watchname);
    }
  }
  fout = fopen("monitor.conf", "a");
  while (1) {
    n = read(notifyfd, eventBuff, BUFFSIZE);
    for (p = eventBuff; p < eventBuff+n; ) {
      event = (struct inotify_event *)p;
      p += sizeof(struct inotify_event) + event->len;
      if (event->mask & IN_MODIFY) {
        fprintf(fout, "%s modified\n", watchednames[event->wd]);
      }
      if (event->mask & IN_DELETE_SELF) {
        fprintf(fout, "%s deleted\n", watchednames[event->wd]);
      }
      fflush(fout);
    }
  }
  return 0;
}


