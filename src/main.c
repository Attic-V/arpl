#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "data.h"
#include "file.h"
#include "ir_gen.h"
#include "memory.h"
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "x86_gen.h"

int main (int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: %s <path>\n", argv[0]);
		return 1;
	}

	char *path = argv[1];

	char *source = readFile(path);
	Token *tokens = scan(source);
	Ast *ast = parse(tokens);
	Ir *ir = gen_ir(ast);
	gen_x86(ir);

	mem_free();

	system("nasm -felf64 out.s -oout.o");
	system("ld out.o -oout");

	return 0;
}
