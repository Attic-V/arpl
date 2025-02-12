#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "x86_gen.h"

#define push(register) emit("\tpush    " #register)
#define pop(register) emit("\tpop     " #register)

static void emit (char *format, ...);

static void transform (Ir *r);
static void transformAdd (Ir *ir);
static void transformAnd (Ir *ir);
static void transformAssign (Ir *ir);
static void transformEqu (Ir *ir);
static void transformJmp (Ir *ir);
static void transformJmpFalse (Ir *ir);
static void transformLabel (Ir *ir);
static void transformLess (Ir *ir);
static void transformLessEqu (Ir *ir);
static void transformMul (Ir *ir);
static void transformNeg (Ir *ir);
static void transformNot (Ir *ir);
static void transformNotEqu (Ir *ir);
static void transformOr (Ir *ir);
static void transformPush (Ir *ir);
static void transformRef (Ir *ir);
static void transformReserve (Ir *ir);
static void transformSar (Ir *ir);
static void transformShl (Ir *ir);
static void transformSub (Ir *ir);
static void transformVal (Ir *ir);
static void transformXor (Ir *ir);

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

	pop(rax);
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
		transformer(Equ),
		transformer(Jmp),
		transformer(JmpFalse),
		transformer(Label),
		transformer(Less),
		transformer(LessEqu),
		transformer(Mul),
		transformer(Neg),
		transformer(Not),
		transformer(NotEqu),
		transformer(Or),
		transformer(Push),
		transformer(Ref),
		transformer(Reserve),
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
	emit("\tadd     r8d, r9d");
	push(r8);
}

static void transformAnd (Ir *ir)
{
	IrAnd *instruction = ir->as.and;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tand     r8d, r9d");
	push(r8);
}

static void transformAssign (Ir *ir)
{
	IrAssign *instruction = ir->as.assign;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tmov     dword [r8], r9d");
	push(r8);
}

static void transformEqu (Ir *ir)
{
	IrEqu *instruction = ir->as.equ;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tcmp     r8, r9");
	emit("\tsete    r8b");
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
	emit("\ttest    r8d, r8d");
	emit("\tjz      .LB%d", instruction->n);
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
	emit("\tcmp     r8, r9");
	emit("\tsetl    r8b");
	push(r8);
}

static void transformLessEqu (Ir *ir)
{
	IrLessEqu *instruction = ir->as.lessEqu;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tcmp     r8, r9");
	emit("\tsetle   r8b");
	push(r8);
}

static void transformMul (Ir *ir)
{
	IrMul *instruction = ir->as.mul;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\timul    r8d, r9d");
	push(r8);
}

static void transformNeg (Ir *ir)
{
	IrNeg *instruction = ir->as.neg;
	(void)instruction;
	pop(r9);
	emit("\tneg     r9d");
	push(r9);
}

static void transformNot (Ir *ir)
{
	IrNot *instruction = ir->as.not;
	(void)instruction;
	pop(r8);
	emit("\tnot     r8d");
	push(r8);
}

static void transformNotEqu (Ir *ir)
{
	IrNotEqu *instruction = ir->as.notEqu;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tcmp     r8, r9");
	emit("\tsetne   r8b");
	push(r8);
}

static void transformOr (Ir *ir)
{
	IrOr *instruction = ir->as.or;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tor      r8d, r9d");
	push(r8);
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
	emit("\tlea     r9, [rbp - %d]", instruction->idx + 4);
	push(r9);
}

static void transformReserve (Ir *ir)
{
	IrReserve *instruction = ir->as.reserve;
	(void)instruction;
	emit("\tsub     rsp, %d", instruction->bytes);
}

static void transformSar (Ir *ir)
{
	IrSar *instruction = ir->as.sar;
	(void)instruction;
	pop(rcx);
	pop(r8);
	emit("\tsar     r8d, cl");
	push(r8);
}

static void transformShl (Ir *ir)
{
	IrShl *instruction = ir->as.shl;
	(void)instruction;
	pop(rcx);
	pop(r8);
	emit("\tshl     r8d, cl");
	push(r8);
}

static void transformSub (Ir *ir)
{
	IrSub *instruction = ir->as.sub;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\tsub     r8d, r9d");
	push(r8);
}

static void transformVal (Ir *ir)
{
	IrVal *instruction = ir->as.val;
	(void)instruction;
	emit("\tmov     r9d, dword [rbp - %d]", instruction->idx + 4);
	push(r9);
}

static void transformXor (Ir *ir)
{
	IrXor *instruction = ir->as.xor;
	(void)instruction;
	pop(r9);
	pop(r8);
	emit("\txor     r8d, r9d");
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
