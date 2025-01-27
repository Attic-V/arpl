#include "ssa_generator.h"

Ssa *ssa_generate (Arena *arena, Tac *tac)
{
	Ssa *ssa = ssa_init(arena);
	SsaInstruction *assign = ssa_init_assign(arena, tac->as.assign.var, tac->as.assign.value);
	ssa_set_head(ssa, assign);
	return ssa;
}
