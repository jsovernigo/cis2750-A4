/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 *	this file contains the library API for the stream lib.
 *	These functions are used to modify stream files in messages/
 */
#ifndef __GSOVERNIGO_STREAM_H__
#define __GSOVERNIGO_STREAM_H__

#include <mysql/mysql.h>

struct userPost
{
	char* username;
	char* streamname;
	char* date;
	char* text;
};


/**
 *
 */
char* compdate();

/**
 * 	updateStream
 * 	Posts a userpost to the file identified in it.
 *	IN:			st			- the userpost struct we want to post
 *							  to.
 *	OUT:		returns 0 on success.
 *	POST:		st has been written to its appropriate stream file in messages/
 *	ERROR:		st is null, or any of their fields are invalid or null
 */
int updateStream(struct userPost* st);

/**
 *	addUser
 *	adds a user to a stream file.  files are called messages/<stname>StreamUsers
 *	IN:			username	- a username we will add
 *				list		- a list of streams to be added to.
 *	OUT:	returns 0 on success.
 *	POST:	username has been added to all files specified by list.
 *	ERROR:	list contains references to files which are too privilledged.
 */
int addUser(char* username, char* list);

/**
 *	removeUser
 *	removes a user from the files specified in list.
 *	IN:			username	- the username we will be removing.
 *				list		- the list of streams to remove the user from.
 *	OUT:		returns 0 on success.
 *	POST:		username has been removed from all files named in list.
 *	ERROR:		username does not exist in the file, or list contains references
 *				to streams that we cannot access.
 */
int removeUser(char* username, char* list);

/**
 *	checkIfPresent
 *	checks if a word is present inside the file named in fname.
 *	IN:			fname		- the file name we will be checking.
 *				word		- the word to search for.
 *	OUT:		1 if word is present in file, 0 if not.
 *	POST:		integer returned.
 *	ERROR:		fname is invalid or null, word is null.
 */
int checkIfPresent(char* fname, char* word);

#endif
