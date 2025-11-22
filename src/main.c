#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "data.h"
#include "file.h"
#include "memory.h"
#include "parser.h"
#include "scanner.h"
#include "token.h"

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

	mem_free();

	return 0;
}
