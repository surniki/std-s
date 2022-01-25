
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <setjmp.h>

extern const struct exception assert_failure_exception;

struct exception {
    const char *message;
};

struct exception_frame {
    struct exception_frame *prev;
    jmp_buf env;
    const char *function;
    const char *file;
    int line;
    const struct exception *exception;
};

extern struct exception_frame *exception_stack;

void exception_throw(const struct exception *e, const char *function, const char *file, int line);

#define THROW(e) exception_throw((&e), __func__, __FILE__, __LINE__)
#define RAISE exception_throw(exception_frame.exception, exception_frame.function, exception_frame.file, exception_frame.line)

enum exception_flag {
    EXCEPTION_FLAG_ENTERED = 0,
    EXCEPTION_FLAG_THROWN,
    EXCEPTION_FLAG_HANDLED,
    EXCEPTION_FLAG_FINALIZED
};

#define TRY do {                                        \
    volatile enum exception_flag exception_flag;        \
    struct exception_frame exception_frame;             \
    exception_frame.prev = exception_stack;             \
    exception_stack = &exception_frame;                 \
    exception_flag = setjmp(exception_frame.env);       \
    if (exception_flag == EXCEPTION_FLAG_ENTERED) {

#define CATCH(e) if (exception_flag == EXCEPTION_FLAG_ENTERED) exception_stack = exception_stack->prev; \
    } else if (exception_frame.exception == &(e)) {                                                     \
    exception_flag = EXCEPTION_FLAG_HANDLED;

#define OTHERWISE if (exception_flag == EXCEPTION_FLAG_ENTERED) exception_stack = exception_stack->prev; \
    } else {                                                                                             \
    exception_flag = EXCEPTION_FLAG_HANDLED;

#define FINALLY if (exception_flag == EXCEPTION_FLAG_ENTERED) exception_stack = exception_stack->prev; \
    } {                                                                                                \
    if (exception_flag == EXCEPTION_FLAG_ENTERED)                                                      \
        exception_flag = EXCEPTION_FLAG_FINALIZED;

#define END_TRY if (exception_flag == EXCEPTION_FLAG_ENTERED) exception_stack = exception_stack->prev; \
    } if (exception_flag == EXCEPTION_FLAG_THROWN) RAISE;                                              \
    } while (0)
    
#endif
