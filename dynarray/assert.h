
#ifndef ASSERT_H
#define ASSERT_H

#undef assert
#ifdef NDEBUG
#define assert(e) ((void)0)
#else
#include "exception.h"
void assert(int e);
#define assert(e) ((void) ((e) || (THROW (assert_failure_exception), 0)))
#endif
#endif
