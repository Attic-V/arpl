#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "memory.h"
#include "x86_gen.h"

#define push(register) emit("\tpush    " #register)
#define pop(register) emit("\tpop     " #register)

static void emit (char *format, ...);

static void transform (Ir *r);
static void transformAdd (Ir *ir);
static void transformAnd (Ir *ir);
static void transformAssign (Ir *ir);
static void transformCastConvert (Ir *ir);
static void transformCastReinterpret (Ir *ir);
static void transformCopy (Ir *ir);
static void transformDec (Ir *ir);
static void transformDeref (Ir *ir);
static void transformEqu (Ir *ir);
static void transformInc (Ir *ir);
static void transformJmp (Ir *ir);
static void transformJmpFalse (Ir *ir);
static void transformJmpTrue (Ir *ir);
static void transformLabel (Ir *ir);
static void transformLess (Ir *ir);
static void transformLessEqu (Ir *ir);
static void transformMul (Ir *ir);
static void transformNeg (Ir *ir);
static void transformNot (Ir *ir);
static void transformNotEqu (Ir *ir);
static void transformOr (Ir *ir);
static void transformPop (Ir *ir);
static void transformPush (Ir *ir);
static void transformRef (Ir *ir);
static void transformReserve (Ir *ir);
static void transformRet (Ir *ir);
static void transformSar (Ir *ir);
static void transformShl (Ir *ir);
static void transformSub (Ir *ir);
static void transformVal (Ir *ir);
static void transformXor (Ir *ir);

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

	emit("");

	emit("main:");
	push(rbp);
	emit("\tmov     rbp, rsp");

	for (Ir *r = ir; r != NULL; r = r->next) {
		transform(r);
	}

	emit("\tmov     rsp, rbp");
	pop(rbp);
	emit("\tret");

	fclose(gen.fp);
}

static void transform (Ir *r)
{
	#define transformer(type) [Ir_##type] = transform##type
	static void (*transformers[])(Ir *ir) = {
		transformer(Add),
		transformer(And),
		transformer(Assign),
		transformer(CastConvert),
		transformer(CastReinterpret),
		transformer(Copy),
		transformer(Dec),
		transformer(Deref),
		transformer(Equ),
		transformer(Inc),
		transformer(Jmp),
		transformer(JmpFalse),
		transformer(JmpTrue),
		transformer(Label),
		transformer(Less),
		transformer(LessEqu),
		transformer(Mul),
		transformer(Neg),
		transformer(Not),
		transformer(NotEqu),
		transformer(Or),
		transformer(Pop),
		transformer(Push),
		transformer(Ref),
		transformer(Reserve),
		transformer(Ret),
		transformer(Sar),
		transformer(Shl),
		transformer(Sub),
		transformer(Val),
		transformer(Xor),
	};
	transformers[r->type](r);
	#undef transformer
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

static void transformCastConvert (Ir *ir)
{
	IrCastConvert *instruction = ir->as.castConvert;
	(void)instruction;
	size_t sizeFrom = dataType_getSize(instruction->from);
	size_t sizeTo = dataType_getSize(instruction->to);
	pop(r8);
	if (dataType_isI8(instruction->from) && dataType_isI32(instruction->to)) {
		emit("\tmovsx   %s, %s", reg[r8][sizeTo], reg[r8][sizeFrom]);
	}
	if (dataType_isI32(instruction->from) && dataType_isI8(instruction->to)) {
		emit("\tmov     %s, %s", reg[r8][sizeTo], reg[r8][sizeTo]);
	}
	push(r8);
}

static void transformCastReinterpret (Ir *ir)
{
	IrCastReinterpret *instruction = ir->as.castReinterpret;
	(void)instruction;
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
