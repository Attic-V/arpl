#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "x86_gen.h"

void emit (char *format, ...);
void transform (Ir *r);
void transformPush (IrPush *r);

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

void transform (Ir *r)
{
	switch (r->type) {
		case Ir_Push: transformPush(r->as.push); break;
	}
}

void transformPush (IrPush *r)
{
	emit("\tpush    %d", r->value);
}

void emit (char *format, ...)
{
	va_list args;
	va_start(args, format);

	vfprintf(gen.fp, format, args);
	fprintf(gen.fp, "\n");

	va_end(args);
}
