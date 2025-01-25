#include "parser.h"

AstRoot *getRoot (void);
AstLiteral *getLiteral (void);

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

	AstRoot *root = getRoot();
	Ast *ast = ast_init(arena, root);

	return ast;
}

AstRoot *getRoot (void)
{
	return ast_initRoot(parser.arena, getLiteral());
}

AstLiteral *getLiteral (void)
{
	return ast_initLiteral(parser.arena, parser.tokens[parser.current++]);
}
