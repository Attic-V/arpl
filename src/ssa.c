#include "ssa.h"

Ssa *ssa_initAssign (Arena *arena, char *var, int32_t value)
{
	Ssa *ssa = arena_allocate(arena, sizeof(*ssa));
	ssa->type = Ssa_Assign;
	ssa->as.assign.var = var;
	ssa->as.assign.value = value;
	ssa->next = NULL;
	ssa->previous = NULL;
	return ssa;
}
