#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "ir_gen.h"
#include "linked_list.h"
#include "memory.h"
#include "table.h"

#define pushLabels \
	int currentContinue = gen.continueLabel; \
	int currentBreak = gen.breakLabel;

#define popLabels \
	gen.continueLabel = currentContinue; \
	gen.breakLabel = currentBreak;

static void visitAst (Ast *ast);
static void visitRoot (AstRoot *root);

static void visitStatement (AstStatement *statement);
static void visitStatementBlock (AstStatementBlock *statement);
static void visitStatementBreakL (AstStatementBreakL *statement);
static void visitStatementCaseL (AstStatementCaseL *statement);
static void visitStatementConstD (AstStatementConstD *statement);
static void visitStatementContinueL (AstStatementContinueL *statement);
static void visitStatementDoWhile (AstStatementDoWhile *statement);
static void visitStatementExpr (AstStatementExpr *statement);
static void visitStatementForI (AstStatementForI *statement);
static void visitStatementIfE (AstStatementIfE *statement);
static void visitStatementInit (AstStatementInit *statement);
static void visitStatementReturnE (AstStatementReturnE *statement);
static void visitStatementSwitchC (AstStatementSwitchC *statement);
static void visitStatementVar (AstStatementVar *statement);
static void visitStatementWhileC (AstStatementWhileC *statement);

static void visitExpression (AstExpression *expression);
static void visitExpressionAccessElement (AstExpression *expression);
static void visitExpressionAssign (AstExpression *expression);
static void visitExpressionBinary (AstExpression *expression);
static void visitExpressionBoolean (AstExpression *expression);
static void visitExpressionNumber (AstExpression *expression);
static void visitExpressionPostfix (AstExpression *expression);
static void visitExpressionPrefix (AstExpression *expression);
static void visitExpressionTernary (AstExpression *expression);
static void visitExpressionVar (AstExpression *expression);

static void addInstruction (Ir *instruction);

typedef struct {
	Ir *current;
	int label;
	size_t reservedBytes;
	size_t tableSegment;
	Scope *scope;
	int continueLabel;
	int breakLabel;
	int nextCaseBodyLabel;
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

static void visitRoot (AstRoot *root)
{
	gen.reservedBytes = root->statement->as.block->scope->physicalSize;
	visitStatement(root->statement);
}

static void visitStatement (AstStatement *statement)
{
	switch (statement->type) {
		case AstStatement_Block: visitStatementBlock(statement->as.block); break;
		case AstStatement_BreakL: visitStatementBreakL(statement->as.breakL); break;
		case AstStatement_CaseL: visitStatementCaseL(statement->as.caseL); break;
		case AstStatement_ConstD: visitStatementConstD(statement->as.constD); break;
		case AstStatement_ContinueL: visitStatementContinueL(statement->as.continueL); break;
		case AstStatement_DoWhile: visitStatementDoWhile(statement->as.doWhile); break;
		case AstStatement_Expr: visitStatementExpr(statement->as.expr); break;
		case AstStatement_ForI: visitStatementForI(statement->as.forI); break;
		case AstStatement_IfE: visitStatementIfE(statement->as.ifE); break;
		case AstStatement_Init: visitStatementInit(statement->as.init); break;
		case AstStatement_ReturnE: visitStatementReturnE(statement->as.returnE); break;
		case AstStatement_SwitchC: visitStatementSwitchC(statement->as.switchC); break;
		case AstStatement_Var: visitStatementVar(statement->as.var); break;
		case AstStatement_WhileC: visitStatementWhileC(statement->as.whileC); break;
	}
}

static void visitStatementBlock (AstStatementBlock *statement)
{
	pushLabels
	gen.scope = statement->scope;
	for (AstStatement *stmt = statement->children; stmt != NULL; stmt = stmt->next) {
		visitStatement(stmt);
		gen.scope = statement->scope;
		popLabels
	}
}

static void visitStatementBreakL (AstStatementBreakL *statement)
{
	addInstruction(ir_initJmp(gen.breakLabel));
}

static void visitStatementCaseL (AstStatementCaseL *statement)
{
	int l0 = gen.label++;
	if (statement->e != NULL) {
		addInstruction(ir_initCopy());
		visitExpression(statement->e);
		addInstruction(ir_initEqu(dataType_getSize(statement->e->dataType)));
		addInstruction(ir_initJmpFalse(l0));
	}
	addInstruction(ir_initLabel(gen.nextCaseBodyLabel));
	pushLabels
	for (AstStatement *s = statement->body; s != NULL; s = s->next) {
		visitStatement(s);
		popLabels
	}
	gen.nextCaseBodyLabel = gen.label++;
	addInstruction(ir_initJmp(gen.nextCaseBodyLabel));
	addInstruction(ir_initLabel(l0));
}

static void visitStatementConstD (AstStatementConstD *statement)
{
	visitStatement(ast_initStatementInit(statement->identifier, statement->type, statement->expression, statement->operator));
}

static void visitStatementContinueL (AstStatementContinueL *statement)
{
	addInstruction(ir_initJmp(gen.continueLabel));
}

static void visitStatementDoWhile (AstStatementDoWhile *statement)
{
	int l0 = gen.label++;
	int l1 = gen.label++;
	int l2 = gen.label++;
	gen.continueLabel = l1;
	gen.breakLabel = l2;
	addInstruction(ir_initLabel(l0));
	visitStatement(statement->a);
	addInstruction(ir_initLabel(l1));
	visitExpression(statement->condition);
	addInstruction(ir_initJmpTrue(l0));
	addInstruction(ir_initLabel(l2));
}

static void visitStatementExpr (AstStatementExpr *statement)
{
	visitExpression(statement->expression);
	addInstruction(ir_initPop());
}

static void visitStatementForI (AstStatementForI *statement)
{
	int l0 = gen.label++;
	int l1 = gen.label++;
	int l2 = gen.label++;
	gen.continueLabel = l2;
	gen.breakLabel = l1;
	if (statement->init != NULL) {
		visitStatement(statement->init);
	}
	addInstruction(ir_initLabel(l0));
	if (statement->condition != NULL) {
		visitExpression(statement->condition);
		addInstruction(ir_initJmpFalse(l1));
	}
	if (statement->body != NULL) {
		visitStatement(statement->body);
	}
	addInstruction(ir_initLabel(l2));
	if (statement->update != NULL) {
		visitStatement(ast_initStatementExpr(statement->update));
	}
	addInstruction(ir_initJmp(l0));
	addInstruction(ir_initLabel(l1));
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

static void visitStatementInit (AstStatementInit *statement)
{
	AstStatement *svar = ast_initStatementVar(statement->identifier, statement->type);
	AstExpression *evar = ast_initExpressionVar(statement->identifier);
	evar->modifiable = true;
	evar->dataType = statement->type;
	visitStatement(svar);
	AstExpression *eassign = ast_initExpressionAssign(evar, statement->expression, statement->operator);
	visitExpression(eassign);
	addInstruction(ir_initPop());
}

static void visitStatementReturnE (AstStatementReturnE *statement)
{
	if (statement->expression != NULL) {
		visitExpression(statement->expression);
	}
	addInstruction(ir_initRet());
}

static void visitStatementSwitchC (AstStatementSwitchC *statement)
{
	int l0 = gen.label++;
	gen.breakLabel = l0;
	gen.nextCaseBodyLabel = gen.label++;
	visitExpression(statement->e);
	visitStatement(statement->body);
	addInstruction(ir_initLabel(gen.nextCaseBodyLabel));
	addInstruction(ir_initLabel(l0));
	addInstruction(ir_initPop());
}

static void visitStatementVar (AstStatementVar *statement)
{ }

static void visitStatementWhileC (AstStatementWhileC *statement)
{
	int l0 = gen.label++;
	int l1 = gen.label++;
	gen.continueLabel = l0;
	gen.breakLabel = l1;
	addInstruction(ir_initLabel(l0));
	visitExpression(statement->condition);
	addInstruction(ir_initJmpFalse(l1));
	if (statement->a != NULL) {
		visitStatement(statement->a);
	}
	addInstruction(ir_initJmp(l0));
	addInstruction(ir_initLabel(l1));
}

static void visitExpression (AstExpression *expression)
{
	switch (expression->type) {
		case AstExpression_AccessElement: visitExpressionAccessElement(expression); break;
		case AstExpression_Assign: visitExpressionAssign(expression); break;
		case AstExpression_Binary: visitExpressionBinary(expression); break;
		case AstExpression_Boolean: visitExpressionBoolean(expression); break;
		case AstExpression_Number: visitExpressionNumber(expression); break;
		case AstExpression_Postfix: visitExpressionPostfix(expression); break;
		case AstExpression_Prefix: visitExpressionPrefix(expression); break;
		case AstExpression_Ternary: visitExpressionTernary(expression); break;
		case AstExpression_Var: visitExpressionVar(expression); break;
	}
}

static void visitExpressionAccessElement (AstExpression *expression)
{
	AstExpressionAccessElement *e = expression->as.accessElement;
	visitExpression(e->a);
	visitExpression(e->b);
	addInstruction(ir_initPush(dataType_getSize(e->a->dataType->as.array->type)));
	addInstruction(ir_initMul(QWORD));
	addInstruction(ir_initSub(QWORD));
	if (!expression->modifiable) {
		addInstruction(ir_initDeref(dataType_getSize(e->a->dataType->as.array->type)));
	}
}

static void visitExpressionAssign (AstExpression *expression)
{
	AstExpressionAssign *e = expression->as.assign;
	visitExpression(e->a);
	size_t size = dataType_getSize(e->a->dataType);
	switch (e->operator.type) {
		case TT_Equal:
			visitExpression(e->b);
			break;
		case TT_Plus_Equal:
			visitExpression(e->a);
			addInstruction(ir_initDeref(size));
			visitExpression(e->b);
			addInstruction(ir_initAdd(size));
			break;
		case TT_Minus_Equal:
			visitExpression(e->a);
			addInstruction(ir_initDeref(size));
			visitExpression(e->b);
			addInstruction(ir_initSub(size));
			break;
		case TT_Star_Equal:
			visitExpression(e->a);
			addInstruction(ir_initDeref(size));
			visitExpression(e->b);
			addInstruction(ir_initMul(size));
			break;
		default:
	}
	addInstruction(ir_initAssign(size));
	addInstruction(ir_initDeref(size));
}

static void visitExpressionBinary (AstExpression *expression)
{
	AstExpressionBinary *e = expression->as.binary;
	visitExpression(e->a);
	visitExpression(e->b);
	size_t size = dataType_getSize(e->a->dataType);
	switch (e->operator.type) {
		case TT_And: addInstruction(ir_initAnd(size)); break;
		case TT_And_And: addInstruction(ir_initAnd(size)); break;
		case TT_Bang_Equal: addInstruction(ir_initNotEqu(size)); break;
		case TT_Caret: addInstruction(ir_initXor(size)); break;
		case TT_Equal_Equal: addInstruction(ir_initEqu(size)); break;
		case TT_Greater_Greater: addInstruction(ir_initSar(size)); break;
		case TT_Less: addInstruction(ir_initLess(size)); break;
		case TT_Less_Equal: addInstruction(ir_initLessEqu(size)); break;
		case TT_Less_Less: addInstruction(ir_initShl(size)); break;
		case TT_Pipe: addInstruction(ir_initOr(size)); break;
		case TT_Pipe_Pipe: addInstruction(ir_initOr(size)); break;
		case TT_Plus: addInstruction(ir_initAdd(size)); break;
		case TT_Minus: addInstruction(ir_initSub(size)); break;
		case TT_Star: addInstruction(ir_initMul(size)); break;
		default:
	}
}

static void visitExpressionBoolean (AstExpression *expression)
{
	AstExpressionBoolean *e = expression->as.boolean;
	addInstruction(ir_initPush(e->value));
}

static void visitExpressionNumber (AstExpression *expression)
{
	AstExpressionNumber *e = expression->as.number;
	char *buffer = mem_alloc(e->value.length + 1);
	sprintf(buffer, "%.*s", e->value.length, e->value.lexeme);
	addInstruction(ir_initPush(atoi(buffer)));
}

static void visitExpressionPostfix (AstExpression *expression)
{
	AstExpressionPostfix *e = expression->as.postfix;
	size_t size = dataType_getSize(e->e->dataType);
	visitExpression(e->e);
	addInstruction(ir_initDeref(size));
	visitExpression(e->e);
	switch (e->operator.type) {
		case TT_Plus_Plus: addInstruction(ir_initInc(size)); break;
		case TT_Minus_Minus: addInstruction(ir_initDec(size)); break;
		default:
	}
	addInstruction(ir_initPop());
}

static void visitExpressionPrefix (AstExpression *expression)
{
	AstExpressionPrefix *e = expression->as.prefix;
	visitExpression(e->e);
	size_t size = dataType_getSize(e->e->dataType);
	switch (e->operator.type) {
		case TT_And:
			break;
		case TT_Bang:
			addInstruction(ir_initNot(size));
			addInstruction(ir_initPush(1));
			addInstruction(ir_initAnd(size));
			break;
		case TT_Minus:
			addInstruction(ir_initNeg(size));
			break;
		case TT_Minus_Minus:
			addInstruction(ir_initDec(size));
			addInstruction(ir_initDeref(size));
			break;
		case TT_Plus_Plus:
			addInstruction(ir_initInc(size));
			addInstruction(ir_initDeref(size));
			break;
		case TT_Star:
			addInstruction(ir_initDeref(size));
			if (!expression->modifiable) {
				addInstruction(ir_initDeref(size));
			}
			break;
		case TT_Tilde:
			addInstruction(ir_initNot(size));
			break;
		default:
	}
}

static void visitExpressionTernary (AstExpression *expression)
{
	AstExpressionTernary *e = expression->as.ternary;
	int l0 = gen.label++;
	int l1 = gen.label++;
	visitExpression(e->condition);
	addInstruction(ir_initJmpFalse(l0));
	visitExpression(e->a);
	addInstruction(ir_initJmp(l1));
	addInstruction(ir_initLabel(l0));
	visitExpression(e->b);
	addInstruction(ir_initLabel(l1));
}

static void visitExpressionVar (AstExpression *expression)
{
	AstExpressionVar *e = expression->as.var;
	Symbol *s = telescope_get(gen.scope, e->identifier);
	size_t size = dataType_getSize(expression->dataType);
	if (expression->modifiable) {
		addInstruction(ir_initRef(s->physicalIndex + size));
	} else {
		addInstruction(ir_initVal(s->physicalIndex + size, size));
	}
}

static void addInstruction (Ir *instruction)
{
	dll_insert(gen.current, instruction);
	gen.current = instruction;
}
