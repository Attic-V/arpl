#include "ssa.h"

Ssa *ssa_init (Arena *arena)
{
	Ssa *ssa = arena_allocate(arena, sizeof(*ssa));
	return ssa;
}

SsaInstruction *ssa_init_assign (Arena *arena, char *var, int32_t value)
{
	SsaInstruction *ssa = arena_allocate(arena, sizeof(*ssa));
	ssa->var = var;
	ssa->value = value;
	return ssa;
}

void ssa_set_head (Ssa *ssa, SsaInstruction *head)
{
	ssa->next = head;
}
