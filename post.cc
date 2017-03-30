/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 *	This file contains the C++Lite code for functions designed to
 *	collect and post information to a message board.
 */
 
#include "stream.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

class PostEntry
{

	char* readInput()
	{
		int currentSize;
		int currentLen;

		char temp[256];
		char* text;

		text = malloc(sizeof(char) * 512);
		/* check if malloc failed */
		if(text == NULL)
		{
			return NULL;
		}

		/* sets up the string's inital parameters. */
		currentSize = 512;
		text[0] = '\0';
		currentLen = 0;

		/* puts the initial prompt on the screen */
		fputs("Please enter your text: ", stdout);
		while(fgets(temp, 255, stdin) != NULL)
		{
			fputs("- ", stdout);

			/* adds the total size to the currently stored size */
			currentLen += strlen(temp);
			if(currentLen >= currentSize)
			{
				/* add extra length to the string */
				text = doubleString(text, &currentSize);
			}

			/* add it to the total buffer we are using. */
			strcat(text, temp);
		}
		puts("\n");

		if(text[strlen(text) - 1] == '\n' && text[strlen(text) - 2] == '\n')
		{
			text[strlen(text) - 1] = '\0';
		}

		return text;
	}

	char* getTimeDate()
	{
		char month[4];

		char* timeStamp;
		time_t rawNow;

		struct tm * now;

		/* gets the raw time from the clock */
		rawNow = time(NULL);
		now = localtime(&rawNow);

		/* mallocs the time stamp we are using for the current date. */

		timeStamp = malloc(sizeof(char) * strlen("MON. DD, YYYY HH:MM PM") + 1);

		/* essentially creates the month based on the local timestamp */
		switch(now->tm_mon)
		{
			case(0):
				strcpy(month, "Jan");
				break;
			case(1):
				strcpy(month, "Feb");
				break;
			case(2):
				strcpy(month, "Mar");
				break;
			case(3):
				strcpy(month, "Apr");
				break;
			case(4):
				strcpy(month, "May");
				break;
			case(5):
				strcpy(month, "Jun");
				break;
			case(6):
				strcpy(month, "Jul");
				break;
			case(7):
				strcpy(month, "Aug");
				break;
			case(8):
				strcpy(month, "Sep");
				break;
			case(9):
				strcpy(month, "Oct");
				break;
			case(10):
				strcpy(month, "Nov");
				break;
			case(11):
				strcpy(month, "Dec");
				break;
			default:
				/* error case, this will be because localtime() has failed for some reason. */
				break;
		}
		/* prints the final format to the string to be returned. */
		sprintf(timeStamp, "%3s. %2d, %4d %02d:%02d %2s", month, now->tm_mday, now->tm_year + 1900, now->tm_hour % 12, now->tm_min, (now->tm_hour / 12 > 0 )?"PM":"AM");
		timeStamp[22] = '\0';


		return timeStamp;
	}

	struct userPost* formatEntry(char* username, char* streamname, char* text)
	{
		char* timeStamp;
		
		struct userPost* newPost;

		newPost = malloc(sizeof(struct userPost));
		/* if malloc has for some reason failed */
		if(newPost == NULL)
		{
			return NULL;
		}

		timeStamp = getTimeDate();

		/* mallocs the items in the struct */
		newPost->username = malloc(sizeof(char) * strlen(username) + 1);
		newPost->streamname = malloc(sizeof(char) * strlen(streamname) + 1);
		newPost->date = malloc(sizeof(char) * strlen(timeStamp) + 1);
		newPost->text = malloc(sizeof(char) * strlen(text) + 1);
		
		/* copies the info into the struct. */
		strcpy(newPost->username, username);
		strcpy(newPost->streamname, streamname);
		strcpy(newPost->date, timeStamp);
		strcpy(newPost->text, text);
		
		free(timeStamp);

		return newPost;
	}

	int submitPost(struct userPost* post)
	{
		if(post == NULL)
		{
			return -1;
		}

		return updateStream(post);
	}
};

char* doubleString(char* currentString, int * currentLen)
{
	int i;
	char* newString;

	/* creates a new string, twice as long */
	newString = malloc(sizeof(char) * (*currentLen) * 2 + 1);
	*currentLen *= 2;

	/* copy the old string */
	for(i = 0; i < strlen(currentString); i++)
	{
		newString[i] = currentString[i];
	}

	/* frees the old string */
	free(currentString);

	return newString;
}

int main(int argc, char** argv)
{
	int result;
	char userName[512];
	char* post;
	char stream[512];

	struct userPost* pt;

	class PostEntry pe;

	if(argc < 4)
	{
		return 1;
	}

	strcpy(userName, argv[1]);
	strcpy(stream, argv[2]);
	post = malloc(sizeof(char) * (strlen(argv[3]) + 1));
	strcpy(post, argv[3]);

	pt = pe.formatEntry(userName, stream, post);
	result = pe.submitPost(pt);

	free(pt->username);
	free(pt->streamname);
	free(pt->date);
	free(pt->text);
	free(pt);

	free(post);

	if(result != 0)
	{
		return 1;
	}

	return 0;
}
