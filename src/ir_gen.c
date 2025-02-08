#include <stdio.h>
#include <stdlib.h>

#include "ir_gen.h"
#include "linked_list.h"
#include "memory.h"

static void visitAst (Ast *ast);
static void visitRoot (AstRoot *root);
static void visitExpression (AstExpression *expression);
static void visitExpressionBinary (AstExpressionBinary *expression);
static void visitExpressionBoolean (AstExpressionBoolean *expression);
static void visitExpressionNumber (AstExpressionNumber *expression);
static void visitExpressionUnary (AstExpressionUnary *expression);

static void addInstruction (Ir *instruction);

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

static void visitAst (Ast *ast)
{
	visitRoot(ast->root);
}

static void visitRoot (AstRoot *root)
{
	visitExpression(root->expression);
}

static void visitExpression (AstExpression *expression)
{
	switch (expression->type) {
		case AstExpression_Binary: visitExpressionBinary(expression->as.binary); break;
		case AstExpression_Boolean: visitExpressionBoolean(expression->as.boolean); break;
		case AstExpression_Number: visitExpressionNumber(expression->as.number); break;
		case AstExpression_Unary: visitExpressionUnary(expression->as.unary); break;
	}
}

static void visitExpressionBinary (AstExpressionBinary *expression)
{
	visitExpression(expression->a);
	visitExpression(expression->b);
	switch (expression->operator.type) {
		case TT_And: addInstruction(ir_initAnd()); break;
		case TT_And_And: addInstruction(ir_initAnd()); break;
		case TT_Bang_Equal: addInstruction(ir_initNotEqu()); break;
		case TT_Caret: addInstruction(ir_initXor()); break;
		case TT_Equal_Equal: addInstruction(ir_initEqu()); break;
		case TT_Less: addInstruction(ir_initLess()); break;
		case TT_Less_Equal: addInstruction(ir_initLessEqu()); break;
		case TT_Pipe: addInstruction(ir_initOr()); break;
		case TT_Pipe_Pipe: addInstruction(ir_initOr()); break;
		case TT_Plus: addInstruction(ir_initAdd()); break;
		case TT_Minus: addInstruction(ir_initSub()); break;
		case TT_Star: addInstruction(ir_initMul()); break;
		default:
	}
}

static void visitExpressionBoolean (AstExpressionBoolean *expression)
{
	addInstruction(ir_initPush(expression->value));
}

static void visitExpressionNumber (AstExpressionNumber *expression)
{
	char *buffer = mem_alloc(expression->value.length + 1);
	sprintf(buffer, "%.*s", expression->value.length, expression->value.lexeme);
	addInstruction(ir_initPush(atoi(buffer)));
}

static void visitExpressionUnary (AstExpressionUnary *expression)
{
	visitExpression(expression->right);
	switch (expression->operator.type) {
		case TT_Bang:
			addInstruction(ir_initNot());
			addInstruction(ir_initPush(1));
			addInstruction(ir_initAnd());
			break;
		case TT_Minus: addInstruction(ir_initNeg()); break;
		case TT_Tilde: addInstruction(ir_initNot()); break;
		default:
	}
}

static void addInstruction (Ir *instruction)
{
	dll_insert(gen.current, instruction);
	gen.current = instruction;
}
