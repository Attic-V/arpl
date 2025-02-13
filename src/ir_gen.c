#include <stdio.h>
#include <stdlib.h>

#include "ir_gen.h"
#include "linked_list.h"
#include "memory.h"
#include "table.h"

static void visitAst (Ast *ast);
static void visitRoot (AstRoot *root);

static void visitStatement (AstStatement *statement);
static void visitStatementBlock (AstStatementBlock *statement);
static void visitStatementExpr (AstStatementExpr *statement);
static void visitStatementIfE (AstStatementIfE *statement);
static void visitStatementVar (AstStatementVar *statement);

static void visitExpression (AstExpression *expression);
static void visitExpressionAssign (AstExpressionAssign *expression);
static void visitExpressionBinary (AstExpressionBinary *expression);
static void visitExpressionBoolean (AstExpressionBoolean *expression);
static void visitExpressionNumber (AstExpressionNumber *expression);
static void visitExpressionTernary (AstExpressionTernary *expression);
static void visitExpressionUnary (AstExpressionUnary *expression);
static void visitExpressionVar (AstExpressionVar *expression);

static void addInstruction (Ir *instruction);

typedef struct {
	Ir *current;
	int label;
	size_t reservedBytes;
	Table *table;
} IrGenerator;

static IrGenerator gen;

Ir *gen_ir (Ast *ast)
{
	gen.current = NULL;
	gen.label = 0;
	gen.reservedBytes = 0;

	visitAst(ast);

	Ir *ir;
	for (ir = gen.current; ir != NULL && ir->previous != NULL; ir = ir->previous);

	Ir *reserve = ir_initReserve(gen.reservedBytes);
	dll_insert(reserve, ir);
	ir = reserve;

	return ir;
}

static void visitAst (Ast *ast)
{
	visitRoot(ast->root);
}

static void reserveSymbol (Symbol *symbol)
{
	gen.reservedBytes += getDtSize(symbol->type);
}

static void visitRoot (AstRoot *root)
{
	gen.table = root->table;
	table_apply(gen.table, reserveSymbol);
	visitStatement(root->statement);
}

static void visitStatement (AstStatement *statement)
{
	switch (statement->type) {
		case AstStatement_Block: visitStatementBlock(statement->as.block); break;
		case AstStatement_Expr: visitStatementExpr(statement->as.expr); break;
		case AstStatement_IfE: visitStatementIfE(statement->as.ifE); break;
		case AstStatement_Var: visitStatementVar(statement->as.var); break;
	}
}

static void visitStatementBlock (AstStatementBlock *statement)
{
	for (AstStatement *stmt = statement->children; stmt != NULL; stmt = stmt->next) {
		visitStatement(stmt);
	}
}

static void visitStatementExpr (AstStatementExpr *statement)
{
	visitExpression(statement->expression);
}

static void visitStatementIfE (AstStatementIfE *statement)
{
	int l0 = gen.label++;
	int l1 = gen.label++;
	visitExpression(statement->condition);
	addInstruction(ir_initJmpFalse(l0));
	visitStatement(statement->a);
	if (statement->b != NULL) {
		addInstruction(ir_initJmp(l1));
	}
	addInstruction(ir_initLabel(l0));
	if (statement->b != NULL) {
		visitStatement(statement->b);
		addInstruction(ir_initLabel(l1));
	}
}

static void visitStatementVar (AstStatementVar *statement)
{ }

static void visitExpression (AstExpression *expression)
{
	switch (expression->type) {
		case AstExpression_Assign: visitExpressionAssign(expression->as.assign); break;
		case AstExpression_Binary: visitExpressionBinary(expression->as.binary); break;
		case AstExpression_Boolean: visitExpressionBoolean(expression->as.boolean); break;
		case AstExpression_Number: visitExpressionNumber(expression->as.number); break;
		case AstExpression_Ternary: visitExpressionTernary(expression->as.ternary); break;
		case AstExpression_Unary: visitExpressionUnary(expression->as.unary); break;
		case AstExpression_Var: visitExpressionVar(expression->as.var); break;
	}
}

static void visitExpressionAssign (AstExpressionAssign *expression)
{
	addInstruction(ir_initRef(table_get(gen.table, expression->a->as.var->identifier)->physicalIndex));
	visitExpression(expression->b);
	addInstruction(ir_initAssign());
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
		case TT_Greater_Greater: addInstruction(ir_initSar()); break;
		case TT_Less: addInstruction(ir_initLess()); break;
		case TT_Less_Equal: addInstruction(ir_initLessEqu()); break;
		case TT_Less_Less: addInstruction(ir_initShl()); break;
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

static void visitExpressionTernary (AstExpressionTernary *expression)
{
	int l0 = gen.label++;
	int l1 = gen.label++;
	visitExpression(expression->condition);
	addInstruction(ir_initJmpFalse(l0));
	visitExpression(expression->a);
	addInstruction(ir_initJmp(l1));
	addInstruction(ir_initLabel(l0));
	visitExpression(expression->b);
	addInstruction(ir_initLabel(l1));
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

static void visitExpressionVar (AstExpressionVar *expression)
{
	addInstruction(ir_initVal(table_get(gen.table, expression->identifier)->physicalIndex));
}

static void addInstruction (Ir *instruction)
{
	dll_insert(gen.current, instruction);
	gen.current = instruction;
}
