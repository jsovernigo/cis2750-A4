/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *	
 *	this file contains code that allows the instantiation of a vector adt.
 */
#include"vector.h"
#include<stdlib.h>

Vector* makeVector(int size)
{
	Vector* vec;

	vec = malloc(sizeof(Vector));
	if(vec == NULL)
	{
		return NULL;
	}

	vec->csize = size;
	vec->length = 0;

	vec->contents = malloc(sizeof(void*) * size);
	if(vec->contents == NULL)
	{
		free(vec);
		return NULL;
	}

	return vec;
}

void destroyVector(Vector* vec)
{
	int i;

	if(vec == NULL)
	{
		return;
	}
	else if(vec->contents == NULL)
	{
		free(vec);
		return;
	}

	for(i = 0; i < vec->length; i++)
	{
		free(vec->contents[i]);
	}
	free(vec->contents);
	free(vec);

	return;
}

void doublesize(Vector* vector)
{
	int nsize;
	int i;

	void** ncontents;

	if(vector == NULL || vector->contents == NULL)
	{
		return;
	}

	nsize = vector->csize*2;
	ncontents = malloc(sizeof(void*) * nsize);
	if(ncontents == NULL)
	{
		return;
	}
	else
	{
		vector->csize = nsize;
	}

	for(i = 0; i < vector->length; i++)
	{
		ncontents[i] = vector->contents[i];
	}

	free(vector->contents);
	vector->contents = ncontents;

	return;
}



int insert(Vector* vec, void* v, int pos)
{
	int i;

	if(vec == NULL || v == NULL)
	{
		return -1;
	}
	if(pos > vec->length || pos < 0)
	{
		return -1;
	}

	if(vec->length == vec->csize)
	{
		doublesize(vec);
	}

	vec->length ++;

	for(i = vec->length - 1; i > pos; i--)
	{

		vec->contents[i] = vec->contents[i-1];
	}

	vec->contents[pos] = v;
	return 0;
}

int append(Vector* vec, void* v)
{
	if(vec == NULL || v == NULL)
	{
		return -1;
	}

	if(vec->length == vec->csize)
	{
		doublesize(vec);
	}

	/* we are adding an item to the array... */
	vec->length ++;

	/* adds v to the end of the array */
	vec->contents[vec->length-1] = v;

	return 0;
}

int push(Vector* vec, void* v)
{
	if(vec == NULL || v == NULL)
	{
		return -1;
	}

	if(vec->length == vec->csize)
	{
		doublesize(vec);
	}

	insert(vec, v, 0);

	return 0;
}

void* pop(Vector* vec)
{
	if(vec == NULL)
	{
		return NULL;
	}
	else if(vec->contents == NULL)
	{
		return NULL;
	}

	return removeAt(vec, 0);
}

void* removeAt(Vector* vec, int pos)
{
	int i;
	void* p;

	if(vec == NULL)
	{
		return NULL;
	}
	if(vec->contents == NULL)
	{
		return NULL;
	}

	if(pos < 0 || pos >= vec->length)
	{
		return NULL;
	}

	p = vec->contents[pos];

	for(i = pos; i < vec->length - 1; i++)
	{
		vec->contents[i] = vec->contents[i+1];
	}
	vec->contents[vec->length - 1] = NULL;
	vec->length --;

	return p;
}
