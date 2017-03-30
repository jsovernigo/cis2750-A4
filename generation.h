/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 * 	this file contains source for an html generator. 
 */
#ifndef __GSOVERNIGO_GENERATION_H__
#define __GSOVERNIGO_GENERATION_H__

/**
 *	generatePage
 *	generates a full page, given a file name
 *	IN:			fname			- the file name we will be reading.
 *	OUT:		void
 *	POST:		a whole php/html page will be printed to the page.
 *	ERROR:		fname does not point to a valid file.
 */
void generatePage(char* fname);

/**
 *	generateTag
 *	generates a tag that is described by configtag.
 *	IN:			configTag		- a wpml tag that needs to be turned into html
 *	OUT:		produces an html tag.
 *	POST:		none
 *	ERROR:		configtag is null.
 */
char* generateTag(char* configTag);

/**
 *	getValue
 *	gets a value from an attribute that is used to create an html tag.
 *	IN:			attribute		- an attribute that is like: value="<value"
 *	OUT:		returns a malloced version of the attribute's value.
 *	POST:		none
 *	ERROR:		malloc failes, attribute is null.
 */
char* getValue(char* attribute);

/**
 *	splitTagLine
 *	splits a wpml tag line by the period delimiters.
 *	IN:			tagLine			- the period separated tag line.
 *	OUT:		a string array that contains the tags for this line.
 *	POST:		none
 *	ERROR:		tagline is null, malloc fails.
 */
char** splitTagLine(char* tagLine);

/**
 *	getArgs
 *	gets the args from an attribute tag.
 *	IN:			wholetag		- a whole tag, with attributes inside.
 *	OUT:		produces a string array of all attributes.
 *	POST:		none
 *	ERROR:		malloc failes, and wholetag is null.
 */
char** getArgs(char* wholeTag);

#endif
