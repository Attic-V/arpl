#include "ast.h"

Ast *ast_init (Arena *arena, AstRoot *root)
{
	Ast *ast = arena_allocate(arena, sizeof(*ast));
	ast->root = root;
	return ast;
}

AstRoot *ast_initRoot (Arena *arena, AstExpression *expression)
{
	AstRoot *root = arena_allocate(arena, sizeof(*root));
	root->expression = expression;
	return root;
}

AstExpression *ast_initExpressionNumber (Arena *arena, Token value)
{
	AstExpressionNumber *number = arena_allocate(arena, sizeof(*number));
	number->value = value;
	AstExpression *expression = arena_allocate(arena, sizeof(*expression));
	expression->type = AstExpression_Number;
	expression->as.number = number;
	return expression;
}
