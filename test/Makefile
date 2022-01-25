.POSIX:

CC = cc
CFLAGS = -std=c99 -Wall -Werror -Wpedantic -Wextra -g 

.PHONY: dirs

all: test_test

test_test:
	$(CC) $(CFLAGS) -DDEFINE_TESTS -o test_test test_test.c

clean:
	rm test_test
