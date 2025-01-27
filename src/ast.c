#include "ast.h"
#include "memory.h"

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
	AstExpressionNumber *number = mem_alloc(sizeof(*number));
	number->value = value;
	AstExpression *expression = mem_alloc(sizeof(*expression));
	expression->type = AstExpression_Number;
	expression->as.number = number;
	return expression;
}
