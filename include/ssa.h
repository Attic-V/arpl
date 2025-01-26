#ifndef ssa_h
#define ssa_h

#include <stdint.h>

#include "arena.h"

typedef struct {
	char *var;
	int32_t value;
} SsaInstruction;

typedef struct {
	SsaInstruction *next;
} Ssa;

Ssa *ssa_init (Arena *arena);
SsaInstruction *ssa_init_assign (Arena *arena, char *var, int32_t value);
void ssa_set_head (Ssa *ssa, SsaInstruction *head);

#endif
