/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 *	this file contains the run() method, and a simple printing function
 *	for vectors.
 */
#ifndef __GSOVERNIGO_DRIVER_H__
#define __GSOVERNIGO_DRIVER_H__

#include"vector.h"
#include<stdio.h>

/**
 *	printVector
 *	prints a vector as a string vector, with no separation.
 *	IN:			v			- the vector to be printed.
 *				out			- a file pointer to the required output.
 *	OUT:		void
 *	POST:		v has been printed to stdout
 *	ERROR:		v is null.
 */
void printVector(Vector* v, FILE* out);

/**
 *	run
 *	runs the main program.  calls the functions in order.
 *	IN:			fname		- the file name we are going to read
 *	OUT:		0 on success.
 *	POST:		<fname>.c has been created and written to.
 *	ERROR:		fname points to an invalid file path.
 */
int run(char* fname);

#endif
