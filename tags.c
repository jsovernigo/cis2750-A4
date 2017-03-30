
#include "tags.h"
#include "generation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

char* createText(char* textTag)
{
	int i;

	char** attributes;
	char* finalTag;
	
	char tagText[2048];
	char tagArgs[1024];

	i = 0;
	tagArgs[0] = '\0';
	tagText[0] = '\0';

	if(textTag == NULL)
	{
		return NULL;
	}

	/* gets the tag attributes from the tag. */
	attributes = getArgs(textTag);

	if(attributes == NULL)
	{
		strcpy(tagText, "Default Text");
	}
	else
	{
		while(attributes[i] != NULL)
		{
			/* checks for the text attribute.  If it is not present, then file is the attribute. */
			if(strncmp("text", attributes[i], 4) == 0)
			{
				char* value;

				value = getValue(attributes[i]);

				strcpy(tagText, value);

				free(value);
			}
			/* the only other tag that can be here is the "file" attribute. */
			else if(strncmp("file", attributes[i], 4) == 0)
			{
				FILE* inFile;
				char* value;

				value = getValue(attributes[i]);

				inFile = fopen(value, "r");
				if(inFile == NULL)
				{
					/* establishes the default text, allowing the file to not exist safely. */
					strcpy(tagText, "Default Text");
				}
				else
				{
					char buffer[256];
					char tagBuffer[2048];

					tagBuffer[0] = '\0';

					/* get all the text from the file. */
					while(fgets(buffer, 255, inFile) != NULL)
					{
						strcat(tagBuffer, buffer);
					}

					/* copy the text into the tag.  remember to return it. */
					strcpy(tagText, tagBuffer);

					fclose(inFile);
				}
				free(value);
			}
			/* this is the "default" case. */
			else
			{
				strcat(tagArgs, " ");
				strcat(tagArgs, attributes[i]);
			}

			free(attributes[i]);
			i++;
		}
		free(attributes);
	}

	finalTag = malloc(sizeof(char) * (strlen(tagText) + strlen(tagArgs) + 1));
	finalTag[0] = '\0';

	strcat(finalTag, tagText);

	return finalTag;
}


char* createHeader(char* headTag)
{
	int i;

	char tagArgs[1024];
	char headerText[512];
	char size[2];

	char* finalTag;
	char** attributes;

	i = 0;

	tagArgs[0] = '\0';
	headerText[0] = '\0';
	size[0] = '\0';

	/* gets the arguments for this tag. */
	attributes = getArgs(headTag);

	if(attributes != NULL)
	{
		while(attributes[i] != NULL)
		{

			/* if we have found the text attribute */
			if(strncmp(attributes[i], "text", 4) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcat(headerText, value);

				free(value);
			}
			/* the only other tag is size. */
			else if(strncmp(attributes[i], "size", 4) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcpy(size, value);

				free(value);
			}
			else
			{
				/* this can be used to add css elements to the tags. */
				strcat(tagArgs, " ");
				strcat(tagArgs, attributes[i]);
			}

			free(attributes[i]);
			i++;
		}
		free(attributes);
	}

	/* if the size has not been set. */
	if(strlen(size) == 0)
	{
		strcpy(size, "3");
	}

	/* similar; if the header text has not been set, we need to set it. */
	if(strlen(headerText) == 0)
	{
		strcpy(headerText, "HEADER");
	}

	/* add 9 extra chars, and a null terminator. */
	finalTag = malloc(sizeof(char) * (strlen(headerText) + strlen(tagArgs) + 10));
	if(finalTag == NULL)
	{
		return NULL;
	}

	finalTag[0] = '\0';

	/* add all the components to the final tag. */
	strcat(finalTag, "<h");
	strcat(finalTag, size);
	strcat(finalTag, tagArgs);
	strcat(finalTag, ">");
	strcat(finalTag, headerText);
	strcat(finalTag, "</h");
	strcat(finalTag, size);
	strcat(finalTag, ">");

	return finalTag;
}


char* createLink(char* linkTag)
{
	int i;

	char tagArgs[1024];
	char linkText[512];
	char pageLink[512];

	char* finalTag;
	char** attributes;

	i = 0;

	tagArgs[0] = '\0';
	linkText[0] = '\0';

	/* gets the arguments for this tag. */
	attributes = getArgs(linkTag);

	if(attributes != NULL)
	{
		while(attributes[i] != NULL)
		{
			/* if we have found the text attribute */
			if(strncmp(attributes[i], "text", 4) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcpy(linkText, value);

				free(value);
			}
			/* the only other tag is link. */
			else if(strncmp(attributes[i], "link", 4) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcpy(pageLink, "\"");
				strcat(pageLink, value);
				strcat(pageLink, "\"");

				free(value);
			}
			else
			{
				/* this can be used to add css elements to the tags. */
				strcat(tagArgs, " ");
				strcat(tagArgs, attributes[i]);
			}
			free(attributes[i]);
			
			i++;
		}
		free(attributes);
	}

	/* similar; if the header text has not been set, we need to set it. */
	if(strlen(linkText) == 0)
	{
		strcpy(linkText, "\"Link\"");
	}
	if(strlen(pageLink) == 0)
	{
		strcpy(pageLink, "url");
	}

	/* add 9 extra chars, and a null terminator. */
	finalTag = malloc(sizeof(char) * (strlen(linkText) + strlen(pageLink) + strlen(tagArgs) + strlen("<a href=><p></p></a>") + 1));

	if(finalTag == NULL)
	{
		return NULL;
	}

	finalTag[0] = '\0';

	/* add all the components to the final tag. */
	strcat(finalTag, "<a href=");
	strcat(finalTag, pageLink);
	strcat(finalTag, ">");
	strcat(finalTag, "<p");
	strcat(finalTag, tagArgs);
	strcat(finalTag, ">");
	strcat(finalTag, linkText);
	strcat(finalTag, "</p>");
	strcat(finalTag, "</a>");

	return finalTag;
}


char* createButton(char* buttonTag)
{
	int i;

	char tagArgs[1024];
	char buttonName[512];
	char buttonLink[512];
	char buttonText[512];

	char* finalTag;
	char** attributes;

	i = 0;

	tagArgs[0] = '\0';
	buttonName[0] = '\0';
	buttonLink[0] = '\0';
	buttonText[0] = '\0';

	/* gets the arguments for this tag. */
	attributes = getArgs(buttonTag);

	if(attributes != NULL)
	{
		while(attributes[i] != NULL)
		{
			if(strncmp("name", attributes[i], 4) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcpy(buttonName, value);

				free(value);
			}
			else if(strncmp("link", attributes[i], 4) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcpy(buttonLink, value);

				free(value);
			}
			else if(strncmp("text", attributes[i], 4) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcpy(buttonText, value);

				free(value);
			}
			else
			{
				strcat(tagArgs, " ");
				strcat(tagArgs, attributes[i]);
			}

			free(attributes[i]);
			i++;
		}

		free(attributes);
	}
	
	finalTag = malloc(sizeof(char) * (strlen(tagArgs) + strlen(buttonName) + strlen(buttonLink) + strlen(buttonText) + strlen("<form method=\"post\" action=\"\"><button type=\"submit\" name=\"\">Enter</button></form>") + 1));
	finalTag[0] = '\0';

	strcat(finalTag, "<form method=\"post\" action=\"");
	strcat(finalTag, buttonLink);
	strcat(finalTag, "\" ");
	strcat(finalTag, tagArgs);
	strcat(finalTag, "><button type=\"submit\"");
	strcat(finalTag, " name=\"");
	strcat(finalTag, buttonName);
	strcat(finalTag, "\">");
	strcat(finalTag, buttonText);
	strcat(finalTag, "</button></form>");

	return finalTag;
}


char* createInput(char* inputTag)
{
	int i;

	char tagArgs[1024];
	char inputBuffer[2048];
	char finalBuffer[2048];

	char* finalTag;
	char** attributes;

	i = 0;

	tagArgs[0] = '\0';
	inputBuffer[0] = '\0';
	finalBuffer[0] = '\0';

	/* gets the arguments for this tag. */
	attributes = getArgs(inputTag);
	

	while(attributes[i] != NULL)
	{
		/* this is the action that will be done if the form is submitted. */
		if(strncmp(attributes[i], "action", 6) == 0)
		{
			char* value;

			value = getValue(attributes[i]);
			strcat(inputBuffer, " action=\"");
			strcat(inputBuffer, value);
			strcat(inputBuffer, "\">");

			free(value);
		}
		/* text is after name. */
		else if(strncmp(attributes[i], "text", 4) == 0)
		{
			char* value;
			
			value = getValue(attributes[i]);
			strcat(inputBuffer, value);

			free(value);
		}
		/* name must be first. */
		else if(strncmp(attributes[i], "name", 4) == 0)
		{
			char* value;

			value = getValue(attributes[i]);
			strcat(inputBuffer, "<input type=\"text\" name=\"");
			strcat(inputBuffer, value);
			strcat(inputBuffer, "\"");


			free(value);
		}
		/* value is way after name, and just after text. */
		else if(strncmp(attributes[i], "value", 5) == 0)
		{
			char* value;

			value = getValue(attributes[i]);
			strcat(inputBuffer, " value=\"");
			strcat(inputBuffer, value);
			strcat(inputBuffer, "\"><br>");

			free(value);
		}
		else
		{
			strcat(tagArgs, " ");
			strcat(tagArgs, attributes[i]);
		}
		
		free(attributes[i]);
		i++;
	}
	free(attributes);

	strcat(inputBuffer, "<input type=\"submit\" value=\"Submit\"></form>");

	strcat(finalBuffer, "<form method=\"post\" ");
	strcat(finalBuffer, tagArgs);
	strcat(finalBuffer, inputBuffer);

	finalTag = malloc(sizeof(char) * (strlen(finalBuffer) + 1));
	strcpy(finalTag, finalBuffer);
	
	return finalTag;
}


char* createRadio(char* radioTag)
{
	int i;
	int cbuff;
	int numButtons;

	char tagArgs[1024];
	char radioBuffer[2048];
	char actionBuffer[512];
	char nameBuffer[512];
	char valueBuffers[64][256];

	char* finalTag;
	char** attributes;


	tagArgs[0] = '\0';
	radioBuffer[0] = '\0';
	actionBuffer[0] = '\0';
	nameBuffer[0] = '\0';
	
	/* clears all 64 buffers for the values that will eventually do something */
	for(i = 0; i < 64; i++)
	{
		valueBuffers[i][0] = '\0';
	}
	

	i = 0;
	cbuff = 0;
	numButtons = 0;

	/* gets the arguments for this tag. */
	attributes = getArgs(radioTag);

	if(attributes != NULL)
	{
		while(attributes[i] != NULL)
		{
			if(strncmp(attributes[i], "action", 6) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcpy(actionBuffer, value);

				free(value);
			}
			else if(strncmp(attributes[i], "name", 4) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcpy(nameBuffer, value);

				free(value);
			}
			else if(strncmp(attributes[i], "value", 5) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcpy(valueBuffers[cbuff], value);
				cbuff++;
				numButtons++;

				free(value);
			}
			else
			{
				strcat(tagArgs, " ");
				strcat(tagArgs, attributes[i]);
			}

			free(attributes[i]);
			i++;
		}
		free(attributes);
	}

	strcat(radioBuffer, "<form action=\"");
	strcat(radioBuffer, actionBuffer);
	strcat(radioBuffer, "\">");
	for(i = 0; i < numButtons; i++)
	{
		strcat(radioBuffer, "<input type=\"radio\" name=\"");
		strcat(radioBuffer, nameBuffer);
		strcat(radioBuffer, "\" value=\"");
		strcat(radioBuffer, valueBuffers[i]);
		strcat(radioBuffer, "\"");
		if(i == 0)
		{
			strcat(radioBuffer, " checked");
		}
		strcat(radioBuffer, ">");
		strcat(radioBuffer, valueBuffers[i]);
		strcat(radioBuffer, "<br>");
	}
	strcat(radioBuffer, "<input type=\"submit\" value=\"submit\"></form>");

	finalTag = malloc(sizeof(char) * (strlen(radioBuffer) + 1));
	strcpy(finalTag, radioBuffer);

	return finalTag;
}


char* createPicture(char* pictureTag)
{
	int i;

	char tagArgs[1024];
	char imageBuffer[512];
	char width[5];
	char height[5];

	char* finalTag;
	char** attributes;

	i = 0;

	tagArgs[0] = '\0';
	imageBuffer[0] = '\0';
	width[0] = '\0';
	height[0] = '\0';

	/* gets the arguments for this tag. */
	attributes = getArgs(pictureTag);

	if(attributes != NULL)
	{
		while(attributes[i] != NULL)
		{
			if(strncmp(attributes[i], "image", 5) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcpy(imageBuffer, value);

				free(value);
			}
			else if(strncmp(attributes[i], "size", 4) == 0)
			{
				int j;
				int swp;
				int cpos;

				char* value;

				j = 0;
				swp = 0;
				cpos = 0;

				/* stored as <width>x<height> */
				value = getValue(attributes[i]);
				for(j = 0; j < strlen(value); j++)
				{
					if(value[j] == 'x')
					{
						swp = 1;
						width[cpos] = '\0';
						cpos = 0;
					}
					else
					{
						if(swp == 1)
						{
							height[cpos] = value[j];
						}
						else
						{
							width[cpos] = value[j];
						}
						cpos ++;
					}
					
				}
				height[cpos] = '\0';

				free(value);
			}
			else
			{
				strcat(tagArgs, " ");
				strcat(tagArgs, attributes[i]);
			}
			
			free(attributes[i]);
			i++;
		}
		free(attributes);
	}

	finalTag = malloc(sizeof(char) * (strlen(height) + strlen(width) + strlen(imageBuffer) + strlen(tagArgs) + strlen("<img src=\"\" alt=\"Image_\" height=\"\" width = \"\">") + 1));
	finalTag[0] = '\0';

	strcat(finalTag, "<img src=\"");
	strcat(finalTag, imageBuffer);
	strcat(finalTag, "\" alt=\"Image_");
	strcat(finalTag, imageBuffer);
	strcat(finalTag, "\" width=\"");
	strcat(finalTag, width);
	strcat(finalTag, "\" height=\"");
	strcat(finalTag, height);
	strcat(finalTag, "\">");
	
	return finalTag;
}


char* createExec(char* execTag)
{
	int i;

	char tagArgs[1024];
	char execBuffer[2048];

	char* finalTag;
	char** attributes;

	i = 0;

	tagArgs[0] = '\0';
	execBuffer[0] = '\0';

	/* gets the arguments for this tag. */
	attributes = getArgs(execTag);

	if(attributes != NULL)
	{

		while(attributes[i] != NULL)
		{
			if(strncmp(attributes[i], "exe", 3) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcpy(execBuffer, value);	

				free(value);
			}
			else
			{
				strcat(tagArgs, " ");
				strcat(tagArgs, attributes[i]);
			}

			free(attributes[i]);
			i++;
		}
		free(attributes);
	}
	finalTag = malloc(sizeof(char) * (strlen(execBuffer) + strlen(tagArgs) + strlen("<?php exec(\"\", $output, $result); ?>") + 1));

	strcat(finalTag, "<?php exec(\"");
	strcat(finalTag, execBuffer);
	strcat(finalTag, "\", $output, $result); ");
	strcat(finalTag, tagArgs);
	strcat(finalTag, "?>");

	return finalTag;
}

char* createDiv(char* divTag)
{
	int i;
	char tagArgs[1024];
	char idBuffer[512];
	char divBuffer[2048];

	char* finalTag;
	char** attributes;

	i = 0;
	tagArgs[0] = '\0';
	idBuffer[0] = '\0';
	divBuffer[0] = '\0';

	attributes = getArgs(divTag);

	if(attributes != NULL)
	{
		while(attributes[i] != NULL)
		{
			if(strncmp(attributes[i], "id", 2) == 0)
			{
				char* value;
				value = getValue(attributes[i]);
				strcpy(idBuffer, value);

				free(value);
			}
			else
			{
				strcat(tagArgs, " ");
				strcat(tagArgs, attributes[i]);
			}

			free(attributes[i]);
			i++;
		}
		free(attributes);
	}
	
	strcpy(divBuffer, "<div id=\"");
	strcat(divBuffer, idBuffer);
	strcat(divBuffer, "\" ");
	strcat(divBuffer, tagArgs);
	strcat(divBuffer, ">");

	finalTag = malloc(sizeof(char) * (strlen(divBuffer) + 1));
	strcpy(finalTag, divBuffer);

	return finalTag;
}


char* createSelector(char* selTag)
{
	int i;
	char tagArgs[1024];
	char formBuffer[512];
	char buffer[2048];

	char* finalTag;
	char** attributes;

	attributes = getArgs(selTag);
	i = 0;
	tagArgs[0] = '\0';
	buffer[0] = '\0';

	if(attributes != NULL)
	{
		while(attributes[i] != NULL)
		{
			if(strncmp(attributes[i], "form", 4) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcpy(formBuffer, value);

				free(value);
			}
			else
			{
				strcat(tagArgs, " ");
				strcat(tagArgs, attributes[i]);
			}

			free(attributes[i]);
			i++;
		}
		free(attributes);
	}


	strcat(buffer, "<?php\n");
	strcat(buffer, "$files = scandir(\"./messages\");\n");
	strcat(buffer, "$streams = array();\n");

	strcat(buffer, "foreach($files as $fn)\n{\n");
	strcat(buffer, "if(strpos($fn, \"StreamUsers\") !== false)\n{\n");
	strcat(buffer, "$streams[] = substr($fn, 0, strlen($fn) - 11);");
	strcat(buffer, "\n}\n}\n");

	strcat(buffer, "echo \"<select ");
	strcat(buffer, tagArgs);
	strcat(buffer, " name=\\\"stream\\\" form=\\\"");
	strcat(buffer, formBuffer);
	strcat(buffer, "\\\">\\n\";\n");
	strcat(buffer, "foreach($streams as $stream)\n{\n");
	strcat(buffer, "echo \"<option value=\\\"$stream\\\">$stream</option>\\n\";\n}\n");
	strcat(buffer, "echo \"<option value=\\\"all\\\">all</option>\";");
	strcat(buffer, "echo \"</select><br>\\n\";\n");
	strcat(buffer, "?>");

	finalTag = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(finalTag, buffer);

	return finalTag;
}


char* createDependency(char* depTag)
{
	int i;
	char tagArgs[1024];
	char idBuffer[512];
	char varBuffer[512];

	char depBuffer[2048];

	char* finalTag;
	char** attributes;

	i = 0;
	tagArgs[0] = '\0';
	idBuffer[0] = '\0';
	depBuffer[0] = '\0';

	attributes = getArgs(depTag);

	if(attributes != NULL)
	{
		while(attributes[i] != NULL)
		{
			if(strncmp(attributes[i], "id", 2) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcpy(idBuffer, value);

				free(value);
			}
			else if(strncmp(attributes[i], "dep", 3) == 0)
			{
				char* value;

				value = getValue(attributes[i]);
				strcpy(varBuffer, value);

				free(value);
			}
			else
			{
				strcat(tagArgs, " ");
				strcat(tagArgs, attributes[i]);
			}

			free(attributes[i]);
			i++;
		}
		free(attributes);
	}

	strcpy(depBuffer, "<?php echo \"<input type=\\\"hidden\\\" value=\\\"\".$_POST[\"");
	strcat(depBuffer, varBuffer);
	strcat(depBuffer, "\"].\"\\\" name=\\\"");
	strcat(depBuffer, varBuffer);
	strcat(depBuffer, "\\\" form=\\\"");
	strcat(depBuffer, idBuffer);
	strcat(depBuffer, "\\\" ");
	strcat(depBuffer, tagArgs);
	strcat(depBuffer, ">\"; ?>");

	finalTag = malloc(sizeof(char) * (strlen(depBuffer) + 1));
	strcpy(finalTag, depBuffer);

	return finalTag;
}


char* createAdd(char* addTag)
{
	char buffer[2048];

	char* finalTag;

	buffer[0] = '\0';
	strcat(buffer, "<?php\n");
	strcat(buffer, " echo \"<form id=\\\"adduser\\\" method=\\\"post\\\" action=\\\"addauthor.php\\\">\";\n");
	strcat(buffer, "echo \"<input type=\\\"text\\\" name=\\\"newstreams\\\"><br>\";\n");
	strcat(buffer, "$files = scandir(\"./messages\");\n");
	strcat(buffer, "$streams = array();\n");
	strcat(buffer, "foreach($files as $fn)\n{\n if(strpos($fn, \"StreamUsers\") !== false)\n{\n $streams[] = substr($fn, 0, strlen($fn) - 11);\n}\n}\n");
	strcat(buffer, "foreach($streams as $stream)\n{\n echo \"<input type=\\\"checkbox\\\" name=\\\"streams[]\\\" value=\\\"$stream\\\">$stream<br>\";\n}\n");
	strcat(buffer, "echo \"<input type=\\\"submit\\\" value=\\\"add\\\" name=\\\"action\\\"><input type=\\\"submit\\\" value=\\\"remove\\\" name=\\\"action\\\">\";\n");
	strcat(buffer, "echo \"</form>\";\n?>");
	
	finalTag = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(finalTag, buffer);

	return finalTag;
}


char* createView(char* viewTag)
{
	char buffer[2048];

	char* finalTag;

	buffer[0] = '\0';

	strcat(buffer, "<?php\n");

	strcat(buffer, "echo \"<div id=\\\"view\\\">\\n\";\n");
	strcat(buffer, "echo \"<p>$post</p>\";\n");
	strcat(buffer, "echo \"</div>\";\n");

	strcat(buffer, "echo \"<form method=\\\"post\\\" action=\\\"view.php\\\" id=\\\"command\\\">\\n\";\n");

	strcat(buffer, "echo \"<button type=\\\"submit\\\" name=\\\"userin\\\" value=\\\"next\\\">Next</button>\\n\";\n");
	strcat(buffer, "echo \"<button type=\\\"submit\\\" name=\\\"userin\\\" value=\\\"prev\\\">Previous</button>\\n\";\n");
	strcat(buffer, "echo \"<button type=\\\"submit\\\" name=\\\"userin\\\" value=\\\"order\\\">Order</button>\\n\";\n");
	strcat(buffer, "echo \"<button type=\\\"submit\\\" name=\\\"userin\\\" value=\\\"mark\\\">Mark All</button>\\n\";\n");
	strcat(buffer, "echo \"<button form=\\\"switch\\\"type=\\\"submit\\\" name=\\\"userin\\\" value=\\\"next\\\">Switch Streams</button>\\n\";\n");

	strcat(buffer, "echo \"</form>\";\n");

	strcat(buffer, "echo \"<form method=\\\"post\\\" action=\\\"switch.php\\\" id=\\\"switch\\\"></form>\\n\";\n");

	strcat(buffer, "?>\n");

	finalTag = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(finalTag, buffer);

	return finalTag;
}


char* createPost(char* postTag)
{
	char buffer[2048];
	char* finalTag;

	buffer[0] = '\0';

	strcat(buffer, "<?php\n");
	strcat(buffer, "echo \"<form id=\\\"post\\\" method=\\\"post\\\" action=\\\"post.php\\\">\\n\";\n");
	strcat(buffer, "echo \"<textarea name=\\\"post\\\" rows=\\\"20\\\" cols=\\\"70\\\">\\n</textarea><br>\\n\";\n");

	strcat(buffer, "echo \"<input type=\\\"submit\\\" name=\\\"postpressed\\\" value=\\\"submit\\\">\\n</form>\\n\";\n");
	strcat(buffer, "?>");

	finalTag = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(finalTag, buffer);

	return finalTag;
}
