#include "ir.h"
#include "linked_list.h"
#include "memory.h"

IrAdd *irAdd_init (void);
IrAnd *irAnd_init (void);
IrEqu *irEqu_init (void);
IrMul *irMul_init (void);
IrNeg *irNeg_init (void);
IrNot *irNot_init (void);
IrNotEqu *irNotEqu_init (void);
IrPush *irPush_init (int32_t value);
IrSub *irSub_init (void);

Ir *ir_initAdd (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Add;
	ir->as.add = irAdd_init();
	dll_init(ir);
	return ir;
}

Ir *ir_initAnd (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_And;
	ir->as.and = irAnd_init();
	dll_init(ir);
	return ir;
}

Ir *ir_initEqu (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Equ;
	ir->as.equ = irEqu_init();
	dll_init(ir);
	return ir;
}

Ir *ir_initMul (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Mul;
	ir->as.mul = irMul_init();
	dll_init(ir);
	return ir;
}

Ir *ir_initNeg (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Neg;
	ir->as.neg = irNeg_init();
	dll_init(ir);
	return ir;
}

Ir *ir_initNot (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Not;
	ir->as.not = irNot_init();
	dll_init(ir);
	return ir;
}

Ir *ir_initNotEqu (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_NotEqu;
	ir->as.notEqu = irNotEqu_init();
	dll_init(ir);
	return ir;
}

Ir *ir_initPush (int32_t value)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Push;
	ir->as.push = irPush_init(value);
	dll_init(ir);
	return ir;
}

Ir *ir_initSub (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Sub;
	ir->as.sub = irSub_init();
	dll_init(ir);
	return ir;
}

IrAdd *irAdd_init (void)
{
	IrAdd *add = mem_alloc(sizeof(*add));
	return add;
}

IrAnd *irAnd_init (void)
{
	IrAnd *and = mem_alloc(sizeof(*and));
	return and;
}

IrEqu *irEqu_init (void)
{
	IrEqu *equ = mem_alloc(sizeof(*equ));
	return equ;
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

IrNot *irNot_init (void)
{
	IrNot *not = mem_alloc(sizeof(*not));
	return not;
}

IrNotEqu *irNotEqu_init (void)
{
	IrNotEqu *notEqu = mem_alloc(sizeof(*notEqu));
	return notEqu;
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
