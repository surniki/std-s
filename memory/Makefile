.POSIX:

CC = cc
CFLAGS = -std=c99 -Wall -Werror -Wpedantic -Wextra -g

all: test_memory

test_memory: test_memory.c memory.c memory.h assert.c assert.h exception.c exception.h test.o
	$(CC) $(CFLAGS) -DDEFINE_TESTS -o test_memory test_memory.c test.o

test.o: test.c test.h
	$(CC) $(CFLAGS) -c test.c

clean:
	rm -f test_memory
	rm -f test.o
