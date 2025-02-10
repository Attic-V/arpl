#include "hash.h"

uint32_t getHash (void *data, size_t length)
{
	#define FNV_OFFSET 2166136261
	#define FNV_PRIME 16777619

	uint32_t hash = FNV_OFFSET;
	for (int i = 0; i < length; i++) {
		hash ^= ((char *)data)[i];
		hash *= FNV_PRIME;
	}
	return hash;

	#undef FNV_OFFSET
	#undef FNV_PRIME
}
