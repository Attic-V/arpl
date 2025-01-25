#include "parser.h"

typedef struct {
	Arena *arena;
	Token *tokens;
	int current;
} Parser;

Parser parser;

Ast *parse (Arena *arena, Token *tokens)
{
	parser.arena = arena;
	parser.tokens = tokens;
	parser.current = 0;

	AstLiteral *astLiteral = ast_initLiteral(parser.arena, tokens[0]);
	AstRoot *astRoot = ast_initRoot(arena, astLiteral);
	Ast *ast = ast_init(arena, astRoot);

	return ast;
}
