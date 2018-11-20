CC=gcc
OBJ=main.o blockmatrix.o debug.o
MODE=INBLOCK_PARALLEL
CFLAGS=-fopenmp -O3 -Wall -D$(MODE)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

maketest: $(OBJ)
	$(CC) -fopenmp -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJ) maketest
