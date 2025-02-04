#include <stdio.h>
#include <stdlib.h>

#include "ir_gen.h"
#include "memory.h"

void visitAst (Ast *ast);
void visitRoot (AstRoot *root);
void visitExpression (AstExpression *expression);
void visitExpressionBinary (AstExpressionBinary *expression);
void visitExpressionNumber (AstExpressionNumber *expression);
void visitExpressionUnary (AstExpressionUnary *expression);

void addInstruction (Ir *instruction);
Ir *getFirstInstruction (Ir *ir);

typedef struct {
	Ir *ir;
	int temp;
} IrGenerator;

static IrGenerator gen;

Ir *gen_ir (Ast *ast)
{
	gen.temp = 0;
	gen.ir = NULL;

	visitAst(ast);

	return getFirstInstruction(gen.ir);
}

void visitAst (Ast *ast)
{
	visitRoot(ast->root);
}

void visitRoot (AstRoot *root)
{
	visitExpression(root->expression);
}

void visitExpression (AstExpression *expression)
{
	switch (expression->type) {
		case AstExpression_Binary: visitExpressionBinary(expression->as.binary); break;
		case AstExpression_Number: visitExpressionNumber(expression->as.number); break;
		case AstExpression_Unary: visitExpressionUnary(expression->as.unary); break;
	}
}

void visitExpressionBinary (AstExpressionBinary *expression)
{
	visitExpression(expression->a);
	visitExpression(expression->b);
	switch (expression->operator.type) {
		case TT_Plus: addInstruction(ir_initAdd()); break;
		case TT_Minus: addInstruction(ir_initSub()); break;
		case TT_Star: addInstruction(ir_initMul()); break;
		default:
	}
}

void visitExpressionNumber (AstExpressionNumber *expression)
{
	char *buffer = mem_alloc(expression->value.length + 1);
	sprintf(buffer, "%.*s", expression->value.length, expression->value.lexeme);
	addInstruction(ir_initPush(atoi(buffer)));
}

void visitExpressionUnary (AstExpressionUnary *expression)
{
	visitExpression(expression->right);
	switch (expression->operator.type) {
		case TT_Minus: addInstruction(ir_initNeg()); break;
		default:
	}
}

void addInstruction (Ir *instruction)
{
	if (gen.ir != NULL) {
		gen.ir->next = instruction;
	}
	instruction->previous = gen.ir;
	gen.ir = instruction;
}

Ir *getFirstInstruction (Ir *ir)
{
	Ir *r;
	for (r = ir; r->previous != NULL; r = r->previous);
	return r;
}
