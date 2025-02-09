#include "ast.h"
#include "linked_list.h"
#include "memory.h"

static AstStatementExpr *astStatement_initExpr (AstExpression *expression);

static AstExpressionBinary *astExpression_initBinary (AstExpression *a, AstExpression *b, Token operator);
static AstExpressionBoolean *astExpression_initBoolean (bool value);
static AstExpressionNumber *astExpression_initNumber (Token value);
static AstExpressionTernary *astExpression_initTernary (AstExpression *condition, AstExpression *a, AstExpression *b, Token operator);
static AstExpressionUnary *astExpression_initUnary (Token operator, AstExpression *right);

Ast *ast_init (AstRoot *root)
{
	Ast *ast = mem_alloc(sizeof(*ast));
	ast->root = root;
	return ast;
}

AstRoot *ast_initRoot (AstStatement *statement)
{
	AstRoot *root = mem_alloc(sizeof(*root));
	root->statement = statement;
	return root;
}

AstStatement *ast_initStatementExpr (AstExpression *expression)
{
	AstStatement *statement = mem_alloc(sizeof(*statement));
	statement->type = AstStatement_Expr;
	statement->as.expr = astStatement_initExpr(expression);
	dll_init(statement);
	return statement;
}

AstExpression *ast_initExpressionBinary (AstExpression *a, AstExpression *b, Token operator)
{
	AstExpression *expression = mem_alloc(sizeof(*expression));
	expression->type = AstExpression_Binary;
	expression->as.binary = astExpression_initBinary(a, b, operator);
	return expression;
}

AstExpression *ast_initExpressionBoolean (bool value)
{
	AstExpression *expression = mem_alloc(sizeof(*expression));
	expression->type = AstExpression_Boolean;
	expression->as.boolean = astExpression_initBoolean(value);
	return expression;
}

AstExpression *ast_initExpressionNumber (Token value)
{
	AstExpression *expression = mem_alloc(sizeof(*expression));
	expression->type = AstExpression_Number;
	expression->as.number = astExpression_initNumber(value);
	return expression;
}

AstExpression *ast_initExpressionTernary (AstExpression *condition, AstExpression *a, AstExpression *b, Token operator)
{
	AstExpression *expression = mem_alloc(sizeof(*expression));
	expression->type = AstExpression_Ternary;
	expression->as.ternary = astExpression_initTernary(condition, a, b, operator);
	return expression;
}

AstExpression *ast_initExpressionUnary (Token operator, AstExpression *right)
{
	AstExpression *expression = mem_alloc(sizeof(*expression));
	expression->type = AstExpression_Unary;
	expression->as.unary = astExpression_initUnary(operator, right);
	return expression;
}

static AstStatementExpr *astStatement_initExpr (AstExpression *expression)
{
	AstStatementExpr *expr = mem_alloc(sizeof(*expr));
	expr->expression = expression;
	return expr;
}

static AstExpressionBinary *astExpression_initBinary (AstExpression *a, AstExpression *b, Token operator)
{
	AstExpressionBinary *binary = mem_alloc(sizeof(*binary));
	binary->a = a;
	binary->b = b;
	binary->operator = operator;
	return binary;
}

static AstExpressionBoolean *astExpression_initBoolean (bool value)
{
	AstExpressionBoolean *boolean = mem_alloc(sizeof(*boolean));
	boolean->value = value;
	return boolean;
}

static AstExpressionNumber *astExpression_initNumber (Token value)
{
	AstExpressionNumber *number = mem_alloc(sizeof(*number));
	number->value = value;
	return number;
}

static AstExpressionTernary *astExpression_initTernary (AstExpression *condition, AstExpression *a, AstExpression *b, Token operator)
{
	AstExpressionTernary *ternary = mem_alloc(sizeof(*ternary));
	ternary->condition = condition;
	ternary->a = a;
	ternary->b = b;
	ternary->operator = operator;
	return ternary;
}

static AstExpressionUnary *astExpression_initUnary (Token operator, AstExpression *right)
{
	AstExpressionUnary *unary = mem_alloc(sizeof(*unary));
	unary->operator = operator;
	unary->right = right;
	return unary;
}
