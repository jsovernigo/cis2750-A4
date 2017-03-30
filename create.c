#include "generation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_NO_ARGS "<html><body><h1>Whoops!<br><h3>Something went wrong.<br>No page name was given to be generated!</body></html>"

int main(int argc, char** argv)
{
	char fname[512];

	if(argc < 2)
	{
		puts(ERROR_NO_ARGS);
		return -1;
	}
	else
	{
		strcpy(fname, argv[1]);
	}

	generatePage(fname);

	return 0;
}
