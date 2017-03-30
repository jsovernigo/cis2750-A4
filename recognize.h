/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 * 	this file contains functions that are used to identify special patterns
 * 	that occur in c/cpp files, such as function calls/declarations, variables,
 * 	class declarations, and other things.
 */
#ifndef __GSOVERNIGO_RECOGNIZE_H__
#define __GSOVERNIGO_RECOGNIZE_H__

#include"vector.h"

/**
 *	nstrchr
 *	returns the number of characters character, which are containted in string.
 *	Please note, the nomenclature for this function comes from string.h's family
 *	of strchr, strstr, strnchr, and strnstr functions, used to find characters
 *	IN:			string			- the string that will be examined for character
 *				character		- the character to be matched.
 *	OUT:		the number of characters character that are found in string, or
 *				should an error occur, -1 shall be returned instead.
 *	POST:		none.
 *	ERRO:		string is null, signaled by return of -1.
 */
int nstrchr(char* string, char charac);

/**
 *	isValidName
 *	checks the current name for conformity to the c standard 
 *	IN:			name			- the name that will be checked for conformity
 *	OUT:		return 1 if the name is valid, 0 otherwise.
 *	POST:		integer returned.
 *	ERROR:		name is null.
 */
int isValidName(char* name);

/**
 * 	getTypeID
 * 	gets the character id for the variable that is located at position pos in
 * 	tokens.
 * 	IN:			tokens			- the tokens that make up the program
 * 				pos				- the position we are starting at
 * 	OUT:		a single char, representing the type of the variable
 * 	POST:		a character has been returned
 *	ERROR:		tokens are null, or pos does not point to a location that does
 *				not define a variable
 */
char getTypeID(Vector* tokens, int pos);

/**
 *	getType
 *	gets the type of the variable we are located at.
 *	IN:			tokens			- the tokens that we are looking through.
 *				pos				- the position we are located at right now.
 *	OUT:		a malloced string that represents the type of the variable at pos
 *	POST:		the returned string should be destroyed or used somehow.
 *	ERROR:		tokens is null, pos does not point to a variable
 */
char* getType(Vector* tokens, int pos);

/**
 *	isValidType
 *	checks if the type is valid, returns true if so.  Checks restricted keywords
 *	IN:			type			- the type string that is to be checked
 *	OUT:		-1 if there is an error, 1 if the type is an *atomic*, 2 if it is
 *				a valid prefix, 3 if it is a class.
 *	POST:		an integer has been returned to the calling function.
 *	ERROR:		type is null.
 */
int isValidType(char* type);

/**
 *	isClass
 *	checks preceeding and following tokens to ensure the current index is a
 *	class declaration/definition (essentially the same thing at this point)
 *	IN:			tokens			- the tokens vector that will be checked
 *				pos				- the position of the vector that we will check
 *	OUT:		1 if the current token is a class, o otherwise
 *	POST:		integer returned.
 *	ERROR		tokens null, pos invalid.
 */
int isClass(Vector* tokens, int pos);

/**
 *	isFunction
 *	checks if the tokens at pos represents a function
 *	IN:			tokens			- a vector filled with the program tokens.
 *				pos				- a position that should represent a function
 *	OUT:		1 if the function is valid, 0 if not
 *	POST:		number returned, check it
 *	ERROR:		tokens is null, pos is invalid
 */
int isFunction(Vector* tokens, int pos);

/**
 * 	isFunctionCall
 * 	checks if the tokens at pos represents a call to a function
 * 	IN:			tokens			- the tokens filled with the program
 * 				pos				- the position we are checking.
 * 	OUT:		1 if valid, 0 if not.
 * 	POST:		number returned.
 * 	ERROR:		tokens is null, pos is invalid
 */
int isFunctionCall(Vector* tokens, int pos);

/**
 *	isMemberFunction
 *	checks if the tokens at pos represents a member function
 *	IN:			tokens			- a vector filled with the program tokens.
 *				pos				- a position that should represent a function
 *	OUT:		1 if the function is valid, 0 if not
 *	POST:		number returned, check it
 *	ERROR:		tokens is null, pos is invalid
 */
int isMemberFunctionCall(Vector* tokens, int pos);

/**
 *	isFunctionPointer
 *	checks if the tokens at pos represents a function pointer
 *	IN:			tokens			- a vector filled with the program tokens.
 *				pos				- a position that should represent a function
 *	OUT:		1 if the function is valid, 0 if not
 *	POST:		number returned, check it
 *	ERROR:		tokens is null, pos is invalid
 */
int isFunctionPointer(Vector* tokens, int pos);

/**
 *	beginsFunction
 *	checks if the tokens at pos represents the beginning of a function
 *	IN:			tokens			- a vector filled with the program tokens.
 *				pos				- a position that should represent a function
 *	OUT:		1 if the function is valid, 0 if not
 *	POST:		number returned, check it
 *	ERROR:		tokens is null, pos is invalid
 */
int beginsFunction(Vector* tokens, int pos);

/**
 *	isVariable
 *	checks if the tokens at pos represents a variable
 *	IN:			tokens			- a vector filled with the program tokens.
 *				pos				- a position that should represent a function
 *	OUT:		1 if the function is valid, 0 if not
 *	POST:		number returned, check it
 *	ERROR:		tokens is null, pos is invalid
 */
int isVariable(Vector* tokens, int pos);

/**
 *	isVariableDeclaration
 *	checks if the tokens at pos represents a variable declaration
 *	IN:			tokens			- a vector filled with the program tokens.
 *				pos				- a position that should represent a function
 *	OUT:		1 if the function is valid, 0 if not
 *	POST:		number returned, check it
 *	ERROR:		tokens is null, pos is invalid
 */
int isVariableDeclaration(Vector* tokens, int pos);

/**
 *	isClassVariable
 *	checks if the tokens at pos represents a class variable
 *	IN:			tokens			- a vector filled with the program tokens.
 *				pos				- a position that should represent a function
 *	OUT:		1 if the function is valid, 0 if not
 *	POST:		number returned, check it
 *	ERROR:		tokens is null, pos is invalid
 */
int isClassVariable(Vector* tokens, int pos);

#endif
