#include "ir.h"
#include "memory.h"

IrPush *irPush_init (int32_t value);

Ir *ir_initPush (int32_t value)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Push;
	ir->as.push = irPush_init(value);
	ir->next = NULL;
	ir->previous = NULL;
	return ir;
}

IrPush *irPush_init (int32_t value)
{
	IrPush *assign = mem_alloc(sizeof(*assign));
	assign->value = value;
	return assign;
}
