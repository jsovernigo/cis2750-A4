

#ifndef __GSOVERNIGO_DBUTILS_H__
#define __GSOVERNIGO_DBUTILS_H__

#include <mysql/mysql.h>

#define QUERY_MAX 512

#define HOSTNAME "dursley.socs.uoguelph.ca"
#define USERNAME "gsoverni"
#define PASSWORD "0948924"
#define DATABASE "gsoverni"


/**
 *
 */
int mysql_startConnect(MYSQL* sql);

/**
 *
 */
int mysql_make(MYSQL* sql);

/**
 *
 */
int mysql_clear(MYSQL* sql);

/**
 *
 */
int mysql_reset(MYSQL* sql);

/**
 *
 */
int mysql_posts(MYSQL* sql);

/**
 *
 */
int mysql_users(MYSQL* sql);

/**
 *
 */
int mysql_streams(MYSQL* sql);

/**
 *
 */
int getLastRead(char* username, char* stream, MYSQL* sql);

/**
 *
 */
int setLastRead(char* username, char* stream, int n, MYSQL* sql);

/**
 *
 */
int numposts(char* stream, MYSQL* sql);

/**
 *
 */
int getPostN(char* username, char* stream, char* order, int n, MYSQL* sql);

/**
 *
 */
int getStreams(char* username, MYSQL* sql);


#endif
