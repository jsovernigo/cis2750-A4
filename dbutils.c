
#include "dbutils.h"
#include "vector.h"

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int mysql_startConnect(MYSQL* sql)
{
	mysql_init(sql);

	mysql_options(sql, MYSQL_READ_DEFAULT_GROUP, "users");
	if(!mysql_real_connect(sql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0))
	{
		return -1;
	}

	return 0;
}


int mysql_make(MYSQL* sql)
{
	int result;

	result = mysql_query(sql, "create table users (username varchar(64), stream varchar(32), lastread integer(5))");
	result += mysql_query(sql, "create table streams (stream varchar(32))");
	result += mysql_query(sql, "create table posts (stream varchar(32), username varchar(64), pdate char(14), post varchar(10000))");

	return result;
}


int mysql_clear(MYSQL* sql)
{
	int result;

	result = mysql_query(sql, "truncate table users");
	result += mysql_query(sql, "truncate table posts");
	result += mysql_query(sql, "truncate table streams");

	return result;
}

int mysql_reset(MYSQL* sql)
{
	int result;

	result = mysql_query(sql, "drop table users");
	result += mysql_query(sql, "drop table posts");
	result += mysql_query(sql, "drop table streams");

	return result;
}

int mysql_posts(MYSQL* sql)
{
	MYSQL_RES* result;
	MYSQL_ROW row;

	mysql_query(sql, "select post from posts order by pdate");

	result = mysql_store_result(sql);
	if(!result)
	{
		return -1;
	}

	while((row = mysql_fetch_row(result)))
	{
		printf("%s\n\n", row[0]);
	}

	return 0;
}

int mysql_users(MYSQL* sql)
{
	MYSQL_RES* result;
	MYSQL_ROW row;

	mysql_query(sql, "select distinct username from users");

	result = mysql_store_result(sql);
	if(!result)
	{
		return -1;
	}

	while((row = mysql_fetch_row(result)))
	{
		printf("%s\n", row[0]);
	}

	return 0;
}

int mysql_streams(MYSQL* sql)
{
	MYSQL_RES* result;
	MYSQL_ROW row;

	mysql_query(sql, "select distinct stream from users");

	result = mysql_store_result(sql);
	if(!result)
	{
		return -1;
	}

	while((row = mysql_fetch_row(result)))
	{
		printf("%s\n", row[0]);
	}

	return 0;
}

/*-----------------------------------------------------------------------*/

int getLastRead(char* username, char* stream, MYSQL* sql)
{
	char query[256];
	MYSQL_RES* result;
	MYSQL_ROW row;

	query[0] = '\0';

	sprintf(query, "select lastRead from users where username='%s' and stream='%s'", username, stream);
	mysql_query(sql, query);

	result = mysql_store_result(sql);
	
	row = mysql_fetch_row(result);

	return atoi(row[0]);
}

int setLastRead(char* username, char* stream, int n, MYSQL* sql)
{
	int lastRead;
	char query[256];

	lastRead = getLastRead(username, stream, sql);
	if(n <= lastRead)
	{
		return 1;
	}

	sprintf(query, "update users set lastread='%d' where users.stream='%s' and users.username='%s'", n, stream, username);

	mysql_query(sql, query);
	
	return 0;
}

int numposts(char* stream, MYSQL* sql)
{
	char query[256];
	MYSQL_RES* result;

	if(strcmp(stream, "all") == 0)
	{
		sprintf(query, "select * from posts");
	}
	else
	{
		sprintf(query, "select * from posts where stream='%s'", stream);
	}

	mysql_query(sql, query);
	result = mysql_store_result(sql);

	return mysql_num_rows(result);
}

int getPostN(char* username, char* stream, char* order, int n, MYSQL* sql)
{
	int i;
	char query[256];
	MYSQL_RES* result;
	MYSQL_ROW row;

	query[0] = '\0';

	if(strcmp(stream, "all") == 0)
	{

		sprintf(query, "select * from posts order by %s", order);
		mysql_query(sql, query);
	}
	else
	{
		sprintf(query, "select * from posts where stream='%s' order by %s", stream, order);
		mysql_query(sql, query);
	}

	result = mysql_store_result(sql);

	for(i = 0; i < n; i++)
	{
		row = mysql_fetch_row(result);
	}

	row = mysql_fetch_row(result);

	/* this is the correct row, and also the post segment. */
	puts(row[3]);
	setLastRead(username, stream, n, sql);

	return 0;
}

int getStreams(char* username, MYSQL* sql)
{
	char allStreams[512];
	char query[256];
	MYSQL_RES* result;
	MYSQL_ROW row;

	allStreams[0] = '\0';

	sprintf(query, "select stream from users where username='%s'", username);

	mysql_query(sql, query);

	result = mysql_store_result(sql);

	while((row = mysql_fetch_row(result)))
	{

	}

	return 0;
}
