CC=gcc
CFLAGS=-g -Wall
LFLAGS=-pthread

all: treeMTTest

treeMT.o: treeMT.c
	$(CC) $(CFLAGS) -c $< -o $@

treeMTTest: treeMTTest.c treeMT.o
	$(CC) $(CFLAGS) $< treeMT.o -o $@ $(LFLAGS)

clean:
	rm -rf treeMT treeMTTest *.o

