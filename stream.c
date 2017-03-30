
#include "stream.h"
#include "dbutils.h"

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


char* compdate()
{
	char* tstmp;
	time_t rawNow;

	struct tm * now;

	/* gets the raw time from the clock */
	rawNow = time(NULL);
	now = localtime(&rawNow);

	tstmp = malloc(sizeof(char) * 15);

	/* YYYY MM DD HH mm SS */
	sprintf(tstmp, "%04d%02d%02d%02d%02d%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
	tstmp[14] = '\0';

	return tstmp;
}


int updateStream(struct userPost* st)
{
	char query[256];
	char post[1024];
	char* pdate;

	MYSQL sql;

	query[0] = '\0';
	post[0] = '\0';

	if(st == NULL)
	{
		return -1;
	}

	mysql_init(&sql);
	mysql_options(&sql, MYSQL_READ_DEFAULT_GROUP, "users");
	if(!mysql_real_connect(&sql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0))
	{
		return -1;
	}

	pdate = compdate();

	sprintf(post, "Sender: %s\nDate: %s\n%s", st->username, st->date, st->text);

	strcpy(query, "insert into posts values ('");
	strcat(query, st->streamname);
	strcat(query, "', '");
	strcat(query, st->username);
	strcat(query, "', '");
	strcat(query, pdate);
	strcat(query, "', '");
	strcat(query, post);
	strcat(query, "')");

	mysql_query(&sql, query);

	mysql_close(&sql);

	free(pdate);
	return 0;
}

int addUser(char* username, char* list)
{
	int i;
	int cpos;
	char buffer[512];

	MYSQL sql;

	cpos = 0;

	if(username == NULL || list == NULL)
	{
		return -1;
	}

	mysql_init(&sql);

	mysql_options(&sql, MYSQL_READ_DEFAULT_GROUP, "users");
	if(!mysql_real_connect(&sql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0))
	{
		return -1;
	}

	/* find all the commas */
	for(i = 0; i < strlen(list); i++)
	{
		buffer[cpos] = list[i];
		cpos ++;

		/* if we found a comma, we *should* have a complete file name */
		if(list[i] == ',' || i == strlen(list) - 1)
		{
			char query[256];

			buffer[cpos] = '\0';
			query[0] = '\0';

			if(buffer[cpos - 1] == ',')
			{
				buffer[cpos - 1] = '\0';
			}

			strcat(query, "insert into users values ('");
			strcat(query, username);
			strcat(query, "', '");
			strcat(query, buffer);
			strcat(query, "', '0')");

			mysql_query(&sql, query);

			cpos = 0;
			buffer[0] = '\0';

		}
	}

	mysql_close(&sql);
	return 0;
}

int removeUser(char* username, char* list)
{
	int i;
	int cpos;
	char buffer[256];

	MYSQL mysql;

	cpos = 0;

	if(username == NULL || list == NULL)
	{
		return -1;
	}

	mysql_init(&mysql);

	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "users");
	if(!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0))
	{
		return -1;
	}

	/* find all the commas */
	for(i = 0; i < strlen(list); i++)
	{
		buffer[cpos] = list[i];
		cpos ++;

		/* if we found a comma, we *should* have a complete file name */
		if(list[i] == ',' || i == strlen(list) - 1)
		{
			char query[256];

			query[0] = '\0';
			buffer[cpos] = '\0';

			if(buffer[cpos - 1] == ',')
			{
				buffer[cpos - 1] = '\0';
			}


			strcat(query, "delete from users where username='");
			strcat(query, username);
			strcat(query, "' and stream='");
			strcat(query, buffer);
			strcat(query, "'");

			mysql_query(&mysql, query);

			buffer[0] = '\0';
			cpos = 0;
		}
	}

	mysql_close(&mysql);

	return 0;
}

int checkIfPresent(char* fname, char* word)
{
	char line[1024];
	FILE* checkFile;

	checkFile = fopen(fname, "r");
	if(checkFile == NULL)
	{
		return 0;
	}

	/* check all lines in the file for word */
	while(fgets(line, 1023, checkFile) != NULL)
	{
		if(strstr(line, word) != NULL)
		{
			fclose(checkFile);
			return 1;
		}
	}

	fclose(checkFile);

	return 0;
}
