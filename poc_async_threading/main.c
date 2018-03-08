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
Name        :
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
Name        :
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
Name        :
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
Name        :
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
Name        :
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
Name        :
Input(s)    :
Output(s)   :
Description :
***********************************************/
static void *thread_action_read(void *db) {
  int rc;
  const char *sql = "select count(*)from employee;";
  char *zErrMsg;
  char *sqlQuery;
  rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
  if (SQLITE_OK == rc) {
    printf("read successfull\n");
  }
  else {
   printf("read unsuccessfull due to %s !! ... \n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  sqlQuery = "select id, salary , name from employee where id = 100000;";
  rc = sqlite3_exec(db, sqlQuery, callback, NULL, &zErrMsg);
  printf("read status_1 : %d %s\n", rc, zErrMsg);

  sqlQuery = "select id, salary, name from employee where id = 2;";
  rc = sqlite3_exec(db, sqlQuery, callback, NULL, &zErrMsg);
  printf("read status_2 : %d %s\n", rc, zErrMsg);


   /* sql = "update company set EMP_ID = 2 where EMP_ID = 1;";
    rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg); */

  /*int i;
  static int j;
  const char *sql = "update company set ID = ";
  char *sqlQuery = calloc(strlen(sql) + 30, sizeof(char) );
  char *zErrMsg;

  printf("In read thread with threadId = %lu\n", pthread_self());
  sqlite3async_run();
  for (i = 0; i < 1000; i++, j++) {
    sprintf(sqlQuery, "%s %d where ID = %d", sql, j, j+1);
    rc = sqlite3_exec(db, sqlQuery, callback, NULL, &zErrMsg);
    if (SQLITE_OK == rc) {
      printf("update successfull\n");
    }
    else {
      printf("update unsuccessfull due to %s !! ... \n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
  } */
  return NULL;
}

/*************************************************************************************
*                          PUBLIC FUNCTIONS                                          *
*************************************************************************************/
/********************************/
int main (int argc, char *argv[]) {
  sqlite3 *db = NULL;
  char *sql = NULL;
  char *sqlQuery = "select count(*) from employee;";
  char *zErrMsg;
  int rc , choice;
  struct timespec start, stop;
  sqlite3_vfs *vfs;
  pthread_t write_thread;
  pthread_t read_thread;
  double timeDiff;
  int fd, rd;
  char buff[BUF_SIZE] = {'\0'};


  sqlite3_initialize();

  vfs = sqlite3_vfs_find("unix");
  sqlite3_vfs_register(vfs, 1);

  async_process_init();
  /*rc = sqlite3async_control(SQLITEASYNC_HALT, SQLITEASYNC_HALT_IDLE);
  if (rc != SQLITE_OK) {
    printf("control assign of halt failure\n");
    return -1;
  }
  else {
    printf("assigned control for halt\n");
  }*/

  sqlite3async_control(SQLITEASYNC_GET_HALT, &rc);
  printf("halt status : %d", rc);

  rc = sqlite3async_control(SQLITEASYNC_LOCKFILES, 0);
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

  if ((sql = query_create_table(sql, db) ) != NULL) {
    printf("Created query successfully\n");
  }
  else {
    printf("failed creation of query\n");
    return -1;
  }

  if (SQLITE_OK == (rc = (sqlite3_exec(db, sql, callback, 0, &zErrMsg) ) ) ) {
    printf("success create table\n");
    free(sql);
  } else {
    printf("%d error in table creation\n : %s\n", rc, zErrMsg);
    free(sql);
    sqlite3_free(zErrMsg);
    return 0;
  }

  if ( (fd = open("records_3.txt", O_RDONLY)) < 1) {
    printf("Error opening file\n");
    return -1;
  }
  printf("open success\n");

  while (0 == (rd = read(fd, buff, 150000) ) ) {
    printf("Error in reading file\n");
    return -1;
  }
  printf("query of bytes : %d", rd);

  pthread_create(&write_thread, NULL, thread_action_write, (void*)db);

  while (1) {
    rc = sqlite3_total_changes(db);
    printf("number of changes : %d\n", rc);
    if (NULL == (sql = malloc(strlen(buff)) ) ) {
    printf("Unable to allocate memory\n");
    }
    memcpy(sql, buff, strlen(buff));

    printf("enter 1 to write into database\n");
    printf("enter 2 to read from database\n");
    printf("enter 3 to disconnect db\n");
    printf("4 to count records.\n");

    scanf("%d", &choice);

    switch(choice) {
      case 1  :
               clock_gettime(CLOCK_REALTIME, &start);
               sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
               clock_gettime(CLOCK_REALTIME, &stop);
               timeDiff = (stop.tv_nsec - start.tv_nsec) / MILLION;
               printf("timediff : %lf\n", timeDiff);
               free(sql);
               start.tv_nsec = start.tv_sec = stop.tv_nsec = stop.tv_sec = 0;
               printf("sql freed\n");
               break;
      case 2  :
               pthread_create(&read_thread, NULL, thread_action_read, (void*)db);
               break;
      case 3  :
               printf("Closing Database connection\n");
               sqlite3_close(db);
      case 4  :
               sqlite3_exec(db, sqlQuery, callback, NULL, &zErrMsg);
               break;
      default :
                printf("enter a valid choice\n");
    }
 }

}