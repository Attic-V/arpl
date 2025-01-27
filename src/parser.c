#include "memory.h"
#include "parser.h"

AstRoot *getRoot (void);
AstExpression *getExpression (void);
AstExpression *getExpressionNumber (void);

typedef struct {
	Token *tokens;
	int current;
} Parser;

static Parser parser;

Ast *parse (Token *tokens)
{
	parser.tokens = tokens;
	parser.current = 0;

	AstRoot *root = getRoot();
	Ast *ast = ast_init(root);

	return ast;
}

AstRoot *getRoot (void)
{
	return ast_initRoot(getExpression());
}

AstExpression *getExpression (void)
{
	return getExpressionNumber();
}

AstExpression *getExpressionNumber (void)
{
	return ast_initExpressionNumber(parser.tokens[parser.current++]);
}
