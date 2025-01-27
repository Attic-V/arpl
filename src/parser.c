#include "parser.h"

AstRoot *getRoot (void);
AstExpression *getExpression (void);
AstExpression *getExpressionNumber (void);

typedef struct {
	Arena *arena;
	Token *tokens;
	int current;
} Parser;

static Parser parser;

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
	return ast_initRoot(parser.arena, getExpression());
}

AstExpression *getExpression (void)
{
	return getExpressionNumber();
}

AstExpression *getExpressionNumber (void)
{
	return ast_initExpressionNumber(parser.arena, parser.tokens[parser.current++]);
}
