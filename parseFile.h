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
#ifndef __GSOVERNIGO_PARSEFILE_H__
#define __GSOVERNIGO_PARSEFILE_H__

#include"vector.h"

/**
 *  getTokens
 *  gets the tokens (program independent terms) from the passed in cc file, and
 *  organizes them into a vector struct.
 *  IN:			fname			- the name of the file that will be tokenized.
 *  OUT:		tokens			- the token vector that was derived from the file.
 *  POST:		a vector has been passed back, full of malloced strings.
 *  ERROR:		fname is an invalid file (null returned)
 */
Vector* getTokens(char* fname);

/**
 * 	appendString
 *	takes string and the strlen of that string, and mallocs a new string, also
 *	appending it to the end of vector v.
 *	IN:			v				- the vector to be appended to.
 *				string			- the string that will be malloced and appeneded
 *				i				- the strlen of the string that was passed in.
 *	OUT:		-1 on error, 0 on success, 1 on non-populated string
 *	POST:		string has been copied into v.
 *	ERROR:		if malloc fails, or any passed in arguments are null/invalid
 */
int appendString(Vector* v, char* string, int i);

/**
 *	printProgram
 *	prints the program out, in a formatted way. (traditional brackets and tabbing)
 *	IN:			tokens			- a vector of the tokens that are to be printed.
 *	OUT:		void; no output produced.
 *	POST:		tokens has been printed to the screen/file
 *	ERROR:		tokens in null.
 */
void printProgram(Vector* tokens);

#endif
