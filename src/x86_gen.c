#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "x86_gen.h"

static void emit (char *format, ...);
static void transform (Ir *r);
static void transformAdd (IrAdd *instruction);
static void transformAnd (IrAnd *instruction);
static void transformAssign (IrAssign *instruction);
static void transformEqu (IrEqu *instruction);
static void transformJmp (IrJmp *instruction);
static void transformJmpFalse (IrJmpFalse *instruction);
static void transformLabel (IrLabel *instruction);
static void transformLess (IrLess *instruction);
static void transformLessEqu (IrLessEqu *instruction);
static void transformMul (IrMul *instruction);
static void transformNeg (IrNeg *instruction);
static void transformNot (IrNot *instruction);
static void transformNotEqu (IrNotEqu *instruction);
static void transformOr (IrOr *instruction);
static void transformPush (IrPush *instruction);
static void transformRef (IrRef *instruction);
static void transformReserve (IrReserve *instruction);
static void transformSar (IrSar *instruction);
static void transformShl (IrShl *instruction);
static void transformSub (IrSub *instruction);
static void transformVal (IrVal *instruction);
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
	emit("\tcall    main");
	emit("\tmov     rdi, rax");
	emit("\tmov     rax, 60");
	emit("\tsyscall");

	emit("");

	emit("main:");
	emit("\tpush    rbp");
	emit("\tmov     rbp, rsp");

	for (Ir *r = ir; r != NULL; r = r->next) {
		transform(r);
	}

	emit("\tpop     rax");
	emit("\tmov     rsp, rbp");
	emit("\tpop     rbp");
	emit("\tret");

	fclose(gen.fp);
}

static void transform (Ir *r)
{
	switch (r->type) {
		case Ir_Add: transformAdd(r->as.add); break;
		case Ir_And: transformAnd(r->as.and); break;
		case Ir_Assign: transformAssign(r->as.assign); break;
		case Ir_Equ: transformEqu(r->as.equ); break;
		case Ir_Jmp: transformJmp(r->as.jmp); break;
		case Ir_JmpFalse: transformJmpFalse(r->as.jmpFalse); break;
		case Ir_Label: transformLabel(r->as.label); break;
		case Ir_Less: transformLess(r->as.less); break;
		case Ir_LessEqu: transformLessEqu(r->as.lessEqu); break;
		case Ir_Mul: transformMul(r->as.mul); break;
		case Ir_Neg: transformNeg(r->as.neg); break;
		case Ir_Not: transformNot(r->as.not); break;
		case Ir_NotEqu: transformNotEqu(r->as.notEqu); break;
		case Ir_Or: transformOr(r->as.or); break;
		case Ir_Push: transformPush(r->as.push); break;
		case Ir_Ref: transformRef(r->as.ref); break;
		case Ir_Reserve: transformReserve(r->as.reserve); break;
		case Ir_Sar: transformSar(r->as.sar); break;
		case Ir_Shl: transformShl(r->as.shl); break;
		case Ir_Sub: transformSub(r->as.sub); break;
		case Ir_Val: transformVal(r->as.val); break;
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

static void transformAssign (IrAssign *instruction)
{
	emit("\tpop     r9");
	emit("\tpop     r8");
	emit("\tmov     dword [r8], r9d");
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

static void transformJmp (IrJmp *instruction)
{
	emit("\tjmp     .LB%d", instruction->n);
}

static void transformJmpFalse (IrJmpFalse *instruction)
{
	emit("\tpop     r8");
	emit("\ttest    r8d, r8d");
	emit("\tjz      .LB%d", instruction->n);
}

static void transformLabel (IrLabel *instruction)
{
	emit(".LB%d:", instruction->n);
}

static void transformLess (IrLess *instruction)
{
	emit("\tpop     r9");
	emit("\tpop     r8");
	emit("\tcmp     r8, r9");
	emit("\tsetl    r8b");
	emit("\tpush    r8");
}

static void transformLessEqu (IrLessEqu *instruction)
{
	emit("\tpop     r9");
	emit("\tpop     r8");
	emit("\tcmp     r8, r9");
	emit("\tsetle   r8b");
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

static void transformRef (IrRef *instruction)
{
	emit("\tlea     r9, [rbp - %d]", instruction->idx * 4 + 4);
	emit("\tpush    r9");
}

static void transformReserve (IrReserve *instruction)
{
	emit("\tsub     rsp, %d", instruction->bytes);
}

static void transformSar (IrSar *instruction)
{
	emit("\tpop     rcx");
	emit("\tpop     r8");
	emit("\tsar     r8d, cl");
	emit("\tpush    r8");
}

static void transformShl (IrShl *instruction)
{
	emit("\tpop     rcx");
	emit("\tpop     r8");
	emit("\tshl     r8d, cl");
	emit("\tpush    r8");
}

static void transformSub (IrSub *instruction)
{
	emit("\tpop     r9");
	emit("\tpop     r8");
	emit("\tsub     r8d, r9d");
	emit("\tpush    r8");
}

static void transformVal (IrVal *instruction)
{
	emit("\tmov     r9d, dword [rbp - %d]", instruction->idx * 4 + 4);
	emit("\tpush    r9");
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
