.POSIX:

CC = cc
CFLAGS = -std=c99 -Wall -Werror -Wpedantic -Wextra -g

all: test_exception

test_exception: exception.c exception.h assert.c assert.h test_exception.c test.o
	$(CC) $(CFLAGS) -DDEFINE_TESTS -o test_exception test_exception.c test.o

test.o: test.c test.h
	$(CC) $(CFLAGS) -c test.c

clean:
	rm -f test_exception
	rm -f test.o
