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
#include"vector.h"
#include"parseFile.h"
#include"classToStruct.h"
#include"recognize.h"
#include"driver.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void printVector(Vector* v, FILE* out)
{
	int i;

	for(i = 0; i < v->length; i++)
	{
		fprintf(out, "%s", (char*)v->contents[i]);
	}
}

int run(char* fname)
{
	char outfname[512];

	FILE* out;
	Vector* tokens;

	strcpy(outfname, fname);
	outfname[strlen(outfname) - 1] = '\0';

	out = fopen(outfname, "w");
	if(out == NULL)
	{
		printf("Error in opening output file %s\n", outfname);
		return 1;
	}

	tokens = getTokens(fname);
	if(tokens == NULL)
	{
		printf("File was invalid, could not read.");
		fclose(out);
		return 1;
	}

	mangleAllMembers(tokens);
	addSelfReferences(tokens);
	placeConstructors(tokens);

	fixOuterFunctions(tokens);
	migrateFunctions(tokens);
	createAllConstructors(tokens);

	replaceClass(tokens);

	printVector(tokens, out);
	destroyVector(tokens);

	fclose(out);

	return 0;
}
