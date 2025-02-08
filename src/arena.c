#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"
#include "linked_list.h"

typedef struct Region Region;

struct Region {
	void *memory;
	void *previousMem;
	size_t offset;
	size_t size;
	Region *next;
	Region *previous;
};

struct Arena {
	Region *region;
	size_t regionsize;
};

static void *arena_allocateRaw (Arena *arena, size_t size);
static void *arena_allocateSegment (Arena *arena, size_t size);

static Region *region_init (size_t size);
static void *region_allocate (Region *region, size_t size);
static void *region_allocateSegment (Region *region, size_t size);
static void *region_allocateRaw (Region *region, size_t size);
static void *region_reallocate (Region *region, void *ptr, size_t size);
static void region_releaseSegment (Region *region, size_t size);
static void region_release (Region *region, size_t size);
static size_t region_getFree (Region *region);

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
	for (Region *region = arena->region; region != NULL; region = region->previous) {
		free(region->memory);
		free(region);
	}
	free(arena);
}

void *arena_allocate (Arena *arena, size_t size)
{
	return arena_allocateSegment(arena, size);
}

static void *arena_allocateSegment (Arena *arena, size_t size)
{
	*(size_t *)arena_allocateRaw(arena, sizeof(size)) = size;
	return arena_allocateRaw(arena, size);
}

void *arena_reallocate (Arena *arena, void *ptr, size_t size)
{
	return region_reallocate(arena->region, ptr, size);
}

static void *arena_allocateRaw (Arena *arena, size_t size)
{
	if (size > region_getFree(arena->region)) {
		Region *region = region_init(arena->regionsize);
		dll_insert(arena->region, region);
		arena->region = region;
	}
	void *ptr = region_allocateRaw(arena->region, size);
	return ptr;
}

static Region *region_init (size_t size)
{
	Region *region = malloc(sizeof(*region));
	region->memory = malloc(size);
	region->previousMem = NULL;
	region->offset = 0;
	region->size = size;
	dll_init(region);
	return region;
}

static void *region_allocate (Region *region, size_t size)
{
	return region_allocateSegment(region, size);
}

static void *region_allocateSegment (Region *region, size_t size)
{
	*(size_t *)region_allocateRaw(region, sizeof(size)) = size;
	return region_allocateRaw(region, size);
}

static void *region_allocateRaw (Region *region, size_t size)
{
	region->previousMem = region->memory + region->offset;
	if (region->offset + size > region->size) {
		return NULL;
	}
	void *ptr = region->memory + region->offset;
	region->offset += size;
	return ptr;
}

static void *region_reallocate (Region *region, void *ptr, size_t size)
{
	size_t oldsize = *(size_t *)(ptr - sizeof(oldsize));
	if (size < oldsize) {
		return ptr;
	}
	if (region->previousMem == ptr) {
		region_releaseSegment(region, oldsize);
		return region_allocate(region, size);
	}
	void *newptr = region_allocate(region, size);
	memcpy(newptr, ptr, oldsize);
	return newptr;
}

static void region_releaseSegment (Region *region, size_t size)
{
	region_release(region, size + sizeof(size));
}

static void region_release (Region *region, size_t size)
{
	region->offset -= size;
}

static size_t region_getFree (Region *region)
{
	return region->size - region->offset;
}
