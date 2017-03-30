
#ifndef __GSOVERNIGO_TAGS_H__
#define __GSOVERNIGO_TAGS_H__

#define START_TAGS "<html><body>"
#define END_TAGS "</body></html>"
#define LINE_BREAK "<br>"

/**
 *	createText
 *	creates a text html tag from the passed in wpml tag.
 *	IN:			textTag			- the text tag that we will be reading
 *	OUT:		a new html-stable text tag (no <p> tags surrounding)
 *	POST:		none
 *	ERROR:		textTag is NULL.
 */
char* createText(char* textTag);

/**
 *	createHeader
 *	creates a header tag from the passed in wpml tag.
 *	IN:			headTag			- the wpml header tag
 *	OUT:		a new html header tag <hn> formatted.
 *	POST:		none
 *	ERROR:		headTag is NULL.
 */
char* createHeader(char* headTag);

/**
 *	createLink
 *	creates an <a href> tag from a wpml tag.
 *	IN:			a .l wpml tag.
 *	OUT:		an <a href> html tag.
 *	POST:		none
 *	ERROR:		linktag is NULL
 */
char* createLink(char* linkTag);

/**
 *	createButton
 *	creates a button tag-and-form in html
 *	IN:			buttonTag 		- the button wpml tag.
 *	OUT:		an html button and form tag.
 *	POST:		none
 *	ERROR:		buttontag is null.
 */
char* createButton(char* buttonTag);

/**
 *	createInput
 *	creates an text input tag-and-form
 *	IN:			inputTag		- the input wpml tag
 *	OUT:		an html output tag
 *	POST:		none
 *	ERROR:		inputTag is NULL
 */
char* createInput(char* inputTag);

/**
 *	createRadio
 *	creates an html radio button from a wpml config tag
 *	IN:			radiotag		- the radio wpml tag
 *	OUT:		a new radio button html tag.
 *	POST:		none
 *	ERROR:		radiotag is null
 */
char* createRadio(char* radioTag);

/**
 *	createPicture
 *	creates a picture html tag (actually <img>)
 *	IN:			pictureTag		- the picture wpml tag.
 *	OUT:		a new picture (img) html tag
 *	POST:		none
 *	ERROR:		picturetag is null.
 */
char* createPicture(char* pictureTag);

/**
 *	createExec
 *	creates an executable tag, which when eval()'d by php can execute a 
 *	program.
 *	IN:			execTag			- a new executable tag.
 *	OUT:		a new executable tag that allows you to server-execute
 *				... something.
 *	POST:		none
 *	ERROR:		exectag is NULL.
 */
char* createExec(char* execTag);


/*-------------------------- start of user-made tags ---------------------------*/

/**
 *	createDiv
 *	creates a div (opening) tag.
 *	IN:			divTag			- a wpml f tag
 *	OUT:		produces a div tag.
 *	POST:		none
 *	ERROR:		divTag is null
 */
char* createDiv(char* divTag);

/**
 *	createSelector
 *	creates a selector tag that has all the possible streams in the messages
 *	folder.
 *	IN:			selTag			- the selector tag (wpml .s())
 *	OUT:		a selector html tag with all the options
 *	POST:		none
 *	ERROR:		seltag is null.
 */
char* createSelector(char* selTag);

/**
 *	createDependency
 *	creates a hidden input field that allows thr propogation of values between
 *	several pages without re-submission of the form or page.
 *	IN:			depTag			- a wpml dependency tag
 *	OUT:		a new dependency tag which can produce a hidden field
 *	POST:		none
 *	ERROR:		depTag is NULL.
 */
char* createDependency(char* depTag);

/**
 *	createAdd
 *	creates php that produces an addauthor component.
 *	IN:			addTag			- the wpml .a() tag.
 *	OUT:		a php script that can produce an addauthor component
 *	POST:		none
 *	ERROR:		none
 */
char* createAdd(char* addTag);

/**
 *	createView
 *	creates a php script that generates a view interface.
 *	IN:			a .v() tag.
 *	OUT:		a new php script that creates a view interface.
 *	POST:		none
 *	ERROR:		malloc fails, otherwise none.
 */
char* createView(char* viewTag);

/**
 *	createPost
 *	creates a php script that generates a view tag.
 *	IN:			postTag			- a .p() tag.
 *	OUT:		a php script that generates a post interface.
 *	POST:		none
 *	ERROR:		malloc fails.
 */
char* createPost(char* postTag);

#endif
