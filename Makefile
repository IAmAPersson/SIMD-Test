CC=gcc
CFLAGS=-fopenmp -O2 -march=native

source: source.o
	$(CC) $(CFLAGS) -o source source.o