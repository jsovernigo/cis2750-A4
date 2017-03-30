/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *	
 *	this file contains code that allows the instantiation of a vector adt.
 */
#ifndef __GSOVERNIGO_VECTOR_H__
#define __GSOVERNIGO_VECTOR_H__

struct vector
{
	int length;
	int csize;
	void** contents;
};

typedef struct vector Vector;

/**
 *	makeVector
 *	makes a new vector to be used to store things.
 *	IN:			size		- the size of the vector to be created.  Note that this is more
 *							  like a suggestion, as the size is variable.  It can be used
 *							  to speed up the insertion of initial elements into the vector
 *	OUT:		a new vector that you can use to store anything.
 *	POST:		a new vector has been returned.  Use destroyVector when you're done.
 *	ERROR:		malloc fails
 */
Vector* makeVector(int size);

/**
 * 	destroyVector
 * 	destroys a vector struct completely, so that no memory leaks do not occur.
 * 	IN:			vec			- a vector struct to be destroyed
 *	OUT:		void
 *	POST:		vec has been deallocated, so use it with care.
 *	ERROR:		vec is a dangling pointer.
 */
void destroyVector(Vector* vec);

/**
 *	doublesize
 *	doubles the size of a vector (reallocates the array.)
 *	IN:			vector		- the vector that will have its size doubled (usually the result
 *							  of an add to an already full array.
 *	OUT:		void
 *	POST:		vector->csize has been increased, and the array has been doubled in length
 *				and also copied over so as to perserve its information
 *	ERROR:		vector or vector->contents are null.
 */
void doublesize(Vector* vector);

/**
 *	insert
 *	inserts a void pointer at location pos in the vector.  This is used by other functions
 *	also so as to provide a universal interface for those other functions.
 *	IN:			vec			- the vector to be added to
 *				v			- the void pointer we want to add
 *				pos			- the position we want to add it to.
 *	OUT:		-1 on error, 0 on success, positive error code on failure
 *	POST:		v has been added to vec, and vec->length has increased.
 *	ERROR:		any are null, pos is invalid.
 */
int insert(Vector* vec, void* v, int pos);

/**
 *	append
 *	appends the void pointer v to the vector vec.
 *	IN:			vec			- the vector to be added to
 *				v			- the void pointer we want to add to vec.
 *	OUT:		-1 on error, 0 on success, positive error code on failure
 *	POST:		v has been added to vec.
 *	ERROR:		vec or v are null
 */
int append(Vector* vec, void* v);

/**
 *	push
 *	pushes void pointer v to the front of vector vec.
 *	IN:			vec			- the vector to be added to
 *	    		v			- the void pointer that will be added to the vector vec.
 *	OUT:		-1 on error, 0 on success, positive error code on failure.
 *	POST:		v has been added to vec, and vec->length has been updated.
 *	ERROR:		vec or v are null or dangling.
 */
int push(Vector* vec, void* v);

/**
 *	pop
 *	returns the void pointer at vec->contents[0], and also removes it from the vector.
 *	IN:			vec			- the vector to be popped from
 *	OUT:		the void pointer that (was) at vec->contents[0]
 *	POST:		vec->contents has been updated; vec->length has been updated;
 *				vec->contents[0] returned.
 *	ERROR:		vec is null or has no items to be returned.
 */
void* pop(Vector* vec);

/**
 *	removeAt
 *	removes the void pointer and returns it as it occurs in vec->contents[i]
 *	IN:			vec			- the vector that will be modified
 *				i			- the index that will be selected
 *	OUT:		returns the item that was formerly at position i in the vector.
 *	POST:		vec is shorter, and a void pointer has been returned.
 *	ERROR:		vec is null, i is invalid.
 */
void* removeAt(Vector* vec, int i);

#endif
