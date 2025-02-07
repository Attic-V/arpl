#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "parser.h"

AstRoot *getRoot (void);
AstExpression *getExpression (void);
AstExpression *getExpressionEquality (void);
AstExpression *getExpressionPrimary (void);
AstExpression *getExpressionProduct (void);
AstExpression *getExpressionSum (void);
AstExpression *getExpressionUnary (void);

bool check (TokenType type);
Token consume (TokenType type, char *format, ...);
void error (Token location, char *format, ...);
void verror (Token location, char *format, va_list args);

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
	return getExpressionEquality();
}

AstExpression *getExpressionEquality (void)
{
	AstExpression *expression = getExpressionSum();
	while (check(TT_Equal_Equal)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionSum();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
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
	return getExpressionPrimary();
}

AstExpression *getExpressionPrimary (void)
{
	Token token = parser.tokens[parser.current++];
	switch (token.type) {
		case TT_Number:
			return ast_initExpressionNumber(token);
		case TT_LParen:
			AstExpression *expression = getExpression();
			consume(TT_RParen, "expected ')'");
			return expression;
		default:
	}
	error(token, "expected expression");
	exit(1);
}

bool check (TokenType type)
{
	return parser.tokens[parser.current].type == type;
}

Token consume (TokenType type, char *format, ...)
{
	Token token = parser.tokens[parser.current++];
	if (token.type != type) {
		va_list args;
		va_start(args, format);
		verror(token, format, args);
		va_end(args);
		exit(1);
	}
	return token;
}

void error (Token location, char *format, ...)
{
	va_list args;
	va_start(args, format);
	verror(location, format, args);
	va_end(args);
}

void verror (Token location, char *format, va_list args)
{
	fprintf(stderr, "%d: error at ", location.line);
	if (location.type == TT_EOF) {
		fprintf(stderr, "EOF");
	} else {
		fprintf(stderr, "'%.*s'", location.length, location.lexeme);
	}
	fprintf(stderr, ": ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
}
