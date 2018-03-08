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
#include "../../sqlite/ext/async/sqlite3async.h"

/*************************************************************************************
 *                          LOCAL MACROS                                              *
 *************************************************************************************/
 #define BUF_SIZE       1024

/*************************************************************************************
 *                          LOCAL TYPEDEFS                                            *
 *************************************************************************************/

 /*************************************************************************************
 *                          LOCAL PROTOTYPES                                          *
 *************************************************************************************/
static void async_process_init(void);
static sqlite3* create_and_init_and_config_db(sqlite3 *db);
static char * query_create_table(char *sql, sqlite3 *db);
static bool enable_load_extension(sqlite3 *db);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
static void *thread_action_1(void *db);
static void *thread_action_2(void *db);
static void *thread_action_3(void *db);

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
static void *thread_action_1(void *db) {
  db = (sqlite3*) db;
  char *sql, *zErrMsg;
  int rc;
  sqlite3async_run();
  //sqlite3async_control(SQLITEASYNC_GET_LOCKFILES);
  //sqlite3async_shutdown();
  sql = "INSERT INTO company values (1, 'Paul', 32, '8989564514', datetime('now') ); " ;

  if (SQLITE_OK == (rc = (sqlite3_exec(db, sql, callback, 0, &zErrMsg) ) ) ) {
    printf("inserted 1st successfully\n");
  } else {
    printf("failure action_1 : %s",zErrMsg);
  }

  sql = "INSERT INTO COMPANY VALUES (5, 'Dave', 35, '9897949596', datetime('now') ); ";

  if (SQLITE_OK == (rc = (sqlite3_exec(db, sql, callback, 0, &zErrMsg) ) ) ) {
    printf("inserted 1st successfully\n");
  } else {
    printf("failure action_1 5th record : %s",zErrMsg);
  }
  sql = "select *from company";
  printf("thread_1\n");
  sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  return NULL;
}

/********************************************//**
Name        :
Input(s)    :
Output(s)   :
Description :
***********************************************/
static void *thread_action_2(void *db) {
  db = (sqlite3*) db;
  char *sql, *zErrMsg;
  int rc;

  sql = "INSERT INTO company VALUES (2, 'Allen', 25, '9798969299', datetime('now') ); "  ;
  if (SQLITE_OK == (rc = (sqlite3_exec(db, sql, callback, 0, &zErrMsg) ) ) ) {
    printf("inserted 2nd record successfully\n");
  } else {
    printf("failure action_2 : %s",zErrMsg);
  }

  sql = "insert into company values (4, 'Marx', 27, json('{\"key\":\"place\"}'), datetime('now'));";
  if (SQLITE_OK == (rc = (sqlite3_exec(db, sql, callback, 0, &zErrMsg) ) ) ) {
    printf("inserted 2nd record successfully\n");
  } else {
    printf("failure action_2 id = 4: %s",zErrMsg);
  }
  sql = "select *from company";
  printf("thread_action : 2\n");
  sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  return NULL;
}

/********************************************//**
Name        :
Input(s)    :
Output(s)   :
Description :
***********************************************/
static void *thread_action_3(void *db) {
  db = (sqlite3*) db;
  char *sql, *zErrMsg;
  int rc;

  sql = "INSERT INTO company values (3, 'Teddy', 23, '9595878745', datetime('now') );" ;
  if (SQLITE_OK == (rc = (sqlite3_exec(db, sql, callback, 0, &zErrMsg) ) ) ) {
    printf("inserted 3 record successfully\n");
  } else {
    printf("failure action_3 : %s",zErrMsg);
  }
  return NULL;
}

/********************************************//**
Name        :
Input(s)    :
Output(s)   :
Description :
***********************************************/
static sqlite3 *create_and_init_and_config_db(sqlite3 *db) {
  int rc;

  rc = sqlite3_open_v2("Elear.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, NULL);
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
    return NULL;
  }
  memcpy(sql, buff, strlen(buff));
  return sql;
}


/*************************************************************************************
*                          PUBLIC FUNCTIONS                                          *
*************************************************************************************/
/********************************/
int main(int argc, char *arv[]) {
  sqlite3 *db = NULL;
  sqlite3_stmt *stmt;

  char *sqlQuery;
  int rc;
  int id;
  char *name = NULL;
  char *sql = NULL, *zErrMsg;
  pthread_t thread1;
  pthread_t thread2;
  pthread_t thread3;

  async_process_init();
  if ((db = create_and_init_and_config_db(db)) == NULL) {
    printf("Failure at DB creation\n");
    return -1;
  }

  if (enable_load_extension(db) == false) {
    printf("Failed to enable the extensions\n");
    return -1;
  }

  sqlQuery = "PRAGMA journal_mode = WAL;";
  rc = sqlite3_exec(db, sqlQuery, callback, NULL, &zErrMsg);
  /*rc = sqlite3_load_extension(db, "/home/dhavalpunjabi/sqlite/ext/misc/json1.so", NULL, &zErrMsg);

  if (SQLITE_OK == rc) {
    printf("linked successfully\n");
  } else {
    printf("%d : try again : %s\n", rc, zErrMsg);
    //return 0;
  }*/

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
  } else {
    printf("%d error in table creation\n : %s\n", rc, zErrMsg);
    sqlite3_free(zErrMsg);
    return 0;
  }
  sleep(2);
  pthread_create(&thread1, NULL, thread_action_1, (void*)db);
  pthread_create(&thread2, NULL, thread_action_2, (void*)db);
  pthread_create(&thread3, NULL, thread_action_3, (void*)db);

  sqlQuery = "insert into company values (7, json('{\"name\": \"Dhaval\"}'), 23, '878766525', datetime('now'));";
  sqlite3_exec(db, sqlQuery, callback, 0, &zErrMsg);
  printf("in main\n");
  sqlQuery = "select *from company";
  sqlite3_exec(db, sqlQuery, callback, 0, &zErrMsg);

  free(sql);
  sqlQuery = "insert into company values (6, json('{\"name\": \"Theja\"}'), 30, '8787456525', datetime('now'));";
  sqlite3_exec(db, sqlQuery, callback, 0, &zErrMsg);

  rc = sqlite3_exec(db, "PRAGMA foreign_keys = ON;", callback, 0, NULL);
  printf("rc = %d\n", rc);

  sqlQuery = "insert into project values (7, json('{\"name\": \"Karan\"}'), 30, '8787658525', datetime('now'));";
  rc = sqlite3_exec(db, sqlQuery, callback, 0, NULL);

  printf("foreign key constraint check rc: %d\n", rc);
  rc = sqlite3_threadsafe();

  printf("threadsafe status : %d", rc);

  sqlite3_prepare_v2(db, "select *from company", -1, &stmt, NULL);
  while (SQLITE_DONE != sqlite3_step(stmt) ) {

    for (int i = 0; i < 2; i++) {

      if ((sqlite3_column_type(stmt, i) ) == SQLITE_INTEGER)  {
        id = sqlite3_column_int(stmt, i);
        printf("id = %d\n", id);
      }
      if ((sqlite3_column_type(stmt, i) ) == SQLITE_TEXT) {
        name = (char *)sqlite3_column_text(stmt, i);
        printf("name = %s\n", name);
      }
    }
  }
  sqlQuery = "select json_extract(company.NAME, '$.name') from company where EMP_ID = 6;";
  sqlite3_exec(db, sqlQuery, callback, 0, &zErrMsg);
  sqlite3_close(db);
}
