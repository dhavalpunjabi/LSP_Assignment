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
#include <stdarg.h>
#include <time.h>
#include "../../sqlite/ext/async/sqlite3async.h"

/*************************************************************************************
 *                          LOCAL MACROS                                              *
 *************************************************************************************/

/*************************************************************************************
 *                          LOCAL TYPEDEFS                                            *
 *************************************************************************************/
#define BUFSIZE                        1000000

 /*************************************************************************************
 *                          LOCAL PROTOTYPES                                          *
 *************************************************************************************/
static void async_process_init(void);
static sqlite3* create_and_init_and_config_db(sqlite3 *db);
static bool enable_load_extension(sqlite3 *db);
static char * query_create_table(char *sql, sqlite3 *db);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
static void *thread_action_read(void *db);
static void *thread_action_write(void *db);
static void *thread_action_write_2(void *db);

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
  sqlite3async_initialize(NULL, 0);
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
static bool enable_load_extension(sqlite3 *db) {
  int rc;

  rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION, 1, NULL);
  if (SQLITE_OK == rc) {
    printf("success enable extension\n");
    return true;
  } else {
    printf("Failed to enable the configuration\n");
    return false;
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
  char buff[BUFSIZE] = {'\0'};

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
  for(i = 0; i<argc; i++) {
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
  struct timespec start, end;
  db = (sqlite3*)db;
  int rc;
  int fd, rd;
  char buff[BUFSIZE] = {'\0'};
  char *zErrMsg;
  char *sql;

  clock_gettime(CLOCK_REALTIME, &start);
  printf("thread Id : %lu", pthread_self());
  if ( (fd = open("records.txt", O_RDONLY)) < 1) {
    printf("Error opening file\n");
    return NULL;
  }

  while (0 == (rd = read(fd, buff, 150000) ) ) {
    printf("Error in reading file\n");
    return NULL;
  }
  printf("query of bytes : %d", rd);

 if (NULL == (sql = malloc(strlen(buff)) ) ) {
    printf("Unable to allocate memory\n");
    pthread_exit(NULL);
  }
  memcpy(sql, buff, strlen(buff));
  if (SQLITE_OK == (rc = (sqlite3_exec(db, sql, callback, 0, &zErrMsg) ) ) ) {
    printf("success writing ...\n");
  } else {
    printf("%d error in table creation\n : %s\n", rc, zErrMsg);
    free(sql);
    sqlite3_free(zErrMsg);
  }
  clock_gettime(CLOCK_REALTIME, &end);
  printf("time diff : %ld\n", end.tv_sec - start.tv_sec);
  printf("Exit thread write 1 ...\n");
  pthread_exit(NULL);
}

/********************************************//**
Name        :
Input(s)    :
Output(s)   :
Description :
***********************************************/
static void *thread_action_write_2(void *db){
  printf("Exit thread write 2 ...\n");
  pthread_exit(NULL);
}

/********************************************//**
Name        :
Input(s)    :
Output(s)   :
Description :
***********************************************/
static void *thread_action_read(void *db) {
  printf("in thread_2\n");
  db = (sqlite3*)db;
  char *sql, *zErrMsg;
  sql = "select *from company;";
  sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  sql = "select from company where EMP_ID < 5;";
  sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  pthread_exit(NULL);
}

/*************************************************************************************
*                          PUBLIC FUNCTIONS                                          *
*************************************************************************************/
/********************************/
int main (int argc, char *argv[]) {
  sqlite3 *db = NULL;
  sqlite3_vfs *vfs;
  int rc , choice = 0;
  char *sqlQuery;
  char *sql = NULL, *zErrMsg;
  pthread_t write_thread;
  pthread_t read_thread;

  sqlite3_initialize();

  if ((db = create_and_init_and_config_db(db) ) == NULL) {
    printf("Failure at DB creation\n");
    return -1;
  }

  if (enable_load_extension(db) == false) {
    printf("Failed to enable the extensions\n");
    return -1;
  }

  sqlQuery = "PRAGMA journal_mode = WAL;";
  rc = sqlite3_exec(db, sqlQuery, callback, NULL, &zErrMsg);
  printf("Wal mode = %d", rc);

  /*vfs = sqlite3_vfs_find("unix");
  printf("vfs address : %p\n", (void*)vfs);

  rc = sqlite3_vfs_register(vfs, 1);
  if (rc != SQLITE_OK) {
    printf("vfs not registered\n");
    return -1;
  }*/

  async_process_init();
  rc = sqlite3async_control(SQLITEASYNC_HALT, SQLITEASYNC_HALT_NOW);
  if (rc != SQLITE_OK) {
    printf("control assign failure\n");
    return -1;
  }
  else {
    printf("assigned control\n");
  }
  sqlite3async_run();

  if ((sql = query_create_table(sql, db) ) != NULL) {
    printf("Created query successfully\n");
  }
  else {
    printf("failed creation of query\n");
    return -1;
  }
  printf ("sql : %s\n", sql);

  if (SQLITE_OK == (rc = (sqlite3_exec(db, sql, callback, 0, &zErrMsg) ) ) ) {
    printf("success create table\n");
    free(sql);
  } else {
    printf("%d error in table creation\n : %s\n", rc, zErrMsg);
    free(sql);
    sqlite3_free(zErrMsg);
    return 0;
  }
  while (1) {
    printf("enter 1 to write into database\n");
    printf("enter 2 to read from database\n");
    printf("enter 3 to write 2 into database\n");
    printf("enter 4 to disconnect db\n");
    scanf("%d", &choice);

    switch(choice) {
      case 1  :
               pthread_create(&write_thread, NULL, thread_action_write, (void*)db);
               break;
      case 2  :
               pthread_create(&read_thread, NULL, thread_action_read, (void*)db);
               break;
      case 4  :
               printf("Closing Database connection\n");
               sqlite3_close(db);
      case 3  :
               pthread_create(&write_thread, NULL, thread_action_write_2, (void*)db);
               break;
      default :
               printf("enter a valid choice\n");
    }

  }
  return 0;
}