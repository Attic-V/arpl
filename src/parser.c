#include <stdbool.h>

#include "memory.h"
#include "parser.h"

AstRoot *getRoot (void);
AstExpression *getExpression (void);
AstExpression *getExpressionBinary (void);
AstExpression *getExpressionNumber (void);

bool check (TokenType type);

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
	return getExpressionBinary();
}

AstExpression *getExpressionBinary (void)
{
	AstExpression *expression = getExpressionNumber();
	while (check(TT_Plus) || check(TT_Minus)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionNumber();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

AstExpression *getExpressionNumber (void)
{
	return ast_initExpressionNumber(parser.tokens[parser.current++]);
}

bool check (TokenType type)
{
	return parser.tokens[parser.current].type == type;
}
