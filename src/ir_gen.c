#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ir_gen.h"
#include "memory.h"

Ir *visitAst (Ast *ast);
Ir *visitRoot (AstRoot *root);
Ir *visitExpression (AstExpression *expression);
Ir *visitExpressionNumber (AstExpressionNumber *number);

typedef struct {
	int temp;
} IrGenerator;

static IrGenerator gen;

Ir *gen_ir (Ast *ast)
{
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
	switch (expression->type) {
		case AstExpression_Number: return visitExpressionNumber(expression->as.number);
		default: return NULL;
	}
}

Ir *visitExpressionNumber (AstExpressionNumber *number)
{
	char *buffer = mem_alloc(number->value.length + 1);
	sprintf(buffer, "%.*s", number->value.length, number->value.lexeme);
	return ir_initPush(atoi(buffer));
}
