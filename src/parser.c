#include "parser.h"

typedef struct {
	Arena *arena;
	Token *tokens;
	int current;
	Ast *ast;
} Parser;

Parser parser;

Ast *parse (Arena *arena, Token *tokens)
{
	parser.arena = arena;
	parser.tokens = tokens;
	parser.current = 0;

	Ast *value = ast_initLiteral(parser.arena, tokens[0]);
	parser.ast = ast_initRoot(arena, value);

	return parser.ast;
}
