#include "ir.h"
#include "memory.h"

IrAdd *irAdd_init (void);
IrMul *irMul_init (void);
IrNeg *irNeg_init (void);
IrPush *irPush_init (int32_t value);
IrSub *irSub_init (void);

Ir *ir_initAdd (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Add;
	ir->as.add = irAdd_init();
	ir->next = NULL;
	ir->previous = NULL;
	return ir;
}

Ir *ir_initMul (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Mul;
	ir->as.mul = irMul_init();
	ir->next = NULL;
	ir->previous = NULL;
	return ir;
}

Ir *ir_initNeg (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Neg;
	ir->as.neg = irNeg_init();
	ir->next = NULL;
	ir->previous = NULL;
	return ir;
}

Ir *ir_initPush (int32_t value)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Push;
	ir->as.push = irPush_init(value);
	ir->next = NULL;
	ir->previous = NULL;
	return ir;
}

Ir *ir_initSub (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Sub;
	ir->as.sub = irSub_init();
	ir->next = NULL;
	ir->previous = NULL;
	return ir;
}

IrAdd *irAdd_init (void)
{
	IrAdd *add = mem_alloc(sizeof(*add));
	return add;
}

IrMul *irMul_init (void)
{
	IrMul *mul = mem_alloc(sizeof(*mul));
	return mul;
}

IrNeg *irNeg_init (void)
{
	IrNeg *neg = mem_alloc(sizeof(*neg));
	return neg;
}

IrPush *irPush_init (int32_t value)
{
	IrPush *push = mem_alloc(sizeof(*push));
	push->value = value;
	return push;
}

IrSub *irSub_init (void)
{
	IrSub *sub = mem_alloc(sizeof(*sub));
	return sub;
}
