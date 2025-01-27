#ifndef memory_h
#define memory_h

#include <stddef.h>

void *mem_alloc (size_t size);
void *mem_realloc (void *ptr, size_t size);
void mem_free (void);

#endif
