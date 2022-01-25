
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "exception.h"

struct exception_frame *exception_stack = NULL;

void exception_throw(const struct exception *e, const char *function, const char *file, int line)
{
	assert(e);

	struct exception_frame *p = exception_stack;

	if (p == NULL) {
		fprintf(stderr, "Uncaught exception");

		if (e->message)
			fprintf(stderr, " [ %s ] ", e->message);
		else
			fprintf(stderr, " at 0x%p", (void *)e);
        
		if (function && file && line > 0)
			fprintf(stderr, "thrown in function %s at %s:%d\n", function, file, line);

		fprintf(stderr, "Aborting...\n");
		fflush(stderr);
		abort();
	}

	p->exception = e;
	p->file = file;
	p->line = line;
	p->function = function;
	exception_stack = exception_stack->prev;
	longjmp(p->env, EXCEPTION_FLAG_THROWN);
}
