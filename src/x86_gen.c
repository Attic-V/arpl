#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "x86_gen.h"

static void emit (char *format, ...);
static void transform (Ir *r);
static void transformAdd (IrAdd *instruction);
static void transformAnd (IrAnd *instruction);
static void transformEqu (IrEqu *instruction);
static void transformMul (IrMul *instruction);
static void transformNeg (IrNeg *instruction);
static void transformNot (IrNot *instruction);
static void transformNotEqu (IrNotEqu *instruction);
static void transformOr (IrOr *instruction);
static void transformPush (IrPush *instruction);
static void transformSub (IrSub *instruction);
static void transformXor (IrXor *instruction);

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

	for (Ir *r = ir; r != NULL; r = r->next) {
		transform(r);
	}

	emit("\tmov     rax, 60");
	emit("\tpop     rdi");
	emit("\tsyscall");

	fclose(gen.fp);
}

static void transform (Ir *r)
{
	switch (r->type) {
		case Ir_Add: transformAdd(r->as.add); break;
		case Ir_And: transformAnd(r->as.and); break;
		case Ir_Equ: transformEqu(r->as.equ); break;
		case Ir_Mul: transformMul(r->as.mul); break;
		case Ir_Neg: transformNeg(r->as.neg); break;
		case Ir_Not: transformNot(r->as.not); break;
		case Ir_NotEqu: transformNotEqu(r->as.notEqu); break;
		case Ir_Or: transformOr(r->as.or); break;
		case Ir_Push: transformPush(r->as.push); break;
		case Ir_Sub: transformSub(r->as.sub); break;
		case Ir_Xor: transformXor(r->as.xor); break;
	}
}

static void transformAdd (IrAdd *instruction)
{
	emit("\tpop     r9");
	emit("\tpop     r8");
	emit("\tadd     r8d, r9d");
	emit("\tpush    r8");
}

static void transformAnd (IrAnd *instruction)
{
	emit("\tpop     r9");
	emit("\tpop     r8");
	emit("\tand     r8d, r9d");
	emit("\tpush    r8");
}

static void transformEqu (IrEqu *instruction)
{
	emit("\tpop     r9");
	emit("\tpop     r8");
	emit("\tcmp     r8, r9");
	emit("\tsete    r8b");
	emit("\tpush    r8");
}

static void transformMul (IrMul *instruction)
{
	emit("\tpop     r9");
	emit("\tpop     r8");
	emit("\timul    r8d, r9d");
	emit("\tpush    r8");
}

static void transformNeg (IrNeg *instruction)
{
	emit("\tpop     r9");
	emit("\tneg     r9d");
	emit("\tpush    r9");
}

static void transformNot (IrNot *instruction)
{
	emit("\tpop     r8");
	emit("\tnot     r8d");
	emit("\tpush    r8");
}

static void transformNotEqu (IrNotEqu *instruction)
{
	emit("\tpop     r9");
	emit("\tpop     r8");
	emit("\tcmp     r8, r9");
	emit("\tsetne   r8b");
	emit("\tpush    r8");
}

static void transformOr (IrOr *instruction)
{
	emit("\tpop     r9");
	emit("\tpop     r8");
	emit("\tor      r8d, r9d");
	emit("\tpush    r8");
}

static void transformPush (IrPush *instruction)
{
	emit("\tpush    %d", instruction->value);
}

static void transformSub (IrSub *instruction)
{
	emit("\tpop     r9");
	emit("\tpop     r8");
	emit("\tsub     r8d, r9d");
	emit("\tpush    r8");
}

static void transformXor (IrXor *instruction)
{
	emit("\tpop     r9");
	emit("\tpop     r8");
	emit("\txor     r8d, r9d");
	emit("\tpush    r8");
}

static void emit (char *format, ...)
{
	va_list args;
	va_start(args, format);

	vfprintf(gen.fp, format, args);
	fprintf(gen.fp, "\n");

	va_end(args);
}
