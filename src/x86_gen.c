#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "memory.h"
#include "x86_gen.h"

#define TRANSFORMERS \
	X(Access) \
	X(Add) \
	X(And) \
	X(Assign) \
	X(Call) \
	X(Cast) \
	X(Copy) \
	X(Dec) \
	X(Deref) \
	X(Equ) \
	X(FnRef) \
	X(FunctionEnd) \
	X(FunctionStart) \
	X(Inc) \
	X(Jmp) \
	X(JmpFalse) \
	X(JmpTrue) \
	X(Label) \
	X(Less) \
	X(LessEqu) \
	X(Mul) \
	X(Neg) \
	X(Not) \
	X(NotEqu) \
	X(Or) \
	X(Parameter) \
	X(Pop) \
	X(Push) \
	X(Ref) \
	X(Reserve) \
	X(Restore) \
	X(Ret) \
	X(Sar) \
	X(Shl) \
	X(Store) \
	X(Sub) \
	X(Val) \
	X(Xor)

#define push(register) emit("\tpush    " #register)
#define pop(register) emit("\tpop     " #register)

static void emit (char *format, ...);

static void transform (Ir *r);

#define X(name) static void transform##name (Ir *ir);
TRANSFORMERS
#undef X

typedef enum {
	r8,
	r9,
} Register;

static char *reg[][QWORD + 1] = {
	[r8] = { [QWORD] = "r8", [DWORD] = "r8d", [WORD] = "r8w", [BYTE] = "r8b" },
	[r9] = { [QWORD] = "r9", [DWORD] = "r9d", [WORD] = "r9w", [BYTE] = "r9b" },
};

static char *quantity[] = {
	[BYTE] = "byte",
	[WORD] = "word",
	[DWORD] = "dword",
	[QWORD] = "qword",
};

void (*CastTable[DataTypeType_Count][DataTypeType_Count])(DataType *from, DataType *to) = { };

typedef struct {
	FILE *fp;
} AsmGenerator;

static AsmGenerator gen;

void gen_x86 (Ir *ir)
{
	const char *path = "out.s";
	remove(path);
	gen.fp = fopen(path, "ab");
	if (gen.fp == NULL) {
		fprintf(stderr, "could not open file\n");
		exit(1);
	}

	emit("section .text");
	emit("\tglobal _start");
	emit("");
	emit("_start:");
	emit("\tcall    main");
	emit("\tmov     rdi, rax");
	emit("\tmov     rax, 60");
	emit("\tsyscall");

	for (Ir *r = ir; r != NULL; r = r->next) {
		transform(r);
	}

	fclose(gen.fp);
}

static void transform (Ir *r)
{

	#define X(type) [Ir_##type] = transform##type,
	static void (*transformers[])(Ir *ir) = {
		TRANSFORMERS
	};
	#undef X
	if (transformers[r->type] == NULL) {
		fprintf(stderr, "deverr: x86gen: undefined transformer. ir type: %d\n", r->type);
		exit(1);
	}
	transformers[r->type](r);
}

static void transformAccess (Ir *ir)
{
	IrAccess *instruction = ir->as.access;
	(void)instruction;
	pop(r8);
	emit("\tadd     %s, %d", reg[r8][QWORD], instruction->index);
	push(r8);
}

static void transformAdd (Ir *ir)
{
	IrAdd *instruction = ir->as.add;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tadd     %s, %s", reg[r8][instruction->size], reg[r9][instruction->size]);
	push(r8);
}

static void transformAnd (Ir *ir)
{
	IrAnd *instruction = ir->as.and;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tand     %s, %s", reg[r8][instruction->size], reg[r9][instruction->size]);
	push(r8);
}

static void transformAssign (Ir *ir)
{
	IrAssign *instruction = ir->as.assign;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tmov     %s [r8], %s", quantity[instruction->size], reg[r9][instruction->size]);
	push(r8);
}

static void transformCall (Ir *ir)
{
	IrCall *instruction = ir->as.call;
	(void)instruction;
	pop(r8);
	emit("\tcall    %s", reg[r8][QWORD]);
	push(rax);
}

static void transformCast (Ir *ir)
{
	IrCast *instruction = ir->as.cast;
	(void)instruction;
	DataType *from = instruction->from;
	DataType *to = instruction->to;
	CastTable[from->type][to->type](from, to);
}

static void transformCopy (Ir *ir)
{
	IrCopy *instruction = ir->as.copy;
	(void)instruction;
	pop(r8);
	push(r8);
	push(r8);
}

static void transformDec (Ir *ir)
{
	IrDec *instruction = ir->as.dec;
	(void)instruction;
	pop(r8);
	emit("\tdec     %s [r8]", quantity[instruction->size]);
	push(r8);
}

static void transformDeref (Ir *ir)
{
	IrDeref *instruction = ir->as.deref;
	(void)instruction;
	pop(r8);
	emit("\tmov     %s, %s [r8]", reg[r8][instruction->size], quantity[instruction->size]);
	push(r8);
}

static void transformEqu (Ir *ir)
{
	IrEqu *instruction = ir->as.equ;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tcmp     %s, %s", reg[r8][instruction->size], reg[r9][instruction->size]);
	emit("\tsete    r8b");
	push(r8);
}

static void transformFnRef (Ir *ir)
{
	IrFnRef *instruction = ir->as.fnRef;
	(void)instruction;
	emit("\tpush    %.*s", instruction->identifier.length, instruction->identifier.lexeme);
}

static void transformFunctionEnd (Ir *ir)
{
	IrFunctionEnd *instruction = ir->as.functionEnd;
	(void)instruction;
	emit("\tmov     rsp, rbp");
	pop(rbp);
	emit("\tret");
}

static void transformFunctionStart (Ir *ir)
{
	IrFunctionStart *instruction = ir->as.functionStart;
	(void)instruction;
	emit("");
	emit("%.*s:", instruction->identifier.length, instruction->identifier.lexeme);
	push(rbp);
	emit("\tmov     rbp, rsp");
}

static void transformInc (Ir *ir)
{
	IrInc *instruction = ir->as.inc;
	(void)instruction;
	pop(r8);
	emit("\tinc     %s [r8]", quantity[instruction->size]);
	push(r8);
}

static void transformJmp (Ir *ir)
{
	IrJmp *instruction = ir->as.jmp;
	(void)instruction;
	emit("\tjmp     .LB%d", instruction->n);
}

static void transformJmpFalse (Ir *ir)
{
	IrJmpFalse *instruction = ir->as.jmpFalse;
	(void)instruction;
	pop(r8);
	emit("\ttest    %s, %s", reg[r8][BYTE], reg[r8][BYTE]);
	emit("\tjz      .LB%d", instruction->n);
}

static void transformJmpTrue (Ir *ir)
{
	IrJmpTrue *instruction = ir->as.jmpTrue;
	(void)instruction;
	pop(r8);
	emit("\ttest    %s, %s", reg[r8][BYTE], reg[r8][BYTE]);
	emit("\tjnz     .LB%d", instruction->n);
}

static void transformLabel (Ir *ir)
{
	IrLabel *instruction = ir->as.label;
	(void)instruction;
	emit(".LB%d:", instruction->n);
}

static void transformLess (Ir *ir)
{
	IrLess *instruction = ir->as.less;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tcmp     %s, %s", reg[r8][instruction->size], reg[r9][instruction->size]);
	emit("\tsetl    %s", reg[r8][BYTE]);
	push(r8);
}

static void transformLessEqu (Ir *ir)
{
	IrLessEqu *instruction = ir->as.lessEqu;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tcmp     %s, %s", reg[r8][instruction->size], reg[r9][instruction->size]);
	emit("\tsetle   %s", reg[r8][BYTE]);
	push(r8);
}

static void transformMul (Ir *ir)
{
	IrMul *instruction = ir->as.mul;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\timul    %s, %s", reg[r8][instruction->size], reg[r9][instruction->size]);
	push(r8);
}

static void transformNeg (Ir *ir)
{
	IrNeg *instruction = ir->as.neg;
	(void)instruction;
	pop(r9);
	emit("\tneg     %s", reg[r9][instruction->size]);
	push(r9);
}

static void transformNot (Ir *ir)
{
	IrNot *instruction = ir->as.not;
	(void)instruction;
	pop(r8);
	emit("\tnot     %s", reg[r8][instruction->size]);
	push(r8);
}

static void transformNotEqu (Ir *ir)
{
	IrNotEqu *instruction = ir->as.notEqu;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tcmp     %s, %s", reg[r8][instruction->size], reg[r9][instruction->size]);
	emit("\tsetne   %s", reg[r8][BYTE]);
	push(r8);
}

static void transformOr (Ir *ir)
{
	IrOr *instruction = ir->as.or;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tor      %s, %s", reg[r8][instruction->size], reg[r9][instruction->size]);
	push(r8);
}

static void transformParameter (Ir *ir)
{
	IrParameter *instruction = ir->as.parameter;
	(void)instruction;
	emit("\tmov     %s, %s [rbp + %d]", reg[r8][instruction->size], quantity[instruction->size], instruction->pIdx * 8 + 16);
	emit("\tmov     %s [rbp - %d], %s", quantity[instruction->size], instruction->vIdx, reg[r8][instruction->size]);
}

static void transformPop (Ir *ir)
{
	IrPop *instruction = ir->as.pop;
	(void)instruction;
	emit("\tadd     rsp, 8");
}

static void transformPush (Ir *ir)
{
	IrPush *instruction = ir->as.push;
	(void)instruction;
	emit("\tpush    %d", instruction->value);
}

static void transformRef (Ir *ir)
{
	IrRef *instruction = ir->as.ref;
	(void)instruction;
	emit("\tlea     %s, [rbp - %d]", reg[r9][QWORD], instruction->idx);
	push(r9);
}

static void transformReserve (Ir *ir)
{
	IrReserve *instruction = ir->as.reserve;
	(void)instruction;
	emit("\tsub     rsp, %d", instruction->bytes);
}

static void transformRestore (Ir *ir)
{
	IrRestore *instruction = ir->as.restore;
	(void)instruction;
	push(rax);
}

static void transformRet (Ir *ir)
{
	IrRet *instruction = ir->as.ret;
	(void)instruction;
	emit("\tmov     rax, [rsp]");
	emit("\tmov     rsp, rbp");
	pop(rbp);
	emit("\tret");
}

static void transformSar (Ir *ir)
{
	IrSar *instruction = ir->as.sar;
	(void)instruction;
	pop(rcx);
	pop(r8);
	emit("\tsar     %s, cl", reg[r8][instruction->size]);
	push(r8);
}

static void transformShl (Ir *ir)
{
	IrShl *instruction = ir->as.shl;
	(void)instruction;
	pop(rcx);
	pop(r8);
	emit("\tshl     %s, cl", reg[r8][instruction->size]);
	push(r8);
}

static void transformStore (Ir *ir)
{
	IrStore *instruction = ir->as.store;
	(void)instruction;
	pop(rax);
}

static void transformSub (Ir *ir)
{
	IrSub *instruction = ir->as.sub;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tsub     %s, %s", reg[r8][instruction->size], reg[r9][instruction->size]);
	push(r8);
}

static void transformVal (Ir *ir)
{
	IrVal *instruction = ir->as.val;
	(void)instruction;
	emit("\tmov     %s, %s [rbp - %d]", reg[r9][instruction->size], quantity[instruction->size], instruction->idx);
	push(r9);
}

static void transformXor (Ir *ir)
{
	IrXor *instruction = ir->as.xor;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\txor     %s, %s", reg[r8][instruction->size], reg[r9][instruction->size]);
	push(r8);
}

static void emit (char *format, ...)
{
	va_list args;
	va_start(args, format);

	vfprintf(gen.fp, format, args);
	fprintf(gen.fp, "\n");

	va_end(args);
}
