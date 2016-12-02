CC=mpicc
CFLAGS= -g -Wall -o

bucketsort: bucketsort.c
	$(CC) $(CFLAGS) $@ $<

