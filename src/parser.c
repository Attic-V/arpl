#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "linked_list.h"
#include "memory.h"
#include "parser.h"

static AstRoot *getRoot (void);

static AstStatement *getStatement(void);
static AstStatement *getStatementBlock (void);
static AstStatement *getStatementDoWhile (void);
static AstStatement *getStatementExpr (void);
static AstStatement *getStatementForI (void);
static AstStatement *getStatementIfE (void);
static AstStatement *getStatementVar (void);
static AstStatement *getStatementWhileC (void);

static AstExpression *getExpression (void);
static AstExpression *getExpressionAndBitwise (void);
static AstExpression *getExpressionAndLogical (void);
static AstExpression *getExpressionAssign (void);
static AstExpression *getExpressionEquality (void);
static AstExpression *getExpressionOrBitwise (void);
static AstExpression *getExpressionOrLogical (void);
static AstExpression *getExpressionPrimary (void);
static AstExpression *getExpressionProduct (void);
static AstExpression *getExpressionRelational (void);
static AstExpression *getExpressionShift (void);
static AstExpression *getExpressionSum (void);
static AstExpression *getExpressionTernary (void);
static AstExpression *getExpressionUnaryPostfix (void);
static AstExpression *getExpressionUnaryPrefix (void);
static AstExpression *getExpressionXor (void);

static bool check (TokenType type);
static Token consume (TokenType type, char *format, ...);

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

static AstRoot *getRoot (void)
{
	return ast_initRoot(getStatementBlock());
}

static AstStatement *getStatement (void)
{
	switch (parser.tokens[parser.current].type) {
		case TT_Do: return getStatementDoWhile();
		case TT_For: return getStatementForI();
		case TT_If: return getStatementIfE();
		case TT_LBrace: return getStatementBlock();
		case TT_Var: return getStatementVar();
		case TT_While: return getStatementWhileC();
		default:
	}
	return getStatementExpr();
}

static AstStatement *getStatementBlock (void)
{
	consume(TT_LBrace, "expected '{'");
	AstStatement *statements = NULL;
	while (!check(TT_RBrace)) {
		AstStatement *statement = getStatement();
		dll_insert(statements, statement);
		statements = statement;
	}
	consume(TT_RBrace, "expected '}'");
	for (; statements != NULL && statements->previous != NULL; statements = statements->previous);
	return ast_initStatementBlock(statements);
}

static AstStatement *getStatementDoWhile (void)
{
	consume(TT_Do, "expected 'do'");
	AstStatement *statement = getStatement();
	Token keyword = consume(TT_While, "expected 'while'");
	consume(TT_LParen, "expected '('");
	AstExpression *condition = getExpression();
	consume(TT_RParen, "expected ')'");
	consume(TT_Semicolon, "expected ';'");
	return ast_initStatementDoWhile(statement, condition, keyword);
}

static AstStatement *getStatementExpr (void)
{
	AstStatement *statement = ast_initStatementExpr(getExpression());
	consume(TT_Semicolon, "expected ';'");
	return statement;
}

static AstStatement *getStatementForI (void)
{
	Token keyword = consume(TT_For, "expected 'for'");
	consume(TT_LParen, "expected '('");
	AstStatement *init = NULL;
	if (check(TT_Semicolon)) {
		parser.current++;
	} else {
		init = getStatement();
	}
	AstExpression *condition = NULL;
	if (!check(TT_Semicolon)) {
		condition = getExpression();
	}
	consume(TT_Semicolon, "expected ';'");
	AstExpression *update = NULL;
	if (!check(TT_RParen)) {
		update = getExpression();
	}
	consume(TT_RParen, "expected ')'");
	AstStatement *body = NULL;
	if (check(TT_Semicolon)) {
		parser.current++;
	} else {
		body = getStatement();
	}
	return ast_initStatementBlock(ast_initStatementForI(init, condition, update, body, keyword));
}

static AstStatement *getStatementIfE (void)
{
	Token keyword = consume(TT_If, "expected 'if'");
	consume(TT_LParen, "expected '('");
	AstExpression *condition = getExpression();
	consume(TT_RParen, "expected ')'");
	AstStatement *a = getStatement();
	AstStatement *b = NULL;
	if (check(TT_Else)) {
		parser.current++;
		b = getStatement();
	}
	return ast_initStatementIfE(condition, a, b, keyword);
}

static AstStatement *getStatementVar (void)
{
	consume(TT_Var, "expected 'var'");
	Token identifier = consume(TT_Identifier, "expected identifier");
	Token type;
	if (check(TT_Int) || check(TT_Bool)) {
		type = parser.tokens[parser.current++];
	} else {
		error(parser.tokens[parser.current++], "expected 'int' or 'bool'");
	}
	if (check(TT_Equal)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *expression = getExpression();
		consume(TT_Semicolon, "expected ';'");
		return ast_initStatementInit(identifier, type, expression, operator);
	} else {
		consume(TT_Semicolon, "expected ';'");
		return ast_initStatementVar(identifier, type);
	}
}

static AstStatement *getStatementWhileC (void)
{
	Token keyword = consume(TT_While, "expected 'while'");
	consume(TT_LParen, "expected '('");
	AstExpression *condition = getExpression();
	consume(TT_RParen, "expected ')'");
	AstStatement *statement = NULL;
	if (check(TT_Semicolon)) {
		parser.current++;
	} else {
		statement = getStatement();
	}
	return ast_initStatementWhileC(condition, statement, keyword);
}

static AstExpression *getExpression (void)
{
	return getExpressionAssign();
}

static AstExpression *getExpressionAssign (void)
{
	AstExpression *expression = getExpressionTernary();
	while (check(TT_Equal) || check(TT_Plus_Equal) || check(TT_Minus_Equal) || check(TT_Star_Equal)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpression();
		expression = ast_initExpressionAssign(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionTernary (void)
{
	AstExpression *expression = getExpressionOrLogical();
	if (check(TT_Question)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *a = getExpressionOrLogical();
		consume(TT_Colon, "expected ':'");
		AstExpression *b = getExpressionTernary();
		expression = ast_initExpressionTernary(expression, a, b, operator);
	}
	return expression;
}

static AstExpression *getExpressionOrLogical (void)
{
	AstExpression *expression = getExpressionAndLogical();
	while (check(TT_Pipe_Pipe)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionAndLogical();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionAndLogical (void)
{
	AstExpression *expression = getExpressionEquality();
	while (check(TT_And_And)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionEquality();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionEquality (void)
{
	AstExpression *expression = getExpressionRelational();
	while (check(TT_Equal_Equal) || check(TT_Bang_Equal)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionRelational();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionRelational (void)
{
	AstExpression *expression = getExpressionShift();
	while (check(TT_Less) || check(TT_Less_Equal)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionShift();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionShift (void)
{
	AstExpression *expression = getExpressionOrBitwise();
	while (check(TT_Less_Less) || check(TT_Greater_Greater)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionOrBitwise();
		expression  = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionOrBitwise (void)
{
	AstExpression *expression = getExpressionXor();
	while (check(TT_Pipe)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionXor();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionXor (void)
{
	AstExpression *expression = getExpressionAndBitwise();
	while (check(TT_Caret)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionAndBitwise();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionAndBitwise (void)
{
	AstExpression *expression = getExpressionSum();
	while (check(TT_And)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionSum();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionSum (void)
{
	AstExpression *expression = getExpressionProduct();
	while (check(TT_Plus) || check(TT_Minus)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionProduct();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionProduct (void)
{
	AstExpression *expression = getExpressionUnaryPrefix();
	while (check(TT_Star)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionUnaryPrefix();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionUnaryPrefix (void)
{
	if (check(TT_Minus) || check(TT_Bang) || check(TT_Tilde) || check(TT_Plus_Plus) || check(TT_Minus_Minus)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionUnaryPrefix();
		return ast_initExpressionPrefix(operator, right);
	}
	return getExpressionUnaryPostfix();
}

static AstExpression *getExpressionUnaryPostfix (void)
{
	AstExpression *left = getExpressionPrimary();
	while (check(TT_Plus_Plus) || check(TT_Minus_Minus)) {
		Token operator = parser.tokens[parser.current++];
		left = ast_initExpressionPostfix(operator, left);
	}
	return left;
}

static AstExpression *getExpressionPrimary (void)
{
	Token token = parser.tokens[parser.current++];
	switch (token.type) {
		case TT_Number:
			return ast_initExpressionNumber(token);
		case TT_LParen:
			AstExpression *expression = getExpression();
			consume(TT_RParen, "expected ')'");
			return expression;
		case TT_True:
			return ast_initExpressionBoolean(true);
		case TT_False:
			return ast_initExpressionBoolean(false);
		case TT_Identifier:
			return ast_initExpressionVar(token);
		default:
	}
	error(token, "expected expression");
	exit(1);
}

static bool check (TokenType type)
{
	return parser.tokens[parser.current].type == type;
}

static Token consume (TokenType type, char *format, ...)
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
