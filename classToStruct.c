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
#include "classToStruct.h"
#include "recognize.h"
#include "parseFile.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* mangle(char* className, char* functionName, char* args)
{
	unsigned int i;

	int numberOfArgs;
	int finalLength;
	int carg;
	int record;

	char* finalName;
	char* argLetters;

	if(className == NULL || functionName == NULL || args == NULL)
	{
		return NULL;
	}

	carg = 0;
	record = 1;
	numberOfArgs = nstrchr(args, ',') + 1;


	/*
	   essentially, this creates the length of the new name, which will be:
	   the original name, preceeded by the class name, followed by the first letter
	   of all the parameters.
	   */
	finalLength = strlen(functionName) + strlen(className) + numberOfArgs;

	finalName = malloc(sizeof(char) * finalLength + 1);
	if(finalName == NULL)
	{
		return NULL;
	}
	argLetters = malloc(sizeof(char) * numberOfArgs + 1);
	if(argLetters == NULL)
	{
		free(finalName);
		return NULL;
	}

	for(i = 0; i < strlen(args); i++) /* loop through the function name */
	{
		if(record == 1 && isalpha(args[i])) /* if we are currently seeking an argument first letter, and it is indeed a letter */
		{
			argLetters[carg] = args[i]; /* get the argument */
			carg++;
			record = 0;
		}
		else if(args[i] == ',') /* else, if we are starting a new argument */
		{
			record = 1;
			continue;
		}
	}
	argLetters[carg] = '\0'; /* ensure a null terminator after last argument. */


	/* this creates the final name out of the three parts. */
	strcpy(finalName, className);
	strcat(finalName, functionName);
	strcat(finalName, argLetters);

	/* strcpy'd, so who cares about this now. */
	free(argLetters);

	return finalName;
}

int replaceClass(Vector* tokens)
{
	int i;
	int numReplaced;

	if(tokens == NULL)
	{
		return -1;
	}

	numReplaced = 0;

	for(i = 0; i < tokens->length-1; i++) /* loop through the lines (tokens) of the program */
	{
		/* if we have found a token 'class' */
		if(strcmp((char*)tokens->contents[i], "class") == 0)
		{
			free(tokens->contents[i]);
			tokens->contents[i] = malloc(sizeof(char) * 7);
			strcpy((char*)tokens->contents[i], "struct");
			numReplaced ++;
		}
	}

	return numReplaced;
}

int addSelfReferences(Vector* tokens)
{
	int i;
	int bracketCounter;

	char* className;

	if(tokens == NULL)
	{
		return -1;
	}
	for(i = 0; i < tokens->length; i++)
	{
		/* this means we have found a class who's members need 'this' support added to them. */
		if(isClass(tokens, i))
		{
			int j;
			int cStart;
			int cStop;
			int foundBracket;
			cStart = i;

			bracketCounter = 0;

			className = tokens->contents[i];

			foundBracket = 0;

			for(j = i; j < tokens->length; j++)
			{
				if(strcmp(tokens->contents[j], "{") == 0)
				{
					foundBracket = 1;
					bracketCounter ++;
				}
				/* if we find a closing brace */
				else if(strcmp(tokens->contents[j], "}") == 0)
				{
					bracketCounter --;
					if(foundBracket == 1 && bracketCounter == 0)
					{
						cStop = j;
						break;
					}
				}
			}
			/* operates on each class individually, distributing the self pointer */
			distributeThis(tokens, className, cStart, cStop);
		}
	}
	return 0;
}

int distributeThis(Vector* tokens, char* className, int classStart, int classStop)
{
	int i;
	int bracketLevel;
	Vector* classVariables;

	bracketLevel = 0;
	classVariables = makeVector(7);

	/* collect class variables first */
	for(i = classStart; i < classStop; i++)
	{
		if(strcmp(tokens->contents[i], "{") == 0)
		{
			bracketLevel ++;
		}
		else if(strcmp(tokens->contents[i], "}") == 0)
		{
			bracketLevel --;
		}

		if(isFunction(tokens, i))
		{
			break;
		}

		if(bracketLevel == 1 && isVariable(tokens,i))
		{
			appendString(classVariables, tokens->contents[i], strlen(tokens->contents[i]));
		}
	}

	/* now modify the member functions that reference the class variables. */
	for(i = classStart; i < classStop; i++)
	{
		/* this is a member function if we enter this block */
		if(isFunction(tokens, i))
		{
			int j;
			int fStart;
			int fStop;
			int bStart;
			int foundBracket;
			int bracketCounter;

			Vector* functionArgs;

			fStart = i;
			bracketCounter = 0;
			foundBracket = 0;

			/* loop through the tokens from this point and store the function starts and stops */
			for(j = i; j < classStop; j++)
			{
				if(strcmp(tokens->contents[j], "{") == 0)
				{
					if(foundBracket == 0)
					{
						bStart = j;
						foundBracket = 1;
					}
					bracketCounter ++;
				}
				else if(strcmp(tokens->contents[j], "}") == 0)
				{
					bracketCounter --;
					if(foundBracket == 1 && bracketCounter == 0)
					{
						fStop = j;
						break;
					}
				}
			}
	
			functionArgs = getParams(tokens, fStart);

			/* this goes through the function body and replaces the class variables with this-> */
			addThisRef(tokens, classVariables, functionArgs, bStart, fStop);

			/* adds class <cname> * this to the function's arguments. */
			classStop += addThisArg(tokens, className, fStart, bStart);
			
			
			destroyVector(functionArgs);
		}
	}

	destroyVector(classVariables);
	return 0;
}

Vector* getParams(Vector* tokens, int funcStart)
{
	int i;

	Vector* argNames;

	argNames = makeVector(3);

	/* loops through the strings starting at the bracket after the class name. */
	for(i = funcStart + 1; i < tokens->length; i++)
	{
		/* if we found the end of the args */
		if(strcmp(tokens->contents[i], ")") == 0)
		{
			break;
		}
		if(isValidName(tokens->contents[i]))
		{
			/* we found a variable!! */
			appendString(argNames, tokens->contents[i], strlen(tokens->contents[i]));
		}
	}

	return argNames;
}

int addThisArg(Vector* tokens, char* className,  int fStart, int bStart)
{
	int i;
	int j;
	int argEnd;
	char* space;
	char* comma;
	char* classWord;
	char* cnameToken;
	char* pointerToken;
	char* thisToken;

	if(className == NULL || tokens == NULL)
	{
		return -1;
	}

	for(i = bStart; i >= 0; i--)
	{
		if(strcmp(tokens->contents[i], ")") == 0)
		{
			argEnd = i;
			break;
		}
	}

	/* allocs the various variables that will be used to create a "this" pointer */
	space = malloc(sizeof(char) * 2);
	comma = malloc(sizeof(char) * 3);
	classWord = malloc(sizeof(char) * 7);
	cnameToken = malloc(sizeof(char) * strlen(className) + 2);
	pointerToken = malloc(sizeof(char) * 5);
	thisToken = malloc(sizeof(char) * 6);

	/* adds the various terms to their strings */
	strcpy(space, " ");
	strcpy(comma, ", ");
	strcpy(classWord, "class");
	strcpy(cnameToken, className);
	strcpy(pointerToken, " * ");
	strcpy(thisToken, "this ");
	
	/* inserts the tokens into the vector */
	insert(tokens, thisToken, argEnd);
	insert(tokens, pointerToken, argEnd);
	insert(tokens, cnameToken, argEnd);
	insert(tokens, space, argEnd);
	insert(tokens, classWord, argEnd);

	/* if we have an arguement other than ours */
	for(j = argEnd - 1; j >= 0; j--)
	{
		/* if we find an open bracket before another argument term */
		if(strcmp(tokens->contents[j], "(") == 0)
		{
			/* we dont need a comma */
			free(comma);
			return 5;
		}
		else if(!isspace(((char*)tokens->contents[j])[0]))
		{
			/* we need to insert a comma to separate the arguments */
			insert(tokens, comma, j + 1);
			return 6;
		}
	}

	return 0;
}

int addThisRef(Vector* tokens, Vector* classVariables, Vector* params,  int fstart, int fstop)
{
	int i;
	int numAdded;

	Vector* localVars;

	localVars = makeVector(3);
	numAdded = 0;

	/* loop through the tokens that represent this class function */
	for(i = fstart; i < fstop; i++)
	{
		int j;
		/* this whole block here checks for local vars, so as to stop them being "this'd" */
		if(i - 1 >= 0)
		{
			if(isVariable(tokens, i))
			{
				appendString(localVars, tokens->contents[i], strlen(tokens->contents[i]));
			}
		}
		/* loop through the class variables we have found. */
		for(j = 0; j < classVariables->length; j++)
		{
			/* if the token we are on matches a classVariable we are on */
			if(strcmp(tokens->contents[i], classVariables->contents[j]) == 0)
			{
				int k;
				int taken;

				taken = 0;
				/* check if the token is found in the parameters for the function */
				for(k = 0; k < params->length; k++)
				{
					if(strcmp(tokens->contents[i], params->contents[k]) == 0)
					{
						taken = 1;
						break;
					}
				}
				/* check if this token is represented in the local vars... */
				for(k = 0; k < localVars->length; k++)
				{
					if(strcmp(tokens->contents[i], localVars->contents[k]) == 0)
					{
						taken = 1;
						break;
					}
				}
				/* if this flag is set, we know we have run into a local var or a param. */
				if(taken == 1)
				{
					break;
				}
				else if(isValidType(tokens->contents[i - 1]) == 0 && isValidName(tokens->contents[i - 1]) == 0)
				{
					char* vname;
					vname = malloc(sizeof(char) * strlen(tokens->contents[i]) + 7);
					vname[0] = '\0';

					/* prefix the name with this-> */
					strcpy(vname, "this->");
					strcat(vname, tokens->contents[i]);

					free(tokens->contents[i]);
					tokens->contents[i] = vname;
					numAdded ++;

					/* breaks out of the loop searching the classVariables */
					break;
				}
			}
		}
	}

	destroyVector(localVars);
	return numAdded;
}

int mangleAllMembers(Vector* tokens)
{
	int i;
	int inClass;
	int bracketCounter;
	char* className;

	Vector* classFunctions;

	/* loop through all tokens in the program */
	inClass = 0;
	bracketCounter = 0;

	for(i = 0; i < tokens->length; i++)
	{
		if(isClass(tokens, i))
		{
			classFunctions = makeVector(4);

			className = tokens->contents[i];
			inClass = 1;
		}
		if(inClass == 1)
		{
			if(strcmp(tokens->contents[i], "{") == 0)
			{
				bracketCounter ++;
			}
			else if(strcmp(tokens->contents[i], "}") == 0)
			{
				bracketCounter --;

				/* we have broken out of a class */
				if(bracketCounter == 0)
				{
					inClass = 0;
					destroyVector(classFunctions);
				}
			}
			else if(isFunction(tokens, i))
			{
				int j;
				char* fname;
				char* args;

				char* finalName;
				
				appendString(classFunctions, tokens->contents[i], strlen(tokens->contents[i]));
				
				fname = tokens->contents[i];
				args = malloc(sizeof(char)*512);
				if(args == NULL)
				{
					return -1;
				}
				args[0] = '\0';
				j = i+2;
				/* loop until we encounter a closing brace */
				while(strcmp(tokens->contents[j], ")") != 0)
				{
					/* this will only occur if there is a syntax error present in the file. (i.e. open bracket with no corresponding closed) */
					if(j >= tokens->length)
					{
						return -1;
					}
					/* we don't want to include the opening bracket... */
					if(strcmp(tokens->contents[j], "(") != 0)
					{
						/* this adds on another part of the parameters, and then as pace to delimit them. */
						strcat(args, (char*)tokens->contents[j]);
						strcat(args, " ");
					}
					j++;
				}
				finalName = mangle(className, fname, args);
				tokens->contents[i] = finalName;
				free(fname);
				free(args);
			}
			else if(isFunctionCall(tokens, i))
			{
				int j;
				int needsMangling;
				needsMangling = 0;

				for(j = 0; j < classFunctions->length; j++)
				{
					if(strcmp(tokens->contents[i], classFunctions->contents[j]) == 0)
					{
						needsMangling = 1;
						break;
					}
				}
				/* this actually mangles function calls. */
				if(needsMangling)
				{
					int hasArgs;
					char* finalName;
					char* thisPointer;

					finalName = malloc(sizeof(char)*256);
					finalName[0] = '\0';
					hasArgs = 0;

					strcat(finalName, className);
					strcat(finalName, tokens->contents[i]);

					/* loop through the following tokens, looking for the closing brace of the function */
					for(j = i + 1; j < tokens->length; j++)
					{
						char temp[2];
						temp[1] = '\0';
						if(strcmp(tokens->contents[j], ")") == 0)
						{
							char* thisParam;

							thisParam = malloc(sizeof(char) * 5);
							strcpy(thisParam, "this");
							insert(tokens, thisParam, j);

							if(hasArgs == 1)
							{
								char* comma;
								comma = malloc(sizeof(char) * 2);
								strcpy(comma, ",");
								insert(tokens, comma, j);	
							}

							break;
						}
						/* this means if tokens->contents is not present in "(," */
						else if(strstr("(,",tokens->contents[j]) == NULL)
						{
							hasArgs = 1;
							temp[0] = getTypeID(tokens, j);
							strcat(finalName, temp);
						}
					}
					free(tokens->contents[i]);
					tokens->contents[i] = finalName;

					/* insert a this pointer behind the function call */
					thisPointer = malloc(sizeof(char)*7);
					strcpy(thisPointer, "this->");

					insert(tokens, thisPointer, i);
				}
			}
		}
	}
	return 0;
}

void migrateFunctions(Vector* tokens)
{
	int i;
	int inClass;
	int bracketCounter;
	int foundBracket;
	int insertPos;
	Vector* allFunctionTokens;

	inClass = 0;
	bracketCounter = 0;
	foundBracket = 0;
	insertPos = 0;

	allFunctionTokens = makeVector(50);

	/* loop, looking for classes */
	for(i = 0; i < tokens->length; i++)
	{
		if(strcmp(tokens->contents[i], "class") == 0)
		{
			int j;
			for(j = i + 1; j < tokens->length; j++)
			{
				if(isClass(tokens, j))
				{
					inClass = 1;
					break;
				}
				/* basically, if there is anything other than a space before our class declaration, we are NOT in a class */
				else if(!isspace(((char*)tokens->contents[j])[0]))
				{
					inClass = 0;
					break;
				}
			}
		}
		/* if we are currently in a class, we need to record opening and closing brackets */
		if(inClass == 1)
		{
			if(strcmp(tokens->contents[i], "{") == 0)
			{
				if(foundBracket == 0)
				{
					foundBracket = 1;
				}
				bracketCounter++;
			}
			/* encountered a closed bracket, may be an end-of-class */
			else if(strcmp(tokens->contents[i], "}") == 0)
			{
				bracketCounter --;

				if(foundBracket == 1 && bracketCounter == 0)
				{
					inClass = 0;
					foundBracket = 0;
				}
			}
			/* we need to find the previous semicolon. */
			else if(beginsFunction(tokens, i))
			{
				int bFuncCounter;
				int foundBFunc;
				int offset;
				int j;

				Vector* fptrTokens;

				/* this will be inserted into the program */
				fptrTokens = makeVector(10);

				bFuncCounter = 0;
				foundBFunc = 0;

				/* loop until we run out of space, or we find an open block brace */
				for(j = i; j < tokens->length && strcmp(tokens->contents[j], "{") != 0; j++)
				{
					if(isFunction(tokens, j))
					{
						appendString(fptrTokens, "(*", 3);
					}
					appendString(fptrTokens, (char*)tokens->contents[j], strlen((char*) tokens->contents[j]));
					if(isFunction(tokens, j))
					{
						appendString(fptrTokens, ")", 2);
					}
				}
				appendString(fptrTokens, ";\n", 3);

				/* loops through the positions. */
				do
				{
					if(strcmp(tokens->contents[i], "{") == 0)
					{
						foundBFunc = 1;
						bFuncCounter ++;
					}
					else if(strcmp(tokens->contents[i], "}") == 0)
					{
						bFuncCounter --;
						if(foundBFunc == 1 && bFuncCounter == 0)
						{
							append(allFunctionTokens, removeAt(tokens, i));
							break;
						}
					}
					append(allFunctionTokens, removeAt(tokens, i));

				}while(bFuncCounter != 0 || foundBFunc == 0);
				appendString(allFunctionTokens, "\n", 2);

				offset = fptrTokens->length;

				/* inserts the new function Pointer tokens into their proper location */
				/*
				for(j = fptrTokens->length - 1; j >= 0; j--)
				{
					insert(tokens, removeAt(fptrTokens, j), i);
				}
				*/
				while(fptrTokens->length > 0)
				{
					if(fptrTokens->contents[fptrTokens->length - 1] != NULL)
					{
						insert(tokens, removeAt(fptrTokens, fptrTokens->length - 1), i);
					}
				}

				destroyVector(fptrTokens);
				fptrTokens = NULL;

				i += offset - 1;
			}
		}
	}

	for(i = 0; i < tokens->length; i ++)
	{
		/* if we have found main */
		if(isFunction(tokens, i) && strcmp(tokens->contents[i], "main") == 0)
		{
			int j;
			/* loop until we find "int", as this would be the starting function declaration */
			for(j = i - 1; j >= 0; j--)
			{
				if(beginsFunction(tokens, j))
				{
					insertPos = j;
					break;
				}	
			}
			/* backtrack then insert functions above main */
			if(insertPos != 0)
			{
				break;
			}
		}
	}	
	appendString(allFunctionTokens, "\n", 2);
	/* inserts the final tokens into the vector. */
	for(i = allFunctionTokens->length - 1; i >= 0; i--)
	{
		insert(tokens, removeAt(allFunctionTokens, i), insertPos);
	}
	destroyVector(allFunctionTokens);

	return;
}

int createAllConstructors(Vector* tokens)
{
	int i;

	for(i = 0; i < tokens->length; i++)
	{
		char* classConstructor;
		if(isClass(tokens, i))
		{
			int insertPos;
			int j;
			classConstructor = makeConstructor(tokens, i);

			insertPos = -1;

			/* loop to find main */
			for(j = i; j < tokens->length; j++)
			{
				if(beginsFunction(tokens, j))
				{
					int k;
					/* loop to find if the function we just found IS main... */
					for(k = j; k < tokens->length; k++)
					{
						if(isFunction(tokens, k))
						{
							if(strcmp(tokens->contents[k], "main") == 0)
							{
								insertPos = j;
								break;
							}
							else
							{
								break;
							}
						}
					}
				}
				if(insertPos != -1)
				{
					break;
				}
			}

			insert(tokens, classConstructor, insertPos);
		}
	}

	return 0;
}

char* makeConstructor(Vector* tokens, int pos)
{
	int i;
	int foundBracket;
	int bracketCounter;
	char* constructor;

	foundBracket = 0;
	bracketCounter = 0;

	constructor = malloc(sizeof(char)*1024);
	if(constructor == NULL)
	{
		return NULL;
	}

	constructor[0] = '\0';

	strcat(constructor, "void construct");
	strcat(constructor, tokens->contents[pos]);
	strcat(constructor, "(struct ");
	strcat(constructor, tokens->contents[pos]);
	strcat(constructor, "* this)\n{\n");

	/* loop through the class, grabbing the function pointers */
	for(i = pos; i < tokens->length; i++)
	{
		if(strcmp(tokens->contents[i], "{") == 0)
		{
			foundBracket = 1;
			bracketCounter ++;
		}
		else if(strcmp(tokens->contents[i], "}") == 0)
		{
			bracketCounter --;

			/* we have broken out of a class */
			if(bracketCounter == 0 && foundBracket == 1)
			{
				break;
			}
		}
		else if(isVariable(tokens, i))
		{
			int j;
			int addInit;
			char varInit[512];

			varInit[0] = '\0';
			strcpy(varInit, "\tthis->");
			addInit = 0;

			for(j = i; j < tokens->length && strcmp(tokens->contents[j], ";") != 0; j++)
			{
				strcat(varInit, tokens->contents[j]);
			}

			strcat(varInit, ";\n");
			
			for(j = 0; j < strlen(varInit); j++)
			{
				if(varInit[j] == '=')
				{
					addInit = 1;
					break;
				}
			}

			if(addInit == 1)
			{
				j = i + 1;
				while(strcmp((char*)tokens->contents[j], ";") != 0)
				{
					free(removeAt(tokens, j));
				}
				strcat(constructor, varInit);
			}
		}
		else if(isFunctionPointer(tokens, i))
		{
			/* if we have found a function pointer, add it to the constructor */
			strcat(constructor, "\tthis->");
			strcat(constructor, (char*) tokens->contents[i]);
			strcat(constructor, " = &");
			strcat(constructor, (char*) tokens->contents[i]);
			strcat(constructor, ";\n");
		}
	}

	strcat(constructor, "}\n");

	return constructor;
}

int placeConstructors(Vector* tokens)
{
	int i;
	int inClass;
	int bracketCounter;

	inClass = 0;
	bracketCounter = 0;

	for(i = 0; i < tokens->length; i++)
	{
		if(isClass(tokens, i))
		{
			inClass = 1;
		}
		else if(strcmp(tokens->contents[i], "{") == 0)
		{
			bracketCounter ++;
		}
		else if(strcmp(tokens->contents[i], "}") == 0)
		{
			bracketCounter --;
			if(bracketCounter == 0)
			{
				inClass = 0;
			}
		}

		/* this will capture only non-member functions */
		if(isFunction(tokens, i) && inClass == 0)
		{
			int j;
			int bracketLevel;

			bracketLevel = 0;

			for(j = i + 1; j < tokens->length; j++)
			{
				if(strcmp(tokens->contents[j], "{") == 0)
				{
					bracketLevel ++;
				}
				else if(strcmp(tokens->contents[j], "}") == 0)
				{
					bracketLevel --;
					if(bracketLevel == 0)
					{
						break;	
					}
				}
				else if(isClassVariable(tokens, j))
				{
					int k;
					char* varType;
					char* constructorCall;

					varType = getType(tokens, j);
					constructorCall = malloc(sizeof(char) * 512);

					strcpy(constructorCall, "\nconstruct");
					strcat(constructorCall, varType);
					strcat(constructorCall, "(&");
					strcat(constructorCall, tokens->contents[j]);
					strcat(constructorCall, ");\n");

					/* find a semicolon. */
					for(k = j + 1; k < tokens->length; k++)
					{
						if(strcmp(tokens->contents[k], ";") == 0)
						{
							break;
						}
					}
					insert(tokens, constructorCall, k + 1);

					free(varType);
				}
			}
		}
	}

	return 0;
}

int fixOuterFunctions(Vector* tokens)
{
	int i;
	int inClass;
	int bracketCounter;
	int inOuterFunc;

	inClass = 0;
	bracketCounter = 0;
	inOuterFunc = 0;

	/* loop, looking for outer classes. */
	for(i = 0; i < tokens->length; i++)
	{
		if(isClass(tokens, i))
		{
			inClass = 1;	
		}
		else if(strcmp(tokens->contents[i], "{") == 0)
		{
			bracketCounter ++;
		}
		else if(strcmp(tokens->contents[i], "}") == 0)
		{
			bracketCounter --;
			if(bracketCounter == 0)
			{
				inClass = 0;	
				inOuterFunc = 0;
			}
		}
		/* if we are not in a class, we need to check if it is a function */
		else if(inClass == 0 && isFunction(tokens, i))
		{
			inOuterFunc = 1;
		}
		else if(inOuterFunc == 1 && isValidName((char*)tokens->contents[i]) && isMemberFunctionCall(tokens, i))
		{
			modifyFunctionCall(tokens, i);
		}
	}

	return 0;
}

int modifyFunctionCall(Vector* tokens, int pos)
{
	int i;
	int j;
	char* className;
	char* extraParam;
	char* args;
	char* newFName;
	char* comma;

	extraParam = malloc(sizeof(char) * 256);
	args = malloc(sizeof(char) * 512);
	comma = malloc(sizeof(char) * 2);

	args[0] = '\0';
	comma[0] = '\0';
	comma[1] = '\0';

	j = pos+2;

	/* creates the &className that will be added to the parameters */
	strcpy(extraParam, "&");
	strcpy(comma, ",");

	/* gets the name of the class */
	for(i = pos - 1; i >= 0; i--)
	{
		if(isValidName(tokens->contents[i]))
		{
			/* grabs the class variable name */
			strcat(extraParam, tokens->contents[i]);
			className = getType(tokens, i);
			break;
		}
		else if(!isspace(((char*)tokens->contents[i])[0]) && strcmp(tokens->contents[i], ".") != 0)
		{
			return -1;
		}
	}

	while(strcmp(tokens->contents[j], ")") != 0)
	{
		if(j >= tokens->length)
		{
			return -1;
		}
		/* we don't want to include the opening bracket... */
		if(strcmp(tokens->contents[j], "(") != 0)
		{
			/* this adds on another part of the parameters, and then a space to delimit them. */
			if(isValidName(tokens->contents[j]))
			{
				char* paramType;
				paramType = getType(tokens, j);
				if(isValidName(paramType))
				{
					strcat(args, "struct");
				}
				else
				{
					strcat(args, paramType);
				}
				free(paramType);
			}
			strcat(args, ",");
		}
		j++;
	}
	newFName = mangle(className, tokens->contents[pos], args);

	free(tokens->contents[pos]);
	tokens->contents[pos] = newFName;

	insert(tokens, extraParam, j);
	for(i = j - 1; i > pos; i--)
	{
		if(strcmp(tokens->contents[i], "(") == 0)
		{
			free(comma);
			break;
		}
		else if(!isspace(((char*) tokens->contents[i])[0]))
		{
			insert(tokens, comma, j);
			break;	
		}
	}
	free(className);
	free(args);

	return 0;
}
