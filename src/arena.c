#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"

void *arena_allocate_raw (Arena *arena, size_t size);
void arena_release_end (Arena *arena, size_t size);

struct Arena {
	void *memory;
	void *previous;
	size_t offset;
	size_t size;
};

Arena *arena_init (size_t size)
{
	Arena *arena = malloc(sizeof(*arena));
	if (arena == NULL) {
		return NULL;
	}
	arena->memory = malloc(size);
	if (arena->memory == NULL) {
		return NULL;
	}
	arena->previous = NULL;
	arena->offset = 0;
	arena->size = size;
	return arena;
}

void arena_free (Arena *arena)
{
	free(arena->memory);
	free(arena);
}

void *arena_allocate (Arena *arena, size_t size)
{
	*(size_t *)arena_allocate_raw(arena, sizeof(size)) = size;
	return arena_allocate_raw(arena, size);
}

void *arena_reallocate (Arena *arena, void *ptr, size_t size)
{
	size_t oldsize = *(size_t *)(ptr - sizeof(oldsize));
	if (size < oldsize) {
		return ptr;
	}
	if (arena->previous == ptr) {
		arena_release_end(arena, oldsize);
		arena_release_end(arena, sizeof(oldsize));
		return arena_allocate(arena, size);
	}
	void *newptr = arena_allocate(arena, size);
	memcpy(newptr, ptr, oldsize);
	return newptr;
}

void *arena_allocate_raw (Arena *arena, size_t size)
{
	arena->previous = arena->memory + arena->offset;
	if (arena->offset + size > arena->size) {
		return NULL;
	}
	void *ptr = arena->memory + arena->offset;
	arena->offset += size;
	return ptr;
}

void arena_release_end (Arena *arena, size_t size)
{
	arena->offset -= size;
}
