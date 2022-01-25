
#include "assert.h"

const struct exception assert_failure_exception = { "Assertion failed" };

void (assert)(int e) {
    assert(e);
}
