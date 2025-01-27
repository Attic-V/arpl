#include <math.h>
#include <stdio.h>
#include <string.h>

#include "tac_generator.h"

Tac *visitAst (Ast *ast);
Tac *visitRoot (AstRoot *root);
Tac *visitExpression (AstExpression *expression);
Tac *visitExpressionNumber (AstExpressionNumber *number);

typedef struct {
	Arena *arena;
	int temp;
} Generator;

Generator generator;

Tac *tac_generate (Arena *arena, Ast *ast)
{
	generator.arena = arena;
	generator.temp = 0;
	return visitAst(ast);
}

Tac *visitAst (Ast *ast)
{
	return visitRoot(ast->root);
}

Tac *visitRoot (AstRoot *root)
{
	return visitExpression(root->expression);
}

Tac *visitExpression (AstExpression *expression)
{
	if (expression->type == AstExpression_Number) {
		return visitExpressionNumber(expression->as.number);
	}
	return NULL;
}

Tac *visitExpressionNumber (AstExpressionNumber *number)
{
	char *buffer = arena_allocate(generator.arena, (int)log10f(generator.temp ? generator.temp : 1) + 2);
	sprintf(buffer, "t%d", generator.temp++);
	return tac_initAssign(generator.arena, buffer, number->value);
}
