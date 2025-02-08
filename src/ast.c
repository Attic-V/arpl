#include "ast.h"
#include "memory.h"

AstExpressionBinary *astExpression_initBinary (AstExpression *a, AstExpression *b, Token operator);
AstExpressionBoolean *astExpression_initBoolean (bool value);
AstExpressionNumber *astExpression_initNumber (Token value);
AstExpressionUnary *astExpression_initUnary (Token operator, AstExpression *right);

Ast *ast_init (AstRoot *root)
{
	Ast *ast = mem_alloc(sizeof(*ast));
	ast->root = root;
	return ast;
}

AstRoot *ast_initRoot (AstExpression *expression)
{
	AstRoot *root = mem_alloc(sizeof(*root));
	root->expression = expression;
	return root;
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

AstExpression *ast_initExpressionUnary (Token operator, AstExpression *right)
{
	AstExpression *expression = mem_alloc(sizeof(*expression));
	expression->type = AstExpression_Unary;
	expression->as.unary = astExpression_initUnary(operator, right);
	return expression;
}

AstExpressionBinary *astExpression_initBinary (AstExpression *a, AstExpression *b, Token operator)
{
	AstExpressionBinary *binary = mem_alloc(sizeof(*binary));
	binary->a = a;
	binary->b = b;
	binary->operator = operator;
	return binary;
}

AstExpressionBoolean *astExpression_initBoolean (bool value)
{
	AstExpressionBoolean *boolean = mem_alloc(sizeof(*boolean));
	boolean->value = value;
	return boolean;
}

AstExpressionNumber *astExpression_initNumber (Token value)
{
	AstExpressionNumber *number = mem_alloc(sizeof(*number));
	number->value = value;
	return number;
}

AstExpressionUnary *astExpression_initUnary (Token operator, AstExpression *right)
{
	AstExpressionUnary *unary = mem_alloc(sizeof(*unary));
	unary->operator = operator;
	unary->right = right;
	return unary;
}
