#include "ast.h"
#include "memory.h"

AstExpressionNumber *astExpression_initNumber (Token value);

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

AstExpression *ast_initExpressionNumber (Token value)
{
	AstExpression *expression = mem_alloc(sizeof(*expression));
	expression->type = AstExpression_Number;
	expression->as.number = astExpression_initNumber(value);
	return expression;
}

AstExpressionNumber *astExpression_initNumber (Token value)
{
	AstExpressionNumber *number = mem_alloc(sizeof(*number));
	number->value = value;
	return number;
}
