#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tac_generator.h"

Tac *visitAst (Ast *ast);
Tac *visitRoot (AstRoot *root);
Tac *visitExpression (AstExpression *expression);
Tac *visitExpressionNumber (AstExpressionNumber *number);

typedef struct {
	Arena *arena;
	int temp;
} TacGenerator;

static TacGenerator gen;

Tac *tac_generate (Arena *arena, Ast *ast)
{
	gen.arena = arena;
	gen.temp = 0;
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
	char *tempbuf = arena_allocate(gen.arena, (int)log10f(gen.temp ? gen.temp : 1) + 2);
	sprintf(tempbuf, "t%d", gen.temp++);
	char *valbuf = arena_allocate(gen.arena, number->value.length + 1);
	sprintf(valbuf, "%.*s", number->value.length, number->value.lexeme);
	return tac_initAssign(gen.arena, tempbuf, atoi(valbuf));
}
