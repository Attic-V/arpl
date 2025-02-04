#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "parser.h"

AstRoot *getRoot (void);
AstExpression *getExpression (void);
AstExpression *getExpressionNumber (void);
AstExpression *getExpressionProduct (void);
AstExpression *getExpressionSum (void);
AstExpression *getExpressionUnary (void);

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
	return getExpressionSum();
}

AstExpression *getExpressionSum (void)
{
	AstExpression *expression = getExpressionProduct();
	while (check(TT_Plus) || check(TT_Minus)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionProduct();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

AstExpression *getExpressionProduct (void)
{
	AstExpression *expression = getExpressionUnary();
	while (check(TT_Star)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionUnary();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

AstExpression *getExpressionUnary (void)
{
	if (check(TT_Minus)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionUnary();
		return ast_initExpressionUnary(operator, right);
	}
	return getExpressionNumber();
}

AstExpression *getExpressionNumber (void)
{
	Token token = parser.tokens[parser.current++];
	if (token.type != TT_Number) {
		fprintf(stderr, "%d: error at '%.*s': expected number\n", token.line, token.length, token.lexeme);
		exit(1);
	}
	return ast_initExpressionNumber(token);
}

bool check (TokenType type)
{
	return parser.tokens[parser.current].type == type;
}
