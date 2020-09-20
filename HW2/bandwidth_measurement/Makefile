CFLAG = -std=gnu99 -pedantic -Wall -Werror -O3
CC = gcc

bandwidth_gauger: bandwidth_gauger.c
	$(CC) $(CFLAG) -o bandwidth_gauger bandwidth_gauger.c

.PHONY: clean
clean:
	rm -f bandwidth_gauger *~