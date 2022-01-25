
#ifndef MEMORY_H
#define MEMORY_H

extern const struct exception memory_failure_exception;

void *memory_alloc_f(long size, const char *function, const char *file, int line);
void *memory_calloc_f(long count, long elem_size, const char *function, const char *file, int line);
void memory_free_f(void *p, const char *function, const char *file, int line);
void *memory_resize_f(void *p, long size, const char *function, const char *file, int line);

#define memory_alloc(size) memory_alloc_f((size), __func__, __FILE__, __LINE__)
#define memory_calloc(count, elem_size) memory_calloc_f((count), (elem_size), __func__, __FILE__, __LINE__)
#define memory_new(p) (memory_alloc((long)sizeof *(p)))
#define memory_new0(p) (memory_calloc(1, (long)sizeof *(p)))
#define memory_free(p) ((void)(memory_free_f((p), __func__, __FILE__, __LINE__), (p) = ((void *)0)))
#define memory_resize(p, size) ((p) = memory_resize_f((p), (size), __func__, __FILE__, __LINE__))

#endif
