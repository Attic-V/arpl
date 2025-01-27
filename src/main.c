#include <stdio.h>
#include <stdlib.h>

#include "arena.h"
#include "x86_gen.h"
#include "ast.h"
#include "data.h"
#include "file.h"
#include "parser.h"
#include "scanner.h"
#include "ir_gen.h"
#include "token.h"

int main (int argc, char **argv)
{
	Arena *arena = arena_init(4 * MB);

	if (argc != 2) {
		printf("usage: %s <path>\n", argv[0]);
		return 1;
	}

	char *path = argv[1];

	char *source = readFile(arena, path);
	Token *tokens = scan(arena, source);
	Ast *ast = parse(arena, tokens);
	Ir *ir = gen_ir(arena, ast);
	gen_x86(arena, ir);

	arena_free(arena);

	system("nasm -felf64 out.s -oout.o");
	system("ld out.o -oout");

	return 0;
}
