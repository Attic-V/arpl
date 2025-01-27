#include "ssa_generator.h"

Ssa *ssa_generate (Arena *arena, Tac *tac)
{
	return ssa_initAssign(arena, tac->as.assign.var, tac->as.assign.value);
}
