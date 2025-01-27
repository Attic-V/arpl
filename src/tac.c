#include "tac.h"

Tac *tac_initAssign (Arena *arena, char *var, int32_t value)
{
	Tac *tac = arena_allocate(arena, sizeof(*tac));
	tac->type = Tac_Assign;
	tac->as.assign.var = var;
	tac->as.assign.value = value;
	tac->next = NULL;
	tac->previous = NULL;
	return tac;
}
