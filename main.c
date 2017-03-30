/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 *	this file contains the main method.  This program is intended to
 *	convert cppL files to C files (see CFront for more info on the concept
 *	of converting cpp to c)
 */
#include"driver.h"
#include<stdio.h>

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("No file name given.\n");
		return 0;
	}
	
	run(argv[1]);

	return 0;
}
