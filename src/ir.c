#include "ir.h"

Ir *ir_initAssign (Arena *arena, char *var, int32_t value)
{
	Ir *ir = arena_allocate(arena, sizeof(*ir));
	ir->type = Ir_Assign;
	ir->as.assign.var = var;
	ir->as.assign.value = value;
	ir->next = NULL;
	ir->previous = NULL;
	return ir;
}
