CFLAG = -std=gnu99 -pedantic -Wall -Werror -O3
CC = gcc

matrix: matrix.c
	$(CC) $(CFLAG) -o matrix matrix.c

.PHONY: clean
clean:
	rm -f matrix *~