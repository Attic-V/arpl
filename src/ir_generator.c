#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ir_generator.h"

Ir *visitAst (Ast *ast);
Ir *visitRoot (AstRoot *root);
Ir *visitExpression (AstExpression *expression);
Ir *visitExpressionNumber (AstExpressionNumber *number);

typedef struct {
	Arena *arena;
	int temp;
} IrGenerator;

static IrGenerator gen;

Ir *ir_generate (Arena *arena, Ast *ast)
{
	gen.arena = arena;
	gen.temp = 0;
	return visitAst(ast);
}

Ir *visitAst (Ast *ast)
{
	return visitRoot(ast->root);
}

Ir *visitRoot (AstRoot *root)
{
	return visitExpression(root->expression);
}

Ir *visitExpression (AstExpression *expression)
{
	if (expression->type == AstExpression_Number) {
		return visitExpressionNumber(expression->as.number);
	}
	return NULL;
}

Ir *visitExpressionNumber (AstExpressionNumber *number)
{
	char *tempbuf = arena_allocate(gen.arena, (int)log10f(gen.temp ? gen.temp : 1) + 2);
	sprintf(tempbuf, "t%d", gen.temp++);
	char *valbuf = arena_allocate(gen.arena, number->value.length + 1);
	sprintf(valbuf, "%.*s", number->value.length, number->value.lexeme);
	return ir_initAssign(gen.arena, tempbuf, atoi(valbuf));
}
