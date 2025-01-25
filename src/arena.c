#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"

#define ARENA_MEMORY_SIZE (4 * 1024 * 1024)

struct Arena {
	void *memory;
	size_t offset;
};

Arena *arena_init (void)
{
	Arena *arena = malloc(sizeof(Arena));
	if (arena == NULL) {
		return NULL;
	}
	arena->memory = malloc(ARENA_MEMORY_SIZE);
	if (arena->memory == NULL) {
		return NULL;
	}
	arena->offset = 0;
	return arena;
}

void arena_free (Arena *arena)
{
	free(arena->memory);
	free(arena);
}

void *arena_allocate (Arena *arena, size_t size)
{
	if (arena->offset + size > ARENA_MEMORY_SIZE) {
		return NULL;
	}
	void *ptr = arena->memory + arena->offset;
	arena->offset += size;
	return ptr;
}

void *arena_reallocate (Arena *arena, void *ptr, size_t size)
{
	void *new_ptr = arena_allocate(arena, size);
	memcpy(new_ptr, ptr, size);
	return ptr;
}
