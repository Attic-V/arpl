#include <stdio.h>
#include <stdlib.h>

#include "ir_gen.h"
#include "linked_list.h"
#include "memory.h"

void visitAst (Ast *ast);
void visitRoot (AstRoot *root);
void visitExpression (AstExpression *expression);
void visitExpressionBinary (AstExpressionBinary *expression);
void visitExpressionBoolean (AstExpressionBoolean *expression);
void visitExpressionNumber (AstExpressionNumber *expression);
void visitExpressionUnary (AstExpressionUnary *expression);

void addInstruction (Ir *instruction);

typedef struct {
	Ir *current;
} IrGenerator;

static IrGenerator gen;

Ir *gen_ir (Ast *ast)
{
	gen.current = NULL;

	visitAst(ast);

	Ir *ir;
	for (ir = gen.current; ir->previous != NULL; ir = ir->previous);
	return ir;
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
		case AstExpression_Boolean: visitExpressionBoolean(expression->as.boolean); break;
		case AstExpression_Number: visitExpressionNumber(expression->as.number); break;
		case AstExpression_Unary: visitExpressionUnary(expression->as.unary); break;
	}
}

void visitExpressionBinary (AstExpressionBinary *expression)
{
	visitExpression(expression->a);
	visitExpression(expression->b);
	switch (expression->operator.type) {
		case TT_Equal_Equal: addInstruction(ir_initEqu()); break;
		case TT_Plus: addInstruction(ir_initAdd()); break;
		case TT_Minus: addInstruction(ir_initSub()); break;
		case TT_Star: addInstruction(ir_initMul()); break;
		default:
	}
}

void visitExpressionBoolean (AstExpressionBoolean *expression)
{
	addInstruction(ir_initPush(expression->value));
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
		case TT_Bang: addInstruction(ir_initNot()); break;
		case TT_Minus: addInstruction(ir_initNeg()); break;
		default:
	}
}

void addInstruction (Ir *instruction)
{
	dll_insert(gen.current, instruction);
	gen.current = instruction;
}
