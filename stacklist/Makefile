AS	=gas
LD	=gld
CC	=gcc
CFLAGS	=

all:	sltest
objects = staticlist.o sltest.o

sltest: $(objects)
	$(CC) $(objects) -o sltest

staticlist.o: staticlist.h

sltest.o: staticlist.h
	$(CC) -c sltest.c

clean:
	rm sltest *.o

