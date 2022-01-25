.POSIX:

CC = cc
CFLAGS = -std=c99 -g 

all: test_dynarray

test_dynarray: test_dynarray.o memory.o exception.o assert.o test.o
	$(CC) $(CFLAGS) -DDEFINE_TESTS -o test_dynarray test_dynarray.o memory.o exception.o assert.o test.o

test_dynarray.o: dynarray.h test_dynarray.c
	$(CC) $(CFLAGS) -DDEFINE_TESTS -c test_dynarray.c

memory.o: memory.h memory.c exception.h exception.c assert.h assert.c
	$(CC) $(CFLAGS) -c memory.c

exception.o: exception.h exception.c assert.h assert.c
	$(CC) $(CFLAGS) -c exception.c

assert.o: exception.h exception.c assert.h assert.c
	$(CC) $(CFLAGS) -c assert.c

test.o: test.h test.c
	$(CC) $(CFLAGS) -c test.c

clean:
	rm -f test_dynarray
	rm -f test_dynarray.o
	rm -f memory.o
	rm -f exception.o
	rm -f assert.o
	rm -f test.o

test: test_dynarray
	./test_dynarray
