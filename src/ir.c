#include "ir.h"
#include "linked_list.h"
#include "memory.h"

static IrAdd *irAdd_init (void);
static IrAnd *irAnd_init (void);
static IrAssign *irAssign_init (void);
static IrEqu *irEqu_init (void);
static IrInc *irInc_init (void);
static IrJmp *irJmp_init (int n);
static IrJmpFalse *irJmpFalse_init (int n);
static IrJmpTrue *irJmpTrue_init (int n);
static IrLabel *irLabel_init (int n);
static IrLess *irLess_init (void);
static IrLessEqu *irLessEqu_init (void);
static IrMul *irMul_init (void);
static IrNeg *irNeg_init (void);
static IrNot *irNot_init (void);
static IrNotEqu *irNotEqu_init (void);
static IrOr *irOr_init (void);
static IrPop *irPop_init (void);
static IrPush *irPush_init (int32_t value);
static IrRef *irRef_init (size_t idx);
static IrReserve *irReserve_init (size_t bytes);
static IrSar *irSar_init (void);
static IrShl *irShl_init (void);
static IrSub *irSub_init (void);
static IrVal *irVal_init (size_t idx);
static IrXor *irXor_init (void);

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

Ir *ir_initAssign (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Assign;
	ir->as.assign = irAssign_init();
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

Ir *ir_initInc (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Inc;
	ir->as.inc = irInc_init();
	dll_init(ir);
	return ir;
}

Ir *ir_initJmp (int n)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Jmp;
	ir->as.jmp = irJmp_init(n);
	dll_init(ir);
	return ir;
}

Ir *ir_initJmpFalse (int n)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_JmpFalse;
	ir->as.jmpFalse = irJmpFalse_init(n);
	dll_init(ir);
	return ir;
}

Ir *ir_initJmpTrue (int n)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_JmpTrue;
	ir->as.jmpTrue = irJmpTrue_init(n);
	dll_init(ir);
	return ir;
}

Ir *ir_initLabel (int n)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Label;
	ir->as.label = irLabel_init(n);
	dll_init(ir);
	return ir;
}

Ir *ir_initLess (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Less;
	ir->as.less = irLess_init();
	dll_init(ir);
	return ir;
}

Ir *ir_initLessEqu (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_LessEqu;
	ir->as.lessEqu = irLessEqu_init();
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

Ir *ir_initOr (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Or;
	ir->as.or = irOr_init();
	dll_init(ir);
	return ir;
}

Ir *ir_initPop (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Pop;
	ir->as.pop = irPop_init();
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

Ir *ir_initRef (size_t idx)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Ref;
	ir->as.ref = irRef_init(idx);
	dll_init(ir);
	return ir;
}

Ir *ir_initReserve (size_t bytes)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Reserve;
	ir->as.reserve = irReserve_init(bytes);
	dll_init(ir);
	return ir;
}

Ir *ir_initSar (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Sar;
	ir->as.sar = irSar_init();
	dll_init(ir);
	return ir;
}

Ir *ir_initShl (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Shl;
	ir->as.shl = irShl_init();
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

Ir *ir_initVal (size_t idx)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Val;
	ir->as.val = irVal_init(idx);
	dll_init(ir);
	return ir;
}

Ir *ir_initXor (void)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = Ir_Xor;
	ir->as.xor = irXor_init();
	dll_init(ir);
	return ir;
}

static IrAdd *irAdd_init (void)
{
	IrAdd *add = mem_alloc(sizeof(*add));
	return add;
}

static IrAnd *irAnd_init (void)
{
	IrAnd *and = mem_alloc(sizeof(*and));
	return and;
}

static IrAssign *irAssign_init (void)
{
	IrAssign *assign = mem_alloc(sizeof(*assign));
	return assign;
}

static IrEqu *irEqu_init (void)
{
	IrEqu *equ = mem_alloc(sizeof(*equ));
	return equ;
}

static IrInc *irInc_init (void)
{
	IrInc *inc = mem_alloc(sizeof(*inc));
	return inc;
}

static IrJmp *irJmp_init (int n)
{
	IrJmp *jmp = mem_alloc(sizeof(*jmp));
	jmp->n = n;
	return jmp;
}

static IrJmpFalse *irJmpFalse_init (int n)
{
	IrJmpFalse *jmpFalse = mem_alloc(sizeof(*jmpFalse));
	jmpFalse->n = n;
	return jmpFalse;
}

static IrJmpTrue *irJmpTrue_init (int n)
{
	IrJmpTrue *jmpTrue = mem_alloc(sizeof(*jmpTrue));
	jmpTrue->n = n;
	return jmpTrue;
}

static IrLabel *irLabel_init (int n)
{
	IrLabel *label = mem_alloc(sizeof(*label));
	label->n = n;
	return label;
}

static IrLess *irLess_init (void)
{
	IrLess *less = mem_alloc(sizeof(*less));
	return less;
}

static IrLessEqu *irLessEqu_init (void)
{
	IrLessEqu *lessEqu = mem_alloc(sizeof(*lessEqu));
	return lessEqu;
}

static IrMul *irMul_init (void)
{
	IrMul *mul = mem_alloc(sizeof(*mul));
	return mul;
}

static IrNeg *irNeg_init (void)
{
	IrNeg *neg = mem_alloc(sizeof(*neg));
	return neg;
}

static IrNot *irNot_init (void)
{
	IrNot *not = mem_alloc(sizeof(*not));
	return not;
}

static IrNotEqu *irNotEqu_init (void)
{
	IrNotEqu *notEqu = mem_alloc(sizeof(*notEqu));
	return notEqu;
}

static IrOr *irOr_init (void)
{
	IrOr *or = mem_alloc(sizeof(*or));
	return or;
}

static IrPop *irPop_init (void)
{
	IrPop *pop = mem_alloc(sizeof(*pop));
	return pop;
}

static IrPush *irPush_init (int32_t value)
{
	IrPush *push = mem_alloc(sizeof(*push));
	push->value = value;
	return push;
}

static IrRef *irRef_init (size_t idx)
{
	IrRef *ref = mem_alloc(sizeof(*ref));
	ref->idx = idx;
	return ref;
}

static IrReserve *irReserve_init (size_t bytes)
{
	IrReserve *reserve = mem_alloc(sizeof(*reserve));
	reserve->bytes = bytes;
	return reserve;
}

static IrSar *irSar_init (void)
{
	IrSar *sar = mem_alloc(sizeof(*sar));
	return sar;
}

static IrShl *irShl_init (void)
{
	IrShl *shl = mem_alloc(sizeof(*shl));
	return shl;
}

static IrSub *irSub_init (void)
{
	IrSub *sub = mem_alloc(sizeof(*sub));
	return sub;
}

static IrVal *irVal_init (size_t idx)
{
	IrVal *val = mem_alloc(sizeof(*val));
	val->idx = idx;
	return val;
}

static IrXor *irXor_init (void)
{
	IrXor *xor = mem_alloc(sizeof(*xor));
	return xor;
}
