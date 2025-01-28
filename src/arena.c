#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"

typedef struct Region Region;

struct Region {
	void *memory;
	void *previous;
	size_t offset;
	size_t size;
	Region *before;
};

struct Arena {
	Region *region;
	size_t regionsize;
};

void *arena_allocateRaw (Arena *arena, size_t size);
void *arena_allocateSegment (Arena *arena, size_t size);

Region *region_init (size_t size);
void *region_allocate (Region *region, size_t size);
void *region_allocateSegment (Region *region, size_t size);
void *region_allocateRaw (Region *region, size_t size);
void *region_reallocate (Region *region, void *ptr, size_t size);
void region_releaseSegment (Region *region, size_t size);
void region_release (Region *region, size_t size);
size_t region_getFree (Region *region);

Arena *arena_init (size_t size)
{
	Arena *arena = malloc(sizeof(*arena));
	if (arena == NULL) {
		return NULL;
	}
	arena->regionsize = size;
	arena->region = region_init(arena->regionsize);
	return arena;
}

void arena_free (Arena *arena)
{
	for (Region *region = arena->region; region != NULL; region = region->before) {
		free(region->memory);
		free(region);
	}
	free(arena);
}

void *arena_allocate (Arena *arena, size_t size)
{
	return arena_allocateSegment(arena, size);
}

void *arena_allocateSegment (Arena *arena, size_t size)
{
	*(size_t *)arena_allocateRaw(arena, sizeof(size)) = size;
	return arena_allocateRaw(arena, size);
}

void *arena_reallocate (Arena *arena, void *ptr, size_t size)
{
	return region_reallocate(arena->region, ptr, size);
}

void *arena_allocateRaw (Arena *arena, size_t size)
{
	if (size > region_getFree(arena->region)) {
		Region *region = region_init(arena->regionsize);
		region->before = arena->region;
		arena->region = region;
	}
	void *ptr = region_allocateRaw(arena->region, size);
	return ptr;
}

Region *region_init (size_t size)
{
	Region *region = malloc(sizeof(*region));
	region->memory = malloc(size);
	region->previous = NULL;
	region->offset = 0;
	region->size = size;
	region->before = NULL;
	return region;
}

void *region_allocate (Region *region, size_t size)
{
	return region_allocateSegment(region, size);
}

void *region_allocateSegment (Region *region, size_t size)
{
	*(size_t *)region_allocateRaw(region, sizeof(size)) = size;
	return region_allocateRaw(region, size);
}

void *region_allocateRaw (Region *region, size_t size)
{
	region->previous = region->memory + region->offset;
	if (region->offset + size > region->size) {
		return NULL;
	}
	void *ptr = region->memory + region->offset;
	region->offset += size;
	return ptr;
}

void *region_reallocate (Region *region, void *ptr, size_t size)
{
	size_t oldsize = *(size_t *)(ptr - sizeof(oldsize));
	if (size < oldsize) {
		return ptr;
	}
	if (region->previous == ptr) {
		region_releaseSegment(region, oldsize);
		return region_allocate(region, size);
	}
	void *newptr = region_allocate(region, size);
	memcpy(newptr, ptr, oldsize);
	return newptr;
}

void region_releaseSegment (Region *region, size_t size)
{
	region_release(region, size + sizeof(size));
}

void region_release (Region *region, size_t size)
{
	region->offset -= size;
}

size_t region_getFree (Region *region)
{
	return region->size - region->offset;
}
