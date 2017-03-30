CC		=gcc
CCON	=./a1
CFLAGS	=-ansi -Wall -g
TFLAGS	=-Wall -pedantic -Wextra -Werror -ansi -g
COBJECTS=driver.o classToStruct.o parseFile.o recognize.o vector.o
OBJECTS	=stream.o post.o tags.o generation.o
EXEC1 	=post
EXEC2	=addauthor
EXEC3	=create
EXEC4	=db
INCLUDE	=./
LINK	=-lstream
MYSQL	=-lmysqlclient -L/usr/lib/x86_64-linux-gnu
LPATH	=./


#
# This specifies which rules take no parameters.
#
.PHONY: all clean clobber

all: build

#
# This is the normal compilation rule.  Simply typing 'make' will run this
#
build: cconverter post libstream.a addAuthor view.py create database
	mkdir -p ./messages/

#
# Large-scale macros
#

addAuthor: libstream.a addAuthor.c
	$(CC) $(CFLAGS) -o $(EXEC2) addAuthor.c -I$(INCLUDE) -L$(LPATH) $(LINK) $(MYSQL)

libstream.a: stream.o
	ar cr libstream.a stream.o #$(MYSQL)

cconverter: $(COBJECTS)
	$(CC) $(CFLAGS) -o a1 main.c $(COBJECTS) -I$(INCLUDE)

post: libstream.a post.o
	$(CC) $(CFLAGS) -o post post.o -L$(LPATH) $(LINK) $(MYSQL)

create:	tags.o generation.o
	$(CC) $(CFLAGS) -o create create.c tags.o generation.o -I$(INCLUDE)

view.py:
	chmod +x ./view.py

database: dbutils.o
	$(CC) $(CFLAGS) -o db db.c dbutils.o -I$(INCLUDE) $(MYSQL)

#
# Object rules:
#

driver.o: driver.c
	$(CC) $(CFLAGS) -o ./driver.o driver.c -c -I$(INCLUDE)

classToStruct.o: classToStruct.c
	$(CC) $(CFLAGS) -o ./classToStruct.o classToStruct.c -c -I$(INCLUDE)

vector.o: vector.c
	$(CC) $(CFLAGS) -o ./vector.o vector.c -c -I$(INCLUDE)

parseFile.o: parseFile.c
	$(CC) $(CFLAGS) -o ./parseFile.o parseFile.c -c -I$(INCLUDE)

recognize.o: recognize.c
	$(CC) $(CFLAGS) -o ./recognize.o recognize.c -c -I$(INCLUDE)

stream.o: stream.c
	$(CC) $(CFLAGS) -c stream.c -I$(INCLUDE) $(MYSQL)

post.o: precompile
	$(CC) $(CFLAGS) post.c -c -I$(INCLUDE) -L$(LPATH) $(LINK)

tags.o: tags.c tags.h
	$(CC) $(CFLAGS) tags.c -c -I$(INCLUDE)

generation.o: generation.c generation.h
	$(CC) $(CFLAGS) generation.c -c -I$(INCLUDE)

dbutils.o: dbutils.c dbutils.h
	$(CC) $(CFLAGS) dbutils.c -c -I$(INCLUDE)

#
#	c++ conversion rule
#
precompile: $(COBJECTS) post.cc
	$(CCON) post.cc

#
# Clean-up rules.  Removes the executable and objects.
#

deploy:
	scp -r *.c gsoverni@2750web.socs.uoguelph.ca:/srv/www/gsoverni
	scp -r *.h gsoverni@2750web.socs.uoguelph.ca:/srv/www/gsoverni
	scp -r *.cc gsoverni@2750web.socs.uoguelph.ca:/srv/www/gsoverni
	scp -r *.py gsoverni@2750web.socs.uoguelph.ca:/srv/www/gsoverni
	scp -r *.css gsoverni@2750web.socs.uoguelph.ca:/srv/www/gsoverni
	#scp -r *.jpg gsoverni@2750web.socs.uoguelph.ca:/srv/www/gsoverni
	scp -r Makefile gsoverni@2750web.socs.uoguelph.ca:/srv/www/gsoverni
	scp -r *.wpml gsoverni@2750web.socs.uoguelph.ca:/srv/www/gsoverni
	scp -r *.html gsoverni@2750web.socs.uoguelph.ca:/srv/www/gsoverni
	scp -r readme.txt gsoverni@2750web.socs.uoguelph.ca:/srv/www/gsoverni
	scp -r *.php gsoverni@2750web.socs.uoguelph.ca:/srv/www/gsoverni
	scp -r *.txt gsoverni@2750web.socs.uoguelph.ca:/srv/www/gsoverni
	ssh gsoverni@2750web.socs.uoguelph.ca "cd /srv/www/gsoverni && mkdir -p ./bin && make"

close:
	cp * /var/www/html/gsoverni

clean:
	rm $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC$) $(CCON) 

clobber:
	rm $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4) $(CCON) $(COBJECTS) $(OBJECTS) libstream.a post.c
