#include <stdio.h>
#include <stdlib.h>

#include "arena.h"
#include "ast.h"
#include "file.h"
#include "parser.h"
#include "scanner.h"
#include "token.h"

int main (int argc, char **argv)
{
	Arena *arena = arena_init();

	if (argc != 2) {
		printf("usage: %s <path>\n", argv[0]);
		return 1;
	}

	char *path = argv[1];

	char *source = readFile(arena, path);
	Token *tokens = scan(arena, source);
	Ast *ast = parse(arena, tokens);

	(void)ast;

	arena_free(arena);

	return 0;
}
