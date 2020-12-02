#----- Modify CC and CFLAGS as appropriate in your environment
CC = gcc
CFLAGS = -O3 -fopenmp

LIB = -lm -lc

UNITFILES = 

#------------ EXECUTABLE FILES ---------------

all: histo histo_locks histo_atomic histo_creative

histo_locks: histo_locks.c
	$(CC) $(CFLAGS) -o histo_locks histo_locks.c $(LIB)

histo_atomic: histo_atomic.c
	$(CC) $(CFLAGS) -o histo_atomic histo_atomic.c $(LIB)

histo_creative: histo_creative.c
	$(CC) $(CFLAGS) -o histo_creative histo_creative.c $(LIB)

histo : histo.o $(UNITFILES)
	$(CC) $(CFLAGS) -o histo histo.o $(UNITFILES) $(LIB)

histo.o : histo.c
	$(CC) $(CFLAGS) -c histo.c $(INCLUDE) 

clean:
	rm -f histo *.o histo_locks histo_atomic histo_creative
