/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 *	This file contains the main function for the addauthor program.
 *	Utilizes stream.h to update stream files located in messages/
 */
#include "stream.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int remove;
	char list[1024];
	char userName[512];

	/* checks the number of args present, makes sure we actually get a file name */
	if(argc < 3)
	{
		return 1;
	}

	remove = 0;

	strcpy(userName, argv[1]);
	if(strcmp(argv[2], "-r") == 0)
	{
		return 1;
	}
	strcpy(list, argv[2]);

	if(argc > 3)
	{
		if(strcmp(argv[3], "-r") == 0)
		{
			remove = 1;
		}
	}

	if(!remove)
	{
		addUser(userName, list);
	}
	else
	{
		removeUser(userName, list);
	}
	
	return 0;
}
