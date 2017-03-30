/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 *	This file contains functions that are designed to operate on class-to-
 *	struct situations in a cc file, where the desided outcome is to produce
 *	a working c file.
 *
 *	These functions aid in that transition.
 */
#ifndef __GSOVERNIGO_CLASSTOSTRUCT_H__
#define __GSOVERNIGO_CLASSTOSTRUCT_H__

#include"vector.h"

/**
 *	mangle
 *	this function mangles the names of functions, so that they can be used as
 *	function pointers in a struct.
 *	IN:			className			- the name of the class that contains the
 *									  member functionName function.
 *				functionName		- the name of the function to be mangled.
 *									  the function name should not contain
 *									  the return type.
 *				args				- the args string, which contains all the
 *									  args terms.
 *	OUT:		a mangled, malloced name that can be used as a new function
 *				pointer name.  The new name shall conform to:
 *
 *				class math
 *				{
 *					int add(int a, int b)
 *					{
 *						return a + b;
 *					}
 *				}
 *
 *				becomes:
 *
 *				struct math
 *				{
 *					int (*mathaddii)(int a, int b);
 *				}
 *
 *				int mathaddii(int a, int b)
 *				{
 *					return a + b;
 *				}
 *	POST:		a new malloced name has been returned, ensure it is freed.
 *	ERROR:		if malloc fails, or either of the two strings passed in are null
 */
char* mangle(char* className, char* functionName, char* args);

/**
 * 	replaceClass
 * 	replaces the word class in the program with struct.
 * 	IN:			program			- the program struct that we will be modifying.
 * 	OUT:		numReplaced		- the number of items we replaced in the program
 * 	POST:		all instances of the string "class" will be replaced by "struct"
 * 	ERROR:		if program is null, -1 will be returned for numReplaced.
 */
int replaceClass(Vector* tokens);

/**
 * 	addSelfReferences
 * 	adds a "this->" pointer to all class members that are located in the member
 * 	functions in the program.
 * 	IN:			tokens			- the tokens vector that holds the program
 * 	OUT:		the number of "this->" pointers entered into the program
 *	POST:		this-> has been added to class variables in member functions.
 *	ERROR:		tokens is null
 */
int addSelfReferences(Vector* tokens);

/**
 *  distributeThis
 * 	Distributes the various "this->" or "struct <sname> * this" tokens that are
 * 	needed to create a self reference for member functions which have been moved.
 * 	IN:			tokens			- the program tokens in a vector
 * 				className		- the class name we are operating on.
 * 				classStart		- the start of the class body
 * 				classStop		- the stop position of the class.
 * 	OUT:		0 on success, error code on failure.
 * 	POST:		self references have been added to this particular class.
 *
 */
int distributeThis(Vector* tokens, char* className, int classStart, int classStop);

/**
 *	getParams
 *	gets the parameters in the function definition
 *	IN:			tokens			- the tokens of the program.
 *				classStart		- the starting index of the class.
 *	OUT:		a vector, containing the parameter names for the function.
 *	POST:		a vector has been returned to the caller, so you better use it.
 *	ERROR:		tokens is null, or classStart is NOT a class start...
 */
Vector* getParams(Vector* tokens, int classStart);

/**
 *	addThisArg
 *	adds the "this" argument to functions that were relocated outside of their
 *	original classes, to allow access to the current calling instance of the struct.
 *	IN:			tokens			- the tokens of the program, stored in a vector.
 *  			className		- the class name of the function, in a string
 *				fStart			- the starting point of the function
 *				bStart			- the start of the function body (end of head)
 *	OUT:		returns 0 on success.
 *	POST:		this has been added to the function parameters defined by fstart
 *				and bstop
 *	ERROR:		any are null/invalid indexes
 */
int addThisArg(Vector* tokens, char* className, int fStart, int bStart);

/**
 *	addThisRef
 *	adds 'this' references to the functions, so that class variables can be
 *	accessed from other functions.
 *	IN:			tokens			- the tokens of the program
 *				classVariables	- a vector of the class variables.
 *				params			- the function parameters.
 *				fstart			- the starting index of the function.
 *				fstop			- the stopping index of the function.
 *	OUT:		returns the number of substitutions that occurred in the class.
 *	POST:		this-> has been added to all classVariables occurring in tokens.
 */
int addThisRef(Vector* tokens, Vector* classVariables, Vector* params, int fstart, int fstop);

/**
 *	mangleAllMembers
 *	mangles the names of all member function calls in all classes in the program.
 *	IN:			tokens			- the vector of tokens in the program.
 *	OUT:		0 on success.
 *	POST:		all member functions have been mangled.
 *	ERROR:		tokens is NULL.
 */
int mangleAllMembers(Vector* tokens);

/**
 *	migrateFunctions
 *	moves the functions found in in classes out into program scope
 *	IN:			tokens			- the vector containing the program tokens.
 *	OUT:		void
 *	POST:		the functions found in classes in tokens are moved above main.
 *	ERROR:		tokens is NULL
 */
void migrateFunctions(Vector* tokens);

/**
 * 	createAllConstructors
 *	creates all the constructors for all classes, inserting the above main()
 *	IN:			tokens			- the tokens of the program, held in a vector.
 *	OUT:		0 on success, error code (positive or negative) on failure
 *	POST:		several tokens have been inserted above main, which represent
 *				constructors for the structs that we have created.
 *	ERROR:		tokens is null
 */
int createAllConstructors(Vector* tokens);

/**
 * 	createAllConstructors
 * 	creates, for all classes present in the file, a constructor that assigns
 * 	their function pointer member variables to the memory addresses of their
 * 	declared counterparts now located in program scope outside the callers.
 * 	IN:			tokens			- the tokens vector that holds the program
 * 	    		pos				- the position of the class name declaration
 *	OUT:		a char* that represents a formatted constructor which will
 *				populate the function pointer member variables in the class.
 *	POST:		a char* has been returned, and should be inserted into a class.
 *	ERROR:		pos is invalid; tokens in null.
 */
char* makeConstructor(Vector* tokens, int pos);

/**
 *	placeConstructors
 *	places constructors following all declarations in outer functions.
 *	IN:			tokens			- the tokens of the program held in a vector.
 */
int placeConstructors(Vector* tokens);

/**
 *	fixOuterFunctions
 *	corrects function calls in non-member functions, allowing the migrated
 *	functions to be linked successfully with their actual calls.
 *	IN:			tokens			- the tokens of the program held in a vector.
 *	OUT:		
 */
int fixOuterFunctions(Vector* tokens);

/**
 *	modifyFunctionCall
 *	modifies a function call, causing it to conform to its calling context
 *	IN:			tokens			- the tokens of the program held in a vector.
 *				pos				- the position we are starting at.
 *	OUT:		0 on success.
 *	POST:		tokens->contents[pos] has been mangled to fit its calling.
 *	ERROR:		tokens is null, or pos does not point to a function call.
 */
int modifyFunctionCall(Vector* tokens, int pos);

#endif
