/*===================================================================================*/
/*************************************************************************************/
/**
 * @file
 * @brief
 * @details
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
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "../../sqlite/ext/async/sqlite3async.h"
#include <time.h>

/*************************************************************************************
 *                          LOCAL MACROS                                              *
 *************************************************************************************/
 #define BUF_SIZE                                           1500000
 #define BILLION                                            1000000000L
 #define MILLION                                            1000000

 /*************************************************************************************
 *                          LOCAL TYPEDEFS                                            *
 *************************************************************************************/

/*************************************************************************************
 *                          LOCAL PROTOTYPES                                          *
 *************************************************************************************/
static void async_process_init(void);
static sqlite3* create_and_init_and_config_db(sqlite3 *db);
static char * query_create_table(char *sql, sqlite3 *db);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
static void *thread_action_read(void *db);
static void *thread_action_write(void *db);
static int busy_timeout_set(sqlite3 *db, int ms);
static int busy_handler_wrapper(sqlite3 *db);
static int busy_callback(void *, int count);

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
Name        :  static void async_process_init
Input(s)    :
Output(s)   :
Description :
***********************************************/
static void async_process_init(void) {
  sqlite3async_initialize("unix", 1);
  printf("Asynchronous process initialized\n");
  return;
}

/********************************************//**
Name        :  static sqlite3 *create_and_init_and_config_db
Input(s)    :
Output(s)   :
Description :
***********************************************/
static sqlite3 *create_and_init_and_config_db(sqlite3 *db) {
  int rc;

  rc = sqlite3_open_v2("Elear", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, NULL);

  if (rc == SQLITE_OK) {
    printf("Created DB successfully\n");
    return db;
  } else {
      printf("Failed to open or create database\n");
      return NULL;
  }
}

/********************************************//**
Name        :  static char * query_create_table
Input(s)    :
Output(s)   :
Description :
***********************************************/
static char * query_create_table(char *sql, sqlite3 *db) {
  int fd, rd;
  char buff[BUF_SIZE] = {'\0'};

  if ( (fd = open("string.txt", O_RDONLY)) < 1) {
    printf("Error opening file\n");
    return NULL;
  }
  if (0 == (rd = read(fd, buff, 1024) ) ) {
    printf("Error in reading file\n");
    return NULL;
  }
  printf("query of bytes : %d", rd);
  if (NULL == (sql = malloc(strlen(buff)) ) ) {
    printf("Unable to allocate memory\n");
    pthread_exit(NULL);
  }
  memcpy(sql, buff, strlen(buff));
  return sql;
}

/********************************************//**
Name        :  static int callback
Input(s)    :
Output(s)   :
Description :
***********************************************/
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for(i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

/********************************************//**
Name        :  static void *thread_action_write
Input(s)    :
Output(s)   :
Description :
***********************************************/
static void *thread_action_write(void *db){
  sqlite3async_run();
  printf("thread_1 exit\n");
  return NULL;
}

/********************************************//**
Name        :  static void *thread_action_read
Input(s)    :
Output(s)   :
Description :
***********************************************/
static void *thread_action_read(void *db) {
  int i, rc;
  static int j = 1;
  const char *sql = "update employee set id = ";
  char *sqlQuery = calloc(strlen(sql) + 30, sizeof(char) );
  char *zErrMsg;

  printf("In read thread with threadId = %lu\n", pthread_self());
  for (i = 0; i < 10000; i++, j++) {
    sprintf(sqlQuery, "%s %d where id = %d", sql, j, j+1);
    rc = sqlite3_exec(db, sqlQuery, callback, NULL, &zErrMsg);
    if (SQLITE_OK == rc) {
      //printf("update successfull\n");
    }
    else {
      printf("update unsuccessfull %d due to %s !! ... \n", rc, zErrMsg);
      sqlite3_free(zErrMsg);
    }
  }
  free(sqlQuery);
  printf("exit read thread...\n");
  return NULL;
}

/********************************************//**
Name        :  static int busy_timeout_set
Input(s)    :
Output(s)   :
Description :
***********************************************/
static int busy_timeout_set(sqlite3 *db, int ms) {
  return sqlite3_busy_timeout(db, ms);
}

/********************************************//**
Name        :  static int busy_handler_wrapper
Input(s)    :
Output(s)   :
Description :
***********************************************/
static int busy_handler_wrapper(sqlite3 *db) {
  return sqlite3_busy_handler(db, busy_callback, NULL);
}

/********************************************//**
Name        :  static int busy_callback
Input(s)    :
Output(s)   :
Description :
***********************************************/
static int busy_callback(void *arg, int count) {
  return count;
}

/*************************************************************************************
*                          PUBLIC FUNCTIONS                                          *
*************************************************************************************/
/********************************/
int main (int argc, char *argv[]) {
  sqlite3 *db = NULL;
  char *sql = NULL;
  char *sqlQuery;
  char *zErrMsg;
  int rc , choice;
  struct timespec start, stop;
  sqlite3_vfs *vfs;
  pthread_t write_thread;
  pthread_t read_thread;
  double timeDiff;
  int fd, rd;
  char buff[BUF_SIZE] = {'\0'};
  int timeOut;
  int busyStatus;

  sqlite3_initialize();

  vfs = sqlite3_vfs_find("unix");
  sqlite3_vfs_register(vfs, 1);

  async_process_init();

  sqlite3async_control(SQLITEASYNC_GET_HALT, &rc);
  printf("halt status : %d\n", rc);

  rc = sqlite3async_control(SQLITEASYNC_LOCKFILES, 1);
  if (rc != SQLITE_OK) {
    printf("control of lock files failure\n");
    return -1;
  }
  else {
    printf("assigned control for lock files\n");
  }

  sqlite3async_control(SQLITEASYNC_GET_LOCKFILES, &rc);
  printf("lock file status : %d\n", rc);

  if ((db = create_and_init_and_config_db(db) ) == NULL) {
    printf("Failure at DB creation\n");
    return -1;
  }

  /*busyStatus = busy_handler_wrapper(db);
  printf("busyStatus : %d\n", busyStatus);
*/
  if ((sql = query_create_table(sql, db) ) != NULL) {
    printf("Created query successfully\n");
  }
  else {
    printf("failed creation of query\n");
    return -1;
  }

  sqlQuery = "PRAGMA locking_mode = EXCLUSIVE;";
  rc = sqlite3_exec(db, sqlQuery, callback, NULL, &zErrMsg);
  if (rc != SQLITE_OK) {
    printf("Error in PRAGMA locking_mode: %d %s", rc, zErrMsg);
    sqlite3_free(zErrMsg);
    return -1;
  }
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if (SQLITE_OK == rc) {
    free(sql);
    printf("success table creation\n");
  }
  else if (SQLITE_BUSY == rc) {
    printf("sqlite table creation failed due to error %d %s\n", rc, zErrMsg);
    free(sql);
    sqlite3_free(zErrMsg);
  }
  else {
    printf("failure in table creation due to error %d %s\n", rc, zErrMsg);
    sqlite3_free(zErrMsg);
    return -1;
  }
 /* if (SQLITE_OK == (rc = (sqlite3_exec(db, sql, callback, 0, &zErrMsg) ) ) ) {
    printf("success create table\n");
    free(sql);
  } else {
    printf("%d error in table creation : %s\n", rc, zErrMsg);
    free(sql);
    sqlite3_free(zErrMsg);
    return 0;
  }*/

  if ( (fd = open("records_3.txt", O_RDONLY)) < 1) {
    printf("Error opening file\n");
    return -1;
  }
  printf("open success\n");

  while (0 == (rd = read(fd, buff, 150000) ) ) {
    printf("Error in reading file\n");
    return -1;
  }
  printf("query of bytes : %d\n", rd);

  pthread_create(&write_thread, NULL, thread_action_write, (void*)db);

  timeOut = busy_timeout_set(db, 5);
  printf("timeout : %d\n", timeOut);

  while (1) {
    rc = sqlite3_total_changes(db);
    printf("number of changes : %d\n", rc);
    if (NULL == (sql = calloc(strlen(buff) ,sizeof(char)) ) ) {
    printf("Unable to allocate memory\n");
    }
    memcpy(sql, buff, strlen(buff));
    printf("query of records : %s\n", sql);

    printf("enter 1 to write into database\n");
    printf("enter 2 to read from database\n");
    printf("enter 3 to disconnect db\n");
    printf("4 to count records.\n");

    scanf("%d", &choice);

    switch(choice) {
      case 1  :
               sqlQuery = "PRAGMA locking_mode = EXCLUSIVE;";
               rc = sqlite3_exec(db, sqlQuery, callback, NULL, &zErrMsg);
               if (rc != SQLITE_OK) {
                 printf("Error in PRAGMA locking_mode: %d %s", rc, zErrMsg);
                 sqlite3_free(zErrMsg);
                 return -1;
               }
               clock_gettime(CLOCK_REALTIME, &start);
               rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
               if (rc != SQLITE_OK) {
                 printf("Error in writing\n due to %s", zErrMsg);
                 sqlite3_free(zErrMsg);
                 return -1;
               }
               clock_gettime(CLOCK_REALTIME, &stop);
               printf("start : %ld sec %ld nsec, stop : %ld sec %ldnsec \n", start.tv_sec, start.tv_nsec, stop.tv_sec, stop.tv_nsec);
               timeDiff = ((stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) )/ BILLION;
               printf("timediff : %lf\n", timeDiff);
               free(sql);
               start.tv_nsec = start.tv_sec = stop.tv_nsec = stop.tv_sec = 0;
               printf("sql freed\n");
               break;
      case 2  :
               sqlQuery = "PRAGMA locking_mode = EXCLUSIVE;";
               rc = sqlite3_exec(db, sqlQuery, callback, NULL, &zErrMsg);
               if (rc != SQLITE_OK) {
                 printf("Error in PRAGMA locking_mode: %d %s", rc, zErrMsg);
                 sqlite3_free(zErrMsg);
                 return -1;
               }
               pthread_create(&read_thread, NULL, thread_action_read, (void*)db);
               break;
      case 3  :
               printf("Closing Database connection\n");
               sqlite3_close(db);
      case 4  :
               sqlQuery = "select count(*) from employee;";
               sqlite3_exec(db, sqlQuery, callback, NULL, &zErrMsg);
               break;
      default :
                printf("enter a valid choice\n");
    }
    //printf("busyStatus : %d\n", busyStatus);
 }

}