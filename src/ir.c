#include "ir.h"
#include "linked_list.h"
#include "memory.h"

static Ir *ir_init (IrType type);

static IrAccess *irAccess_init (size_t index);
static IrAdd *irAdd_init (size_t size);
static IrAnd *irAnd_init (size_t size);
static IrAssign *irAssign_init (size_t size);
static IrCall *irCall_init (void);
static IrCast *irCast_init (DataType *from, DataType *to);
static IrCopy *irCopy_init (void);
static IrDec *irDec_init (size_t size);
static IrDeref *irDeref_init (size_t size);
static IrEqu *irEqu_init (size_t size);
static IrFnRef *irFnRef_init (Token identifier);
static IrFunctionEnd *irFunctionEnd_init (void);
static IrFunctionStart *irFunctionStart_init (Token identifier);
static IrInc *irInc_init (size_t size);
static IrJmp *irJmp_init (int n);
static IrJmpFalse *irJmpFalse_init (int n);
static IrJmpTrue *irJmpTrue_init (int n);
static IrLabel *irLabel_init (int n);
static IrLess *irLess_init (size_t size);
static IrLessEqu *irLessEqu_init (size_t size);
static IrMul *irMul_init (size_t size);
static IrNeg *irNeg_init (size_t size);
static IrNot *irNot_init (size_t size);
static IrNotEqu *irNotEqu_init (size_t size);
static IrOr *irOr_init (size_t size);
static IrParameter *irParameter_init (size_t pIdx, size_t vIdx, size_t size);
static IrPop *irPop_init (void);
static IrPush *irPush_init (int32_t value);
static IrRef *irRef_init (size_t idx);
static IrReserve *irReserve_init (size_t bytes);
static IrRestore *irRestore_init (void);
static IrRet *irRet_init (void);
static IrSar *irSar_init (size_t size);
static IrShl *irShl_init (size_t size);
static IrStore *irStore_init (void);
static IrSub *irSub_init (size_t size);
static IrVal *irVal_init (size_t idx, size_t size);
static IrXor *irXor_init (size_t size);

static Ir *ir_init (IrType type)
{
	Ir *ir = mem_alloc(sizeof(*ir));
	ir->type = type;
	dll_init(ir);
	return ir;
}

Ir *ir_initAccess (size_t index)
{
	Ir *ir = ir_init(Ir_Access);
	ir->as.access = irAccess_init(index);
	return ir;
}

Ir *ir_initAdd (size_t size)
{
	Ir *ir = ir_init(Ir_Add);
	ir->as.add = irAdd_init(size);
	return ir;
}

Ir *ir_initAnd (size_t size)
{
	Ir *ir = ir_init(Ir_And);
	ir->as.and = irAnd_init(size);
	return ir;
}

Ir *ir_initAssign (size_t size)
{
	Ir *ir = ir_init(Ir_Assign);
	ir->as.assign = irAssign_init(size);
	return ir;
}

Ir *ir_initCall (void)
{
	Ir *ir = ir_init(Ir_Call);
	ir->as.call = irCall_init();
	return ir;
}

Ir *ir_initCast (DataType *from, DataType *to)
{
	Ir *ir = ir_init(Ir_Cast);
	ir->as.cast = irCast_init(from, to);
	return ir;
}

Ir *ir_initCopy (void)
{
	Ir *ir = ir_init(Ir_Copy);
	ir->as.copy = irCopy_init();
	return ir;
}

Ir *ir_initDec (size_t size)
{
	Ir *ir = ir_init(Ir_Dec);
	ir->as.dec = irDec_init(size);
	return ir;
}

Ir *ir_initDeref (size_t size)
{
	Ir *ir = ir_init(Ir_Deref);
	ir->as.deref = irDeref_init(size);
	return ir;
}

Ir *ir_initEqu (size_t size)
{
	Ir *ir = ir_init(Ir_Equ);
	ir->as.equ = irEqu_init(size);
	return ir;
}

Ir *ir_initFnRef (Token identifier)
{
	Ir *ir = ir_init(Ir_FnRef);
	ir->as.fnRef = irFnRef_init(identifier);
	return ir;
}

Ir *ir_initFunctionEnd (void)
{
	Ir *ir = ir_init(Ir_FunctionEnd);
	ir->as.functionEnd = irFunctionEnd_init();
	return ir;
}

Ir *ir_initFunctionStart (Token identifier)
{
	Ir *ir = ir_init(Ir_FunctionStart);
	ir->as.functionStart = irFunctionStart_init(identifier);
	return ir;
}

Ir *ir_initInc (size_t size)
{
	Ir *ir = ir_init(Ir_Inc);
	ir->as.inc = irInc_init(size);
	return ir;
}

Ir *ir_initJmp (int n)
{
	Ir *ir = ir_init(Ir_Jmp);
	ir->as.jmp = irJmp_init(n);
	return ir;
}

Ir *ir_initJmpFalse (int n)
{
	Ir *ir = ir_init(Ir_JmpFalse);
	ir->as.jmpFalse = irJmpFalse_init(n);
	return ir;
}

Ir *ir_initJmpTrue (int n)
{
	Ir *ir = ir_init(Ir_JmpTrue);
	ir->as.jmpTrue = irJmpTrue_init(n);
	return ir;
}

Ir *ir_initLabel (int n)
{
	Ir *ir = ir_init(Ir_Label);
	ir->as.label = irLabel_init(n);
	return ir;
}

Ir *ir_initLess (size_t size)
{
	Ir *ir = ir_init(Ir_Less);
	ir->as.less = irLess_init(size);
	return ir;
}

Ir *ir_initLessEqu (size_t size)
{
	Ir *ir = ir_init(Ir_LessEqu);
	ir->as.lessEqu = irLessEqu_init(size);
	return ir;
}

Ir *ir_initMul (size_t size)
{
	Ir *ir = ir_init(Ir_Mul);
	ir->as.mul = irMul_init(size);
	return ir;
}

Ir *ir_initNeg (size_t size)
{
	Ir *ir = ir_init(Ir_Neg);
	ir->as.neg = irNeg_init(size);
	return ir;
}

Ir *ir_initNot (size_t size)
{
	Ir *ir = ir_init(Ir_Not);
	ir->as.not = irNot_init(size);
	return ir;
}

Ir *ir_initNotEqu (size_t size)
{
	Ir *ir = ir_init(Ir_NotEqu);
	ir->as.notEqu = irNotEqu_init(size);
	return ir;
}

Ir *ir_initOr (size_t size)
{
	Ir *ir = ir_init(Ir_Or);
	ir->as.or = irOr_init(size);
	return ir;
}

Ir *ir_initParameter (size_t pIdx, size_t vIdx, size_t size)
{
	Ir *ir = ir_init(Ir_Parameter);
	ir->as.parameter = irParameter_init(pIdx, vIdx, size);
	return ir;
}

Ir *ir_initPop (void)
{
	Ir *ir = ir_init(Ir_Pop);
	ir->as.pop = irPop_init();
	return ir;
}

Ir *ir_initPush (int32_t value)
{
	Ir *ir = ir_init(Ir_Push);
	ir->as.push = irPush_init(value);
	return ir;
}

Ir *ir_initRef (size_t idx)
{
	Ir *ir = ir_init(Ir_Ref);
	ir->as.ref = irRef_init(idx);
	return ir;
}

Ir *ir_initReserve (size_t bytes)
{
	Ir *ir = ir_init(Ir_Reserve);
	ir->as.reserve = irReserve_init(bytes);
	return ir;
}

Ir *ir_initRestore (void)
{
	Ir *ir = ir_init(Ir_Restore);
	ir->as.restore = irRestore_init();
	return ir;
}

Ir *ir_initRet (void)
{
	Ir *ir = ir_init(Ir_Ret);
	ir->as.ret = irRet_init();
	return ir;
}

Ir *ir_initSar (size_t size)
{
	Ir *ir = ir_init(Ir_Sar);
	ir->as.sar = irSar_init(size);
	return ir;
}

Ir *ir_initShl (size_t size)
{
	Ir *ir = ir_init(Ir_Shl);
	ir->as.shl = irShl_init(size);
	return ir;
}

Ir *ir_initStore (void)
{
	Ir *ir = ir_init(Ir_Store);
	ir->as.store = irStore_init();
	return ir;
}

Ir *ir_initSub (size_t size)
{
	Ir *ir = ir_init(Ir_Sub);
	ir->as.sub = irSub_init(size);
	return ir;
}

Ir *ir_initVal (size_t idx, size_t size)
{
	Ir *ir = ir_init(Ir_Val);
	ir->as.val = irVal_init(idx, size);
	return ir;
}

Ir *ir_initXor (size_t size)
{
	Ir *ir = ir_init(Ir_Xor);
	ir->as.xor = irXor_init(size);
	return ir;
}

static IrAccess *irAccess_init (size_t index)
{
	IrAccess *access = mem_alloc(sizeof(*access));
	access->index = index;
	return access;
}

static IrAdd *irAdd_init (size_t size)
{
	IrAdd *add = mem_alloc(sizeof(*add));
	add->size = size;
	return add;
}

static IrAnd *irAnd_init (size_t size)
{
	IrAnd *and = mem_alloc(sizeof(*and));
	and->size = size;
	return and;
}

static IrAssign *irAssign_init (size_t size)
{
	IrAssign *assign = mem_alloc(sizeof(*assign));
	assign->size = size;
	return assign;
}

static IrCall *irCall_init (void)
{
	IrCall *call = mem_alloc(sizeof(*call));
	return call;
}

static IrCast *irCast_init (DataType *from, DataType *to)
{
	IrCast *cast = mem_alloc(sizeof(*cast));
	cast->from = from;
	cast->to = to;
	return cast;
}

static IrCopy *irCopy_init (void)
{
	IrCopy *copy = mem_alloc(sizeof(*copy));
	return copy;
}

static IrDec *irDec_init (size_t size)
{
	IrDec *dec = mem_alloc(sizeof(*dec));
	dec->size = size;
	return dec;
}

static IrDeref *irDeref_init (size_t size)
{
	IrDeref *deref = mem_alloc(sizeof(*deref));
	deref->size = size;
	return deref;
}

static IrEqu *irEqu_init (size_t size)
{
	IrEqu *equ = mem_alloc(sizeof(*equ));
	equ->size = size;
	return equ;
}

static IrFnRef *irFnRef_init (Token identifier)
{
	IrFnRef *fnRef = mem_alloc(sizeof(*fnRef));
	fnRef->identifier = identifier;
	return fnRef;
}

static IrFunctionEnd *irFunctionEnd_init (void)
{
	IrFunctionEnd *functionEnd = mem_alloc(sizeof(*functionEnd));
	return functionEnd;
}

static IrFunctionStart *irFunctionStart_init (Token identifier)
{
	IrFunctionStart *functionStart = mem_alloc(sizeof(*functionStart));
	functionStart->identifier = identifier;
	return functionStart;
}

static IrInc *irInc_init (size_t size)
{
	IrInc *inc = mem_alloc(sizeof(*inc));
	inc->size = size;
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

static IrLess *irLess_init (size_t size)
{
	IrLess *less = mem_alloc(sizeof(*less));
	less->size = size;
	return less;
}

static IrLessEqu *irLessEqu_init (size_t size)
{
	IrLessEqu *lessEqu = mem_alloc(sizeof(*lessEqu));
	lessEqu->size = size;
	return lessEqu;
}

static IrMul *irMul_init (size_t size)
{
	IrMul *mul = mem_alloc(sizeof(*mul));
	mul->size = size;
	return mul;
}

static IrNeg *irNeg_init (size_t size)
{
	IrNeg *neg = mem_alloc(sizeof(*neg));
	neg->size = size;
	return neg;
}

static IrNot *irNot_init (size_t size)
{
	IrNot *not = mem_alloc(sizeof(*not));
	not->size = size;
	return not;
}

static IrNotEqu *irNotEqu_init (size_t size)
{
	IrNotEqu *notEqu = mem_alloc(sizeof(*notEqu));
	notEqu->size = size;
	return notEqu;
}

static IrOr *irOr_init (size_t size)
{
	IrOr *or = mem_alloc(sizeof(*or));
	or->size = size;
	return or;
}

static IrParameter *irParameter_init (size_t pIdx, size_t vIdx, size_t size)
{
	IrParameter *parameter = mem_alloc(sizeof(*parameter));
	parameter->pIdx = pIdx;
	parameter->vIdx = vIdx;
	parameter->size = size;
	return parameter;
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

static IrRestore *irRestore_init (void)
{
	IrRestore *restore = mem_alloc(sizeof(*restore));
	return restore;
}

static IrRet *irRet_init (void)
{
	IrRet *ret = mem_alloc(sizeof(*ret));
	return ret;
}

static IrSar *irSar_init (size_t size)
{
	IrSar *sar = mem_alloc(sizeof(*sar));
	sar->size = size;
	return sar;
}

static IrShl *irShl_init (size_t size)
{
	IrShl *shl = mem_alloc(sizeof(*shl));
	shl->size = size;
	return shl;
}

static IrStore *irStore_init (void)
{
	IrStore *store = mem_alloc(sizeof(*store));
	return store;
}

static IrSub *irSub_init (size_t size)
{
	IrSub *sub = mem_alloc(sizeof(*sub));
	sub->size = size;
	return sub;
}

static IrVal *irVal_init (size_t idx, size_t size)
{
	IrVal *val = mem_alloc(sizeof(*val));
	val->idx = idx;
	val->size = size;
	return val;
}

static IrXor *irXor_init (size_t size)
{
	IrXor *xor = mem_alloc(sizeof(*xor));
	xor->size = size;
	return xor;
}
