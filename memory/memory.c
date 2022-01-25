
#include "exception.h"
#include "assert.h"
#include "memory.h"

const struct exception memory_failure_exception = { "Memory failure" };

#ifdef NCHECKMEMORY

#include <stdlib.h>
#include <stddef.h>
#include "assert.h"
#include "exception.h"
#include "memory.h"

void *memory_alloc_f(long size, const char *function, const char *file, int line)
{
    assert(size > 0);
    assert(function);
    assert(file);
    assert(line > 0);

    void *result = malloc(size);
    if (!result) 
        exception_throw(&memory_failure_exception, function, file, line);

    return result;
}

void *memory_calloc_f(long count, long elem_size, const char *function, const char *file, int line)
{
    assert(count > 0);
    assert(elem_size > 0);
    assert(function);
    assert(file);
    assert(line > 0);

    void *result = calloc(count, elem_size);
    if (!result)
        exception_throw(&memory_failure_exception, function, file, line);

    return result;
}

void memory_free_f(void *p, const char *function, const char *file, int line)
{
    if (p)
        free(p);
}

void *memory_resize_f(void *p, long size, const char *function, const char *file, int line)
{
    assert(p);
    assert(size > 0);
    assert(function);
    assert(file);
    assert(line > 0);

    p = realloc(p, size);
    if (!p)
        exception_throw(&memory_failure_exception, function, file, line);

    return p;
}

#else

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "assert.h"
#include "exception.h"
#include "memory.h"

#define MEMORY_DESCRIPTOR_TABLE_SIZE 2048

union align {
    int i;
    long l;
    long *lp;
    void *p;
    void (*fp)(void);
    float f;
    double d;
    long double ld;
};

static struct descriptor {
    struct descriptor *free;
    struct descriptor *link;
    const void *p;
    long size;
    const char *function;
    const char *file;
    int line;
} *hash_table[MEMORY_DESCRIPTOR_TABLE_SIZE];

static struct descriptor freelist = { .free = &freelist };

/* Here's a note to feel better about this hash function working:
 *
 *  Say the pointer p is 32 bits.
 *
 *  x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x 
 *
 *  Then, if we shift it to the right by three, 
 *
 *  0 0 0 x x x x x x x x x x x x x x x x x x x x x x x x x x x x x 
 * 
 *  For the right hand side of the bitwise and expression, we have the
 *  length of the array minus 1.  If the array length is some power of
 *  2, then the expression on the right hand side is going to be equal
 *  to (n - 1) set bits starting from the least significant bit.  For
 *  example, if the array size is 2048, then that becomes log_2 2048 -
 *  1 = 10 set bits.
 *  
 *  0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 
 *  
 *  Applying the bitwise &, we select what are essentially 10
 *  psuedorandom bits that are uniformly distributed. The range of
 *  indices produced by this is valid for the array [0, 2047]. Since
 *  this is a function that maps pointers to indices of the array, it
 *  fulfills the needs of a hash function for the hash table.
 *
 *  0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 x x x x x x x x x x x 
 */
#define hash(p, table) (((unsigned long)(p)>>3) & ((sizeof (table)/sizeof ((table)[0])) - 1))

static struct descriptor *find(const void *p)
{
    struct descriptor *desc = hash_table[hash(p, hash_table)];

    while (desc && desc->p != p)
        desc = desc->link;

    return desc;
}

#define DESCRIPTOR_CHUNK_SIZE 512

#define MEMORY_BLOCK_ADDITIONAL_SIZE ((4096 + sizeof (union align) - 1) / (sizeof (union align)) * (sizeof (union align)))

static struct descriptor *dalloc(void *p, long size, const char *function, const char *file, int line)
{
    static struct descriptor *desc;
    static int avail;

    if (avail <= 0) {
        desc = malloc(DESCRIPTOR_CHUNK_SIZE * sizeof (*desc));
        if (!desc)
            return NULL;
        
        avail = DESCRIPTOR_CHUNK_SIZE;
    }

    desc->p = p;
    desc->size = size;
    desc->function = function;
    desc->file = file;
    desc->line = line;
    desc->free = desc->link = NULL;

    avail--;
    return desc++;
}

void *memory_alloc_f(long size, const char *function, const char *file, int line)
{
    assert(size > 0);
    assert(function);
    assert(file);
    assert(line > 0);

    void *p;
    
    size = ((size + sizeof (union align) - 1) / (sizeof (union align))) * (sizeof (union align));

    for (struct descriptor *desc = freelist.free; desc; desc = desc->free) {
        if (desc->size > size) {
            desc->size -= size;
            p = (char *)desc->p + desc->size;
            if ((desc = dalloc(p, size, function, file, line))) {
                unsigned i = hash(p, hash_table);
                desc->link = hash_table[i];
                hash_table[i] = desc;
                return p;
            }
            else
                exception_throw(&memory_failure_exception, function, file, line);
        }
        if (desc == &freelist) {
            struct descriptor *newp;
            p = malloc(size + MEMORY_BLOCK_ADDITIONAL_SIZE);
            if (!p)
                exception_throw(&memory_failure_exception, function, file, line);

            
            if (!(p = malloc(size + MEMORY_BLOCK_ADDITIONAL_SIZE)) || !(newp = dalloc(p, size + MEMORY_BLOCK_ADDITIONAL_SIZE, function, file, line)))

            newp->free = freelist.free;
            freelist.free = newp;
        }
    }

    assert(0);
    return NULL;
}

void *memory_calloc_f(long count, long elem_size, const char *function, const char *file, int line)
{
    assert(count > 0);
    assert(elem_size > 0);
    assert(function);
    assert(file);
    assert(line > 0);

    void *p = memory_alloc_f(count * elem_size, function, file, line);
    memset(p, '\0', count * elem_size);

    return p;
}

void *memory_resize_f(void *p, long size, const char *function, const char *file, int line)
{
    assert(p);
    assert(size > 0);
    assert(function);
    assert(file);
    assert(line > 0);

    struct descriptor *desc;

    if (((unsigned long)p) % (sizeof (union align)) != 0 || !(desc = find(p)) || desc->free)
        exception_throw(&memory_failure_exception, function, file, line);
    
    void *newp = memory_alloc_f(size, function, file, line);
    memcpy(newp, p, size < desc->size ? size : desc->size);
    memory_free_f(p, function, file, line);
    return newp;
}

void memory_free_f(void *p, const char *function, const char *file, int line)
{
    if (p) {
        struct descriptor *desc;

        if (((unsigned long)p) % (sizeof (union align)) != 0 || !(desc = find(p)) || desc->free)
            exception_throw(&memory_failure_exception, function, file, line);
        
        desc->free = freelist.free;
        freelist.free = desc;
    }
}

#endif
