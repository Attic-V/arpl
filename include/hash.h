#ifndef hash_h
#define hash_h

#include <stddef.h>
#include <stdint.h>

/**
 * FNV-1a
 */
uint32_t getHash (void *data, size_t length);

#endif
