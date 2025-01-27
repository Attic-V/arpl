#include "arena.h"
#include "data.h"
#include "memory.h"

#define ARENA_SIZE (4 * MB)

static Arena *arena = NULL;

void *mem_alloc (size_t size)
{
	if (arena == NULL) {
		arena = arena_init(ARENA_SIZE);
	}
	return arena_allocate(arena, size);
}

void *mem_realloc (void *ptr, size_t size)
{
	if (arena == NULL) {
		arena = arena_init(ARENA_SIZE);
	}
	return arena_reallocate(arena, ptr, size);
}

void mem_free (void)
{
	if (arena == NULL) {
		return;
	}
	arena_free(arena);
}
