#include "ir.h"
#include "memory.h"

Ir *ir_initAssign (char *var, int32_t value)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Assign;
	ir->as.assign.var = var;
	ir->as.assign.value = value;
	ir->next = NULL;
	ir->previous = NULL;
	return ir;
}
