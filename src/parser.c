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

	parser.ast = ast_init(parser.arena);
	ast_setValue(parser.ast, tokens[0]);

	return parser.ast;
}
