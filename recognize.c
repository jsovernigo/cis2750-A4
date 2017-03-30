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
#include"recognize.h"
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int nstrchr(char* string, char character)
{
	int occurences;
	int length;
	int i;

	if(string == NULL)
	{
		return -1;
	}

	length = strlen(string);
	occurences = 0;

	for(i = 0; i < length; i++) /* loop through the characters in the string */
	{
		if(string[i] == character)
		{
			occurences ++;
		}
	}

	return occurences;
}

int isValidName(char* name)
{
	const char* keywords[] = {"auto","break","case","char","const","continue","default","do","double","else","extern","float","for","goto","if","int","long","register","return","short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while","class",NULL};

	unsigned int i;

	if(name == NULL)
	{
		return -1;
	}

	/* if the first character is not an underscore or not alphabetical */
	if(!isalpha(name[0]) && name[0] != '_')
	{
		return 0;
	}

	/* loop through the rest of the name */
	for(i = 1; i < strlen(name); i++)
	{
		if(!isalpha(name[i]) && !isdigit(name[i]) && name[i] != '_')
		{
			return 0;
		}
	}

	i = 0;
	/* loops through the keywords, NULL is placed at the end to protect from overflow. */
	while(keywords[i] != NULL)
	{
		/* we do not want a variable to be named a keyword! */
		if(strcmp(name, keywords[i]) == 0)
		{
			return 0;
		}

		i++;
	}

	return 1;
}

char getTypeID(Vector* tokens, int pos)
{
	int i;
	int startPos;


	/* there are three stages to this check; we need to check in the following order:
	 * 1-	check local vars and parameters
	 * 2-	check global vars
	 *
	 * note that class variable were not included.  This is because we should have
	 * already called addThisRef and addThisArg prior to invoking this function.  This
	 * would cause all class variables to be prepended with "this->"
	 */

	/* loop backwards, looking for a variable declaration. */

	/* gets the starting position of the program. */
	for(i = pos; i >= 0; i--)
	{
		if(isFunction(tokens, i))
		{
			startPos = i;
			break;
		}
	}

	for(i = startPos + 1; i <= pos; i++)
	{
		/* "if we found a variable declaration, and it matches our given variable name" */
		if(strcmp(tokens->contents[i], tokens->contents[pos]) == 0 && isVariable(tokens, i))
		{
			int j;
			int multipleDeclaration;
			for(j = i - 1; j >= 0; j--)
			{
				if(isValidType(tokens->contents[j]) == 1)
				{
					/* return the first character of the current type identifier. */
					return ((char*) tokens->contents[j])[0];
				}
				else if(isValidName(tokens->contents[j]))
				{
					if(multipleDeclaration == 0)
					{
						return 's';
					}
				}
				/* this means we have found a situation like int a,b; */
				else if(strcmp(tokens->contents[j], ",") == 0)
				{
					multipleDeclaration = 1;
				}
				else if(!isspace(((char*)tokens->contents[j])[0]) && strcmp(tokens->contents[j], "*") != 0)
				{
					continue;
				}
				/* if we find a comma, check the type of the name behind us!!! */
			}
		}
		/* this block executing signifies that the function that we are in has been located */
		else if(isFunction(tokens, i))
		{
			break;
		}
	}

	/* now we have to essentially make sure we find global variables. */
	for(i = 0; i < tokens->length; i++)
	{
		/* "if we found a variable declaration, and it matches our given variable name" */
		if(strcmp(tokens->contents[i], tokens->contents[pos]) == 0 && isVariable(tokens, i))
		{
			int j;
			int multipleDeclaration;
			for(j = i - 1; j >= 0; j--)
			{
				if(isValidType(tokens->contents[j]) == 1)
				{
					/* return the first character of the current type identifier. */
					return ((char*) tokens->contents[j])[0];
				}
				else if(isValidName(tokens->contents[j]))
				{
					if(multipleDeclaration == 0)
					{
						return 's';
					}
				}
				/* this means we have found a situation like int a,b; */
				else if(strcmp(tokens->contents[j], ",") == 0)
				{
					multipleDeclaration = 1;
				}
				else if(!isspace(((char*)tokens->contents[j])[0]))
				{
					continue;
				}
				/* if we find a comma, check the type of the name behind us!!! */
			}
		}

		if(isClass(tokens, i))
		{
			break;
		}
	}

	return '\0';
}

char* getType(Vector* tokens, int pos)
{
	int i;
	int startPos;

	/* gets the starting position of the program. */
	for(i = pos; i >= 0; i--)
	{
		if(isFunction(tokens, i))
		{
			startPos = i;
			break;
		}
	}

	for(i = startPos + 1; i <= pos; i++)
	{
		/* "if we found a variable declaration, and it matches our given variable name" */
		if(strcmp(tokens->contents[i], tokens->contents[pos]) == 0 && isVariable(tokens, i))
		{
			int j;
			int multipleDeclaration;

			multipleDeclaration = 0;
			for(j = i - 1; j >= 0; j--)
			{
				if(isValidType(tokens->contents[j]) == 1)
				{
					char* str;

					str = malloc(sizeof(char) * strlen(tokens->contents[j]) + 1);
					strcpy(str, tokens->contents[j]);
					/* return the first character of the current type identifier. */
					return str;
				}
				else if(isValidName(tokens->contents[j]))
				{
					if(multipleDeclaration == 0)
					{	
						char* str;
						str = malloc(sizeof(char) * strlen(tokens->contents[j]) + 1);
						strcpy(str, tokens->contents[j]);
						return str;
					}
					else
					{
						multipleDeclaration = 0;
					}
				}
				/* this means we have found a situation like int a,b; */
				else if(strcmp(tokens->contents[j], ",") == 0)
				{
					multipleDeclaration = 1;
				}
				else if(!isspace(((char*)tokens->contents[j])[0]) && strcmp(tokens->contents[j], "*") != 0)
				{
					continue;
				}
				/* if we find a comma, check the type of the name behind us!!! */
			}
		}
		/* this block executing signifies that the function that we are in has been located */
		else if(isFunction(tokens, i))
		{
			break;
		}
	}

	/* now we have to essentially make sure we find global variables. */
	for(i = 0; i < tokens->length; i++)
	{
		/* "if we found a variable declaration, and it matches our given variable name" */
		if(strcmp(tokens->contents[i], tokens->contents[pos]) == 0 && isVariable(tokens, i))
		{
			int j;
			int multipleDeclaration;

			multipleDeclaration = 0;
			for(j = i - 1; j >= 0; j--)
			{
				if(isValidType(tokens->contents[j]) == 1)
				{
					char* str;

					str = malloc(sizeof(char) * strlen(tokens->contents[j]) + 1);
					strcpy(str, tokens->contents[j]);
					/* return the first character of the current type identifier. */
					return str;
				}
				else if(isValidName(tokens->contents[j]))
				{
					if(multipleDeclaration == 0)
					{
						char* str;
						str = malloc(sizeof(char) * strlen(tokens->contents[j]) + 1);
						strcpy(str, tokens->contents[j]);
						return str;
					}
				}
				/* this means we have found a situation like int a,b; */
				else if(strcmp(tokens->contents[j], ",") == 0)
				{
					multipleDeclaration = 1;
				}
				else if(!isspace(((char*)tokens->contents[j])[0]))
				{
					continue;
				}
				/* if we find a comma, check the type of the name behind us!!! */
			}
		}

		if(isClass(tokens, i))
		{
			break;
		}
	}

	return NULL;
}

int isValidType(char* type)
{
	const char* validAtomics[] = {"auto","char","double","float","int","long","short","void",NULL};
	const char* validPrefixes[] = {"const","extern","register","signed","static","struct","union","unsigned","volatile",NULL};

	int i;

	if(type == NULL)
	{
		return -1;
	}

	i = 0;

	/* check if valid atomic type. */
	while(validAtomics[i] != NULL)
	{
		if(strcmp(type, validAtomics[i]) == 0)
		{
			return 1;
		}
		i++;
	}

	i = 0;

	/* check if valid prefix, like struct, etc. */
	while(validPrefixes[i] != NULL)
	{
		if(strcmp(type, validPrefixes[i]) == 0)
		{
			return 2;
		}
		i++;
	}

	/* if the type was class, essentially. */
	if(strcmp(type, "class") == 0)
	{
		return 3;
	}

	return 0;
}

int isClass(Vector* tokens, int pos)
{
	int i;
	int classPresent;
	int validName;
	int validSuffix;

	if(tokens == NULL)
	{
		return -1;
	}

	if(pos < 0 || pos == tokens->length)
	{
		return -1;
	}

	/* check for class preceeding this location. */
	for(i = pos - 1; i >= 0; i--)
	{
		/* if our preceeding token is a valid type or valid struct name */
		if(strcmp(tokens->contents[i], "class") == 0)
		{
			classPresent = 1;
			break;
		}
		else if(!isspace(((char*)tokens->contents[i])[0]))
		{
			return 0;
		}
	}

	/* if the current token is a valid function name */
	if(isValidName(tokens->contents[pos]))
	{
		validName = 1;
	}

	for(i = pos + 1; i < tokens->length; i++)
	{
		/* if the next token is an open parenthesis */
		if(strcmp(tokens->contents[i], "{") == 0)
		{
			validSuffix= 1;
			break;
		}
		else if(!isspace(((char*)tokens->contents[i])[0]))
		{
			return 0;
		}
	}

	if(classPresent && validName && validSuffix)
	{
		return 1;
	}

	return 0;
}

/**
 *	cases to consider here:
 *	0	- not a valid function declaration
 *	1	- <c return type> <functionName>(<args>)
 *	2	- <modifier> <c return type> <functionName>(<args>)
 *	3	- <stuct> <structName> <functionName>(<args>)
 *	4	- <class> <className> <functionName>(<args>)
 */
int isFunction(Vector* tokens, int pos)
{
	int i;
	int typePresent;
	int nameValid;
	int suffixValid;

	typePresent = 0;
	nameValid = 0;
	suffixValid = 0;

	if(tokens == NULL)
	{
		return -1;
	}
	else if(pos >= tokens->length || pos < 0)
	{
		return -1;
	}

	for(i = pos - 1; i >= 0; i--)
	{
		if(typePresent == 1)
		{
			break;
		}
		/* if our preceeding token is a valid type or valid struct name */
		if(isValidType(tokens->contents[i]) || isValidName(tokens->contents[i]) || strstr(",",tokens->contents[i]) != NULL)
		{
			typePresent = 1;
			break;
		}
		else if(!isspace(((char*)tokens->contents[i])[0]) && strcmp(tokens->contents[i], "*") != 0)
		{
			return 0;
		}
	}

	/* if the current token is a valid function name */
	if(isValidName(tokens->contents[pos]))
	{
		nameValid = 1;
	}

	for(i = pos + 1; i < tokens->length; i++)
	{
		/* if the next token is an open parenthesis */
		if(strcmp(tokens->contents[i], "(") == 0)
		{
			suffixValid = 1;
			break;
		}
		else if(!isspace(((char*)tokens->contents[i])[0]))
		{
			return 0;
		}
	}

	if(typePresent && nameValid && suffixValid)
	{
		return 1;
	}

	return 0;
}

int isFunctionCall(Vector* tokens, int pos)
{
	int i;

	int prevValid;
	int nameValid;
	int suffValid;

	prevValid = 0;
	nameValid = 0;
	suffValid = 0;

	if(tokens == NULL)
	{
		return -1;
	}

	for(i = pos - 1; i >= 0; i--)
	{
		if(!isValidType(tokens->contents[i]) && !isValidName(tokens->contents[i]) && !isspace(((char*)tokens->contents[i])[0]))
		{
			prevValid = 1;
			break;
		}
		else if(!isspace(((char*)tokens->contents[i])[0]))
		{
			return 0;
		}
	}
	if(isValidName(tokens->contents[pos]))
	{
		nameValid = 1;
	}

	for(i = pos + 1; i < tokens->length; i++)
	{
		if(strcmp(tokens->contents[i], "(") == 0)
		{
			suffValid = 1;
			break;
		}
		else if(!isspace(((char*)tokens->contents[i])[0]))
		{
			return 0;
		}
	}
	if(prevValid && nameValid && suffValid)
	{
		return 1;
	}

	return 0;
}

int isMemberFunctionCall(Vector* tokens, int pos)
{
	int i;

	int periodPresent;
	int nameValid;
	int hasBrackets;

	periodPresent = 0;
	nameValid = 0;
	hasBrackets = 0;

	/* checks for the period preceeding the function call */
	for(i = pos - 1; i >= 0; i--)
	{
		if(strcmp(tokens->contents[i], ".") == 0)
		{
			periodPresent = 1;
			break;
		}
		else if(!isspace(((char*)tokens->contents[i])[0]))
		{
			return 0;
		}
	}

	if(isValidName(tokens->contents[pos]))
	{
		nameValid = 1;
	}
	else
	{
		return 0;
	}

	for(i = pos + 1; i < tokens->length; i++)
	{
		if(strcmp(tokens->contents[i], "(") == 0)
		{
			hasBrackets = 1;
			break;
		}
		else if(!isspace(((char*) tokens->contents[i])[0]))
		{
			return 0;
		}
	}

	return periodPresent && nameValid && hasBrackets;	
}

int isFunctionPointer(Vector* tokens, int pos)
{
	int i;

	int returnValid;
	int prevSyntax;
	int nameValid;
	int postSyntax;
	int hasBrackets;

	returnValid = 0;
	prevSyntax = 0;
	nameValid = 0;
	postSyntax = 0;
	hasBrackets = 0;
	
	
	/* looks for the (* syntax, and then tries to find the return type */
	for(i = pos - 1; i >= 0; i--)
	{
		if(returnValid == 1)
		{
			break;
		}	

		/* this finds the (* syntax that is needed (inserted by migrateFunctions) */
		if(strcmp(tokens->contents[i], "(*") == 0)
		{
			int j;

			prevSyntax = 1;

			/* loop backwards from the (* syntax to get the return type. */
			for(j = i - 1; j >= 0; j--)
			{
				if(isValidType(tokens->contents[j]) || isValidName(tokens->contents[j]))
				{
					returnValid = 1;
					break;
				}
				/* ignore spaces and asterisks */
				else if(!isspace(((char*)tokens->contents[j])[0]) && strcmp(tokens->contents[j], "*") != 0)
				{
					return 0;
				}

			}
		}
		else if(!isspace(((char*)tokens->contents[i])[0]))
		{
			return 0;
		}
	}

	/* if the current token is valid */
	if(isValidName(tokens->contents[pos]))
	{
		nameValid = 1;
	}
	else
	{
		return 0;
	}

	/* checks for the following syntax */
	for(i = pos + 1; i < tokens->length; i++)
	{
		if(postSyntax == 1)
		{
			break;
		}
		if(strcmp(tokens->contents[i], ")") == 0)
		{
			int j;
			postSyntax = 1;

			/* loop after the closing brace that surrounds the function pointer name */
			for(j = i + 1; j < tokens->length; j++)
			{
				/* this would represent the opening brace after the function pointer name */
				if(strcmp(tokens->contents[j], "(") == 0)
				{
					hasBrackets = 1;
					break;
				}
				else if(!isspace(((char*) tokens->contents[j])[0]))
				{
					return 0;
				}
			}
		}
		else if(!isspace(((char*)tokens->contents[i])[0]))
		{
			return 0;
		}
	}

	/* returning 1 only if all the conditions are true */
	return returnValid && prevSyntax && nameValid && postSyntax && hasBrackets;
}

int beginsFunction(Vector* tokens, int pos)
{
	int i;

	int typeValid;
	int nameValid;
	int parnValid;
	int index;

	typeValid = 0;
	nameValid = 0;
	parnValid = 0;
	i = 0;

	index = 0;

	if(isValidType(tokens->contents[pos]) == 1)
	{
		index = pos + 1;
		typeValid = 1;
	}
	else if(isValidType(tokens->contents[pos]) == 2)
	{
		for(i = pos + 1; i < tokens->length; i++)
		{
			if(isValidName(tokens->contents[i]))
			{
				index = i + 1;
				typeValid = 1;
				break;
			}
			else if(!isspace(((char*)tokens->contents[i])[0]))
			{
				return 0;
			}
		}
	}
	else if(isValidType(tokens->contents[pos]) == 3)
	{
		/* check for a valid class name */
		for(i = pos + 1; i < tokens->length; i++)
		{
			if(isValidName(tokens->contents[i]))
			{
				index = i + 1;
				typeValid = 1;
				break;
			}
			else if(!isspace(((char*)tokens->contents[i])[0]))
			{
				return 0;
			}

		}
	}

	for(i = index; i < tokens->length; i++)
	{
		/* this would be the name of the function itself. */
		if(isValidName(tokens->contents[i]))
		{
			nameValid = 1;
			index = i + 1;
			break;
		}
		else if(strstr("*****", tokens->contents[i]) != NULL)
		{
			int j;
			for(j = i + 1; j < tokens->length; j++)
			{
				/* if we found a pointer (up to five levels), we need to find the name */
				if(isValidName(tokens->contents[j]))
				{
					nameValid = 1;
					index = j + 1;
					break;
				}
				else if(!isspace(((char*)tokens->contents[j])[0]))
				{
					return 0;
				}
			}
		}
		else if(!isspace(((char*)tokens->contents[i])[0]))
		{
			return 0;
		}
	}

	/* search for the open parenthesis */
	for(i = index; i < tokens->length; i ++)
	{
		if(strcmp(tokens->contents[i], "(") == 0)
		{
			parnValid = 1;
			break;
		}
		else if(!isspace(((char*)tokens->contents[i])[0]))
		{
			return 0;
		}
	}

	return nameValid && typeValid && parnValid;
}

int isVariable(Vector* tokens, int pos)
{
	int i;

	int typePresent;
	int nameValid;
	int suffixValid;

	typePresent = 0;
	nameValid = 0;
	suffixValid = 0;


	if(tokens == NULL)
	{
		return -1;
	}
	else if(pos >= tokens->length || pos < 0)
	{
		return -1;
	}

	/* loop backwards until we find a variable type */
	for(i = pos - 1; i >= 0; i--)
	{
		if(isValidType(tokens->contents[i]) || isValidName(tokens->contents[i]) )
		{
			typePresent = 1;
			break;
		}
		else if(strcmp(",", tokens->contents[i]) == 0)
		{
			continue;
		}
		else if(!isspace(((char*)tokens->contents[i])[0]) && strcmp(tokens->contents[i], "*") != 0)
		{
			return 0;
		}
	}

	if(isValidName(tokens->contents[pos]))
	{
		nameValid = 1;
	}

	for(i = pos + 1; i < tokens->length; i++)
	{
		if(strstr(",;=[", tokens->contents[i]) != NULL)
		{
			suffixValid = 1;
			break;
		}
		else if(!isspace(((char*)tokens->contents[i])[0]))
		{
			return 0;
		}
	}

	if(typePresent && nameValid && suffixValid)
	{
		return 1;
	}

	return 0;
}

/*
int isVariableDeclaration(Vector* tokens, int pos)
{
	int i;

	int typePreceeds;
	int nameValid;
	int semicolonFollows;

	typePreceeds = 0;
	semicolonFollows = 0;

	if(!isValidName(tokens->contents[pos]))
	{
		return 0;
	}

	for(i = pos - 1; i >= 0; i++)
	{
		if(isValidType(tokens->contents[i]))
		{
			typePreceeds = 1;
		}
		else if(strstr("{;}=", tokens->contents[i]) != NULL)
		{
			return 0;
		}
	}

	if(isValidName(nameValid))
	{
		nameValid = 1;
	}
	else
	{
		return 0;
	}

	for(i = pos + 1; i < tokens->pos; i++)
	{
		if(strcmp(tokens->contents[i]) == 0)
		{

		}
		else if()
		{

		}
	}

	return typePreceeds && nameValid && semicolonFollows;
}
*/

int isClassVariable(Vector* tokens, int pos)
{
	int i;
	int classPresent;
	int classNamePresent;
	int nameValid;
	int isDecOrDef;

	classPresent = 0;
	classNamePresent = 0;
	nameValid = 0;
	isDecOrDef = 0;

	/* find the className. */
	for(i = pos - 1; i >= 0; i--)
	{
		if(classPresent == 1)
		{
			break;
		}

		if(strcmp(tokens->contents[i], "class") == 0)
		{
			int j;

			for(j = i + 1; j < pos; j++)
			{
				if(isValidName(tokens->contents[j]))
				{
					classNamePresent = 1;
					break;
				}
				else if(!isspace(((char*)tokens->contents[j])[0]))
				{
					return 0;
				}
			}
			classPresent = 1;

		}
		/* the only other accepted characters are spaces, variable/class names, and commas */
		else if(!isspace(((char*)tokens->contents[i])[0]) && !isValidName(tokens->contents[i])&& strcmp(tokens->contents[i], ",") != 0)
		{
			return 0;
		}
	}

	if(isValidName(tokens->contents[pos]))
	{
		nameValid = 1;
	}
	else
	{
		return 0;
	}

	/* now we are searching for the semicolon.  we will ignore commas and valid names. */
	for(i = pos + 1; i < tokens->length; i++)
	{
		/* checks if tokens->contents[i] is contained within ;= */
		if(strstr(";=",tokens->contents[i]) != NULL)
		{
			isDecOrDef = 1;
			break;
		}
		else if(!isspace(((char*)tokens->contents[i])[0]) && !isValidName(tokens->contents[i]) && strcmp(tokens->contents[i], ",") != 0)
		{
			return 0;
		}

	}

	return classPresent && classNamePresent && nameValid && isDecOrDef;
}
