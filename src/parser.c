#include "parser.h"

typedef struct {
	Token *tokens;
	int current;
} Parser;

Parser parser;

Ast *parse (Arena *arena, Token *tokens)
{
	parser.tokens = tokens;
	parser.current = 0;

	AstLiteral *astLiteral = ast_initLiteral(arena, tokens[0]);
	AstRoot *astRoot = ast_initRoot(arena, astLiteral);
	Ast *ast = ast_init(arena, astRoot);

	return ast;
}
