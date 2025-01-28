#ifndef arena_h
#define arena_h

#include <stddef.h>

typedef struct Arena Arena;

Arena *arena_init (size_t size);
void arena_free (Arena *arena);
void *arena_allocate (Arena *arena, size_t size);
void *arena_reallocate (Arena *arena, void *ptr, size_t size);

#endif
