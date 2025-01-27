#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "asm_generator.h"

void emit (char *format, ...);

typedef struct {
	FILE *fp;
} AsmGenerator;

AsmGenerator gen;

void asm_generate (Arena *arena, Ssa *ssa)
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
	emit("\tpush    %d", ssa->as.assign.value);
	emit("\tmov     rax, 60");
	emit("\tpop     rdi");
	emit("\tsyscall");

	fclose(gen.fp);
}

void emit (char *format, ...)
{
	va_list args;
	va_start(args, format);

	vfprintf(gen.fp, format, args);
	fprintf(gen.fp, "\n");

	va_end(args);
}
