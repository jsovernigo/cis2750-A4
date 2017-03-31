
#include "dbutils.h"

#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int result;
	char command[64];
	char username[64];
	char stream[64];
	int index;
	char order[64];
	MYSQL mysql;


	if(argc < 2)
	{
		puts("Incorrect usage.  Please invoke as:\n\t./db [-clear][-reset][-posts][-users][-streams]");
		return 1;
	}
	else
	{
		strcpy(command, argv[1]);
		if(argc > 5)
		{
			strcpy(username, argv[2]);
			strcpy(stream, argv[3]);
			index = atoi(argv[4]);		
			strcpy(order, argv[5]);
		}
	}

	result = mysql_startConnect(&mysql);

	if(result < 0)
	{
		puts("Error in initializing SQL");
	}

	result = 0;

	if(strcmp(command, "-clear") == 0)
	{
		result = mysql_clear(&mysql);
	}
	else if(strcmp(command, "-make") == 0)
	{
		result = mysql_make(&mysql);
	}
	else if(strcmp(command, "-reset") == 0)
	{
		result = mysql_reset(&mysql);
	}
	else if(strcmp(command, "-posts") == 0)
	{
		result = mysql_posts(&mysql);
	}
	else if(strcmp(command, "-users") == 0)
	{
		result = mysql_users(&mysql);
	}
	else if(strcmp(command, "-streams") == 0)
	{
		result = mysql_streams(&mysql);
	}
	else if(strcmp(command, "-help") == 0)
	{
		puts("NAME\n\tdb - manages an sql database to hold message posts.\n");
		puts("SYNOPSIS\n\tdb [command]\n");
		puts("DESCRIPTION\n");
		puts("\t-make\n\t\tcreates the databases to be used to store the messages.\n");
		puts("\t-clear\n\t\tclears the database files of all entries.\n");
		puts("\t-reset\n\t\tdrops all the tables from the database.\n");
		puts("\t-posts\n\t\tprints all the posts that are in the database.\n");
		puts("\t-streams\n\t\tprints all the streams found in the database.\n");
		puts("\t-users\n\t\tprints all the users found in the database.\n");
		puts("\t-help\n\t\t... really...?\n");
		puts("AUTHOR\n\tJulian Sovernigo\n\tgsoverni@mail.socs.uoguelph.ca\n\t0948924");
	}
	else if(strcmp(command, "last") == 0)
	{
		int lr;

		lr = getLastRead(username, stream, &mysql);
		printf("%d\n", lr);
	}
	else if(strcmp(command, "view") == 0)
	{
		getPostN(username, stream, order, index, &mysql);
	}
	else if(strcmp(command, "len") == 0)
	{
		int nposts;

		nposts = numposts(stream, &mysql);
		printf("%d\n", nposts);
	}
	else if(strcmp(command, "streams") == 0)
	{
		getStreams(username, &mysql);	
	}

	if(result != 0)
	{
		puts("Something went wrong!");
	}


	return 0;
}
