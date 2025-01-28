#include "ir.h"
#include "memory.h"

IrAssign *irAssign_init (char *var, int32_t value);

Ir *ir_initAssign (char *var, int32_t value)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Assign;
	ir->as.assign = irAssign_init(var, value);
	ir->next = NULL;
	ir->previous = NULL;
	return ir;
}

IrAssign *irAssign_init (char *var, int32_t value)
{
	IrAssign *assign = mem_alloc(sizeof(*assign));
	assign->var = var;
	assign->value = value;
	return assign;
}
