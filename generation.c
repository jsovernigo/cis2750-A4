/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 * 	this file contains source for an html generator. 
 */
#include "generation.h"
#include "tags.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void generatePage(char* fname)
{
	char line[2048];
	FILE* inFile;

	inFile = fopen(fname, "r");
	if(inFile == NULL)
	{
		return;
	}


	while(fgets(line, 2048, inFile) != NULL)
	{
		int i;
		char** tags;

		i = 0;

		/* gets the tags on the line, separated by .s */
		tags = splitTagLine(line);
		if(tags != NULL)
		{
			while(tags[i] != NULL)
			{
				char* markup;

				markup = generateTag(tags[i]);
				puts(markup);
				puts("");

				free(tags[i]);
				free(markup);
				i++;
			}
			free(tags);
		}

		/* puts a break after a line of the config. */
		puts(LINE_BREAK);
	}

	fclose(inFile);

	return;
}

char* generateTag(char* configTag)
{
	char* markup;

	switch(configTag[1])
	{
		case('d'):
			markup = malloc(sizeof(char) * 5);
			strcpy(markup, "<hr>");
			break;
		case('t'):
			markup = createText(configTag);
			break;
		case('h'):
			markup = createHeader(configTag);
			break;
		case('l'):
			markup = createLink(configTag);
			break;
		case('b'):
			markup = createButton(configTag);
			break;
		case('i'):
			markup = createInput(configTag);
			break;
		case('p'):
			markup = createPicture(configTag);
			break;
		case('r'):
			markup = createRadio(configTag);
			break;
		case('e'):
			markup = createExec(configTag);
			break;
		case('f'): /* stands for 'format' */
			markup = createDiv(configTag);
			break;
		case('g'):
			markup = malloc(sizeof(char) * (strlen("</div>") + 1));
			strcpy(markup, "</div>");
			break;
		case('z'):
			markup = createDependency(configTag);
			break;
		case('a'):
			markup = createAdd(configTag);
			break;
		case('v'):
			markup = createView(configTag);
			break;
		case('w'):
			markup = createPost(configTag);
			break;
		case('s'):
			markup = createSelector(configTag);
			break;
	}

	return markup;
}

char* getValue(char* attribute)
{
	int i;
	int startVal;
	int endVal;
	int cpos;

	char* value;

	/* collect the starting position for the tag's value. */
	for(i = 0; i < strlen(attribute); i++)
	{
		if(attribute[i] == '=')
		{
			/* this means we have a " delimiting the attribute value. */
			if(i + 1 < strlen(attribute) && attribute[i + 1] == '\"')
			{
				startVal = i + 2;
				break;
			}
			else
			{
				startVal = i + 1;
				break;
			}
		}
	}

	/* if this value ends with a quote, we need to avoid storing it. */
	if(attribute[strlen(attribute) - 1] == '\"')
	{
		endVal = strlen(attribute) - 2;
	}
	else
	{
		endVal = strlen(attribute) - 1;
	}

	value = malloc(sizeof(char) * (endVal - startVal + 2));
	if(value == NULL)
	{
		return NULL;
	}

	/* set the writing position to 0. */
	cpos = 0;

	/* collect all value characters. */
	for(i = startVal; i <= endVal; i++)
	{
		value[cpos] = attribute[i];
		cpos ++;
	}

	/* store the null terminator. */
	value[cpos] = '\0';

	return value;
}


char** splitTagLine(char* tagLine)
{
	int i;
	int j;
	int numTags;
	int inString;

	char** tags;

	numTags = 0;
	inString = 0;

	/* collect number of periods, this, when incrimented, is the number of arguments. */
	for(i = 0; i < strlen(tagLine); i++)
	{
		/* capture periods. */
		if(tagLine[i] == '.')
		{
			if(inString == 0)
			{
				numTags ++;
			}
		}
		else if(tagLine[i] == '\"')
		{
			if(inString == 1)
			{
				inString = 0;
			}
			else if(inString == 0)
			{
				inString = 1;
			}
		}
	}

	tags = malloc(sizeof(char*) * (numTags + 1));
	if(tags == NULL)
	{
		return NULL;
	}

	/* reset the string flag */
	inString = 0;

	j = 1;
	for(i = 0; i < numTags; i++)
	{
		int charsWritten;
		char tagBuffer[2048];

		/* pre-adds a period. */
		tagBuffer[0] = '.';

		charsWritten = 1;

		while((tagLine[j] != '.' && tagLine[j] != '\n') || inString == 1)
		{
			tagBuffer[charsWritten] = tagLine[j];

			/* set the inString variable. */
			if(tagLine[j] == '\"')
			{
				if(inString == 1)
				{
					inString = 0;
				}
				else
				{
					inString = 1;
				}
			}
			charsWritten++;
			j++;
		}

		tags[i] = malloc(sizeof(char) * (charsWritten + 1));
		if(tags[i] == NULL)
		{
			int k;
			for(k = i; k >= 0; k--)
			{
				free(tags[k]);
			}
			free(tags);

			return NULL;
		}

		tagBuffer[charsWritten] = '\0';
		strcpy(tags[i], tagBuffer);

		/* if we reached the end of the string */
		if(tagLine[j] == '\n')
		{
			break;
		}
		else if(tagLine[j] == '.')
		{
			j++;
		}
	}

	tags[numTags] = NULL;
	return tags;
}

char** getArgs(char* wholeTag)
{
	int i;
	int j;
	int startBracket;
	int endBracket;
	int numArgs;
	int inString;

	char** args;

	numArgs = 1;
	inString = 0;

	startBracket = 2;
	endBracket = strlen(wholeTag) - 1;

	/* TODO add support for in-string commas and brackets */

	/* if there is more than *nothing* between the two brackets */
	if(endBracket - startBracket > 1)
	{
		inString = 0;
		/* count the number of brackets present in the file; each one denote a new argument. */
		for(i = startBracket; i <= endBracket; i++)
		{
			if(wholeTag[i] == ',')
			{
				if(inString == 0)
				{
					numArgs ++;
				}
			}
			else if(wholeTag[i] == '"')
			{
				if(inString == 1)
				{
					inString = 0;
				}
				else if(inString == 0)
				{
					inString = 1;
				}
			}
		}
	}
	/* return NULL, there are no arguments. */
	else
	{
		return NULL;
	}

	args = malloc(sizeof(char*) * (numArgs + 1));
	if(args == NULL)
	{
		return NULL;
	}

	j = startBracket + 1;
	inString = 0;
	/* loop through the args, but not the last one. */
	for(i = 0; i < numArgs; i++)
	{
		int charsWritten;
		char argBuffer[1024];

		charsWritten = 0;

		/* grab the current argument's characters */
		while((wholeTag[j] != ',' && j < strlen(wholeTag) - 1) || inString == 1)
		{
			argBuffer[charsWritten] = wholeTag[j];
			if(wholeTag[j] == '\"')
			{
				if(inString == 1)
				{
					inString = 0;
				}
				else
				{
					inString = 1;
				}
			}
			charsWritten ++;
			j++;
		}

		/* upon exiting the loop, what was the last character read?  a comma? */
		args[i] = malloc(sizeof(char) * (charsWritten + 1));
		if(args[i] == NULL)
		{
			int k;

			/* if we fail to malloc, free every previous argument */
			for(k = i; k >= 0; k--)
			{
				free(args[k]);
			}
			free(args);
		}

		argBuffer[charsWritten] = '\0';
		strcpy(args[i], argBuffer);

		/* this means we have arrived at the end of the CURRENT tag! */
		if(wholeTag[j] == ')')
		{
			break;
		}
		else if(wholeTag[j] == ',')
		{
			/* skips the comma/end brace */
			j++;
		}
	}

	/* like a strings, the array of strings is NULL terminated! */
	args[numArgs] = NULL;

	return args;
}
