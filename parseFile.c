/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 * 	This file contains several methods for parsing a file into tokens
 * 	that, if printed again in that order, will constitute a working cpp
 * 	program.
 */
#include"parseFile.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<assert.h>

Vector* getTokens(char* fname)
{
	char c;
	char lastPrint;
	int i;

	int inString;
	int in89Comment;
	int in99Comment;
	/*
	int inInclude;
	*/
	char* buffer;

	FILE* fin;
	Vector* tokens;

	fin = fopen(fname, "r");
	if(fin == NULL)
	{
		return NULL;
	}

	buffer = malloc(sizeof(char)*512);

	lastPrint = '\0';

	inString = 0;
	in89Comment = 0;
	in99Comment = 0;
	/*inInclude = 0;*/
	i = 0;

	/* sets up the vector to be filled */
	tokens = makeVector(100);

	/* collects the characters into tokens one char at a time */
	do
	{
		c = fgetc(fin);

		if(strchr("\n\t\r ", c) != NULL)
		{
			/* if we need to preserve whitespace right now. */
			if(inString == 1 || in89Comment == 1)
			{
				buffer[i] = c;
				i++;
				lastPrint = c;
			}
			else if(in99Comment == 1)
			{
				if(c == '\n' || c == '\r')
				{
					char* newString;

					buffer[i] = '\0';
					i++;
					appendString(tokens, buffer, i);
					lastPrint = '\0';
					i = 0;
					in99Comment = 0;

					newString = malloc(sizeof(char) * 3);
					newString[0] = c;
					newString[1] = '\0';

					appendString(tokens, newString, strlen(newString));
					free(newString);
				}
				else
				{
					buffer[i] = c;
					i++;
					lastPrint = c;
				}
			}
			else if(inString == 0 && in89Comment == 0 && in99Comment == 0)
			{
				char* newString;

				buffer[i] = '\0';
				i++;
				appendString(tokens, buffer, i);
				lastPrint = '\0';
				i = 0;

				newString = malloc(sizeof(char) * 3);
				newString[0] = c;
				newString[1] = '\0';

				appendString(tokens, newString, strlen(newString));
				free(newString);
			}
		}
		else if(strchr(".,;(){}[]", c) != NULL) /* this grabs punctuation */
		{
			if(inString == 0 && in89Comment == 0 && in99Comment == 0)
			{
				if(i != 0)
				{
					buffer[i] = '\0';
					i++;
					appendString(tokens, buffer, i);
					i = 0;
				}

				/* this saves the new token */
				buffer[i] = c;
				i++;

				buffer[i] = '\0';
				i++;

				appendString(tokens, buffer, i);
				lastPrint = '\0';
				i = 0;
			}
			else
			{
				buffer[i] = c;
				lastPrint = c;
				i++;
			}
		}
		else if(strchr("+-*/%=", c) != NULL)
		{
			if(c == '/') /* possible comment, or division... */
			{
				/* if our last print was a slash too, then we are in a comment right now */
				if(lastPrint == '/' && inString == 0)
				{
					in99Comment = 1;
				}
				else if(lastPrint == '*' && inString == 0)
				{
					in89Comment = 0;
				}

				buffer[i] = c;
				i++;
				lastPrint = c;
			}
			else if(c == '+' || c == '-')
			{
				if(inString == 0 && in89Comment == 0 && in99Comment == 0)
				{
					/* this means we are in a ++ or a -- situation */
					if(lastPrint == c)
					{
						buffer[i] = c;
						i++;
						lastPrint = c;

						buffer[i] = '\0';
						i++;
						appendString(tokens, buffer, i);
						lastPrint = '\0';
						i = 0;
					}
					else
					{
						if(i != 0)
						{
							buffer[i] = '\0';
							i++;
							appendString(tokens, buffer, i);
							lastPrint = '\0';
							i = 0;
						}
						buffer[i] = c;
						i++;
						lastPrint = c;
					}
				}
				else
				{
					buffer[i] = c;
					i++;
					lastPrint = c;
				}
			}
			else if(c == '*') /* we have found an asterisk... might be a comment... */
			{
				if(lastPrint == '/' && inString == 0)
				{
					in89Comment = 1;
				}

				if(inString == 0 && in89Comment == 0 && in99Comment == 0)
				{
					/* this basically identifies the "**" in char** argv as one token */
					if(lastPrint != '*')
					{
						buffer[i] = '\0';
						i++;
						appendString(tokens, buffer, i);
						lastPrint = '\0';
						i = 0;
					}
				}
				/* we need to print something */
				buffer[i] = c;
				i++;
				lastPrint = c;
			}
			else if(c == '%') /* either a modulus or a printf formatter */
			{
				if(inString == 0 && in89Comment == 0 && in99Comment == 0)
				{
					if(i != 0)
					{
						buffer[i] = '\0';
						i++;
						appendString(tokens, buffer, i);
						lastPrint = '\0';
						i = 0;
					}
				}
				buffer[i] = c;
				i++;
				lastPrint = '\0';
			}
			else if(c == '=')
			{
				if(inString == 0 && in89Comment == 0 && in99Comment == 0)
				{
					/* we are in a += kind of situation */
					if(strchr("+-*/%", lastPrint) != NULL)
					{
						buffer[i] = c;
						i++;
						lastPrint = c;

						buffer[i] = '\0';
						i++;
						appendString(tokens, buffer, i);
						lastPrint = '\0';
						i = 0;
						continue;
					}
					else if(i != 0) /* if there is currently a non-operator in the buffer */
					{
						buffer[i] = '\0';
						i++;
						appendString(tokens, buffer, i);
						lastPrint = '\0';
						i = 0;
					}

					/* outside of a string/comment, there is no time when this would be tacked
					   onto anything. */
					lastPrint = c;
					buffer[i] = c;
					i++;

					buffer[i] = '\0';
					i++;
					appendString(tokens, buffer, i);
					lastPrint = '\0';
					i = 0;
				}
				else /* this would run if we found a += in a comment or string */
				{
					buffer[i] = c;
					i++;
					lastPrint = c;
				}
			}
		}
		else /* essentially, any non-special character. */
		{
			if(c == '\"')
			{
				/* if we are not in a string and NOT in a comment, we are now in a string */
				if(inString == 0 && in89Comment == 0 && in99Comment == 0)
				{
					inString = 1;
				}
				else
				{
					/* this would be like a \" situation in a string */
					if(lastPrint != '\\')
					{
						inString = 0;
					}
				}
			}
			if(inString == 0 && in89Comment == 0 && in99Comment == 0)
			{
				/* if our last print was an operator, we need to print it first */
				if(strchr("+-*/%", lastPrint) != NULL)
				{
					buffer[i] = '\0';
					i++;
					appendString(tokens, buffer, i);
					lastPrint = '\0';
					i = 0;
				}
			}

			/* add the default character */
			buffer[i] = c;
			i++;
			lastPrint = c;
		}

	}while(c != EOF);

	/* free all the crap we were using */
	free(buffer);
	fclose(fin);

	return tokens;
}

int appendString(Vector* v, char* string, int i)
{
	char* nstr;

	if(v == NULL || v->contents == NULL || string == NULL)
	{
		return -1;
	}

	/* this ensures real strings are being placed into the vector. */
	if(string[0] != '\0' && i > 0)
	{
		nstr = malloc(sizeof(char) * i + 1);
		strcpy(nstr, string);
		append(v, nstr);
	}
	else
	{
		return 1;
	}

	return 0;
}

void printProgram(Vector* tokens)
{
	int i;
	int tabLevel;

	tabLevel = 0;

	if(tokens == NULL)
	{
		return;
	}

	/* loop through the entirety of the tokens, essentially. */
	for(i = 0; i < tokens->length; i++)
	{
		int j;

		/* these are special characters for printing.  They call for special newline rules */
		if(strstr("{};", (char*) tokens->contents[i]) != NULL)
		{
			if(i + 1 < tokens->length && strcmp(tokens->contents[i+1], ";") == 0)
			{
				printf("%s", (char*) tokens->contents[i]);
			}
			else
			{
				printf("%s\n", (char*) tokens->contents[i]);
			}
			if(strcmp((char*) tokens->contents[i], "{") == 0)
			{
				tabLevel ++;
			}
			else if(strcmp(tokens->contents[i], "}") == 0)
			{
				tabLevel --;
			}

			/* this ensures proper printing levels for the next couple statements. */
			if(i + 1 < tokens->length && strcmp(tokens->contents[i+1], "}") == 0)
			{
				for(j = 0; j < tabLevel - 1; j++)
				{
					printf("    ");
				}
			}
			else
			{
				for(j = 0; j < tabLevel; j++)
				{
					printf("    ");
				}
			}
		}
		else if(strstr(tokens->contents[i], "//") != NULL || strstr(tokens->contents[i], "/*") != NULL)
		{
			printf("%s\n", (char*) tokens->contents[i]);
			if(i + 1 < tokens->length && strcmp(tokens->contents[i+1], "}") == 0)
			{
				for(j = 0; j < tabLevel - 1; j++)
				{
					printf("    ");
				}
			}
			else
			{
				for(j = 0; j < tabLevel; j++)
				{
					printf("    ");
				}
			}
		}
		/* this ensures that preprocessors are printed on seperate lines. */
		else if(strstr((char*) tokens->contents[i], "#include") != NULL)
		{
			printf("%s\n", (char*) tokens->contents[i]);
		}

		else
		{
			printf("%s ", (char*)tokens->contents[i]);
		}
	}
	return;
}
