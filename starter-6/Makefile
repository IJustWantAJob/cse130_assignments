CC = cc
CFLAGS= -g

all : pclock

pclock: guess-mt.o pclock.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

%.o : %.c
	$(CC) -c $(CFLAGS) $<
clean: 
	rm -rf *.o *~ pclock
