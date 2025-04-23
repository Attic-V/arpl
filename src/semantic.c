#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "linked_list.h"
#include "memory.h"
#include "semantic.h"

static void visitAst (Ast *ast);
static void visitRoot (AstRoot *node);

static void visitDeclaration (AstDeclaration *node);
static void visitDeclarationFunction (AstDeclarationFunction *node);

static void visitStatement (AstStatement *node);
static void visitStatementBlock (AstStatementBlock *node);
static void visitStatementBreakL (AstStatementBreakL *node);
static void visitStatementCaseL (AstStatementCaseL *node);
static void visitStatementContinueL (AstStatementContinueL *node);
static void visitStatementDoWhile (AstStatementDoWhile *node);
static void visitStatementExpr (AstStatementExpr *node);
static void visitStatementForI (AstStatementForI *node);
static void visitStatementIfE (AstStatementIfE *node);
static void visitStatementInit (AstStatementInit *node);
static void visitStatementReturnE (AstStatementReturnE *node);
static void visitStatementSwitchC (AstStatementSwitchC *node);
static void visitStatementVar (AstStatementVar *node);
static void visitStatementWhileC (AstStatementWhileC *node);

static void visitExpression (AstExpression *node);
static void visitExpressionAssign (AstExpressionAssign *node);
static void visitExpressionBinary (AstExpressionBinary *node);
static void visitExpressionCall (AstExpressionCall *node);
static void visitExpressionCast (AstExpressionCast *node);
static void visitExpressionBoolean (AstExpressionBoolean *node);
static void visitExpressionNumber (AstExpressionNumber *node);
static void visitExpressionPostfix (AstExpressionPostfix *node);
static void visitExpressionPrefix (AstExpressionPrefix *node);
static void visitExpressionTernary (AstExpressionTernary *node);
static void visitExpressionVar (AstExpressionVar *node);

static bool canCoerce (DataType *from, DataType *to);
static Token getCoercionToken (void);
static bool coerce (AstExpression* e, DataType *target);

static bool coercionMatrix[DataTypeType_Count][DataTypeType_Count] = {
	[DataType_U8][DataType_U16] = 1,
	[DataType_U8][DataType_U32] = 1,
	[DataType_U8][DataType_U64] = 1,
	[DataType_U16][DataType_U32] = 1,
	[DataType_U16][DataType_U64] = 1,
	[DataType_U32][DataType_U64] = 1,

	[DataType_I8][DataType_I16] = 1,
	[DataType_I8][DataType_I32] = 1,
	[DataType_I8][DataType_I64] = 1,
	[DataType_I16][DataType_I32] = 1,
	[DataType_I16][DataType_I64] = 1,
	[DataType_I32][DataType_I64] = 1,

	[DataType_U8][DataType_I8] = 1,
	[DataType_U8][DataType_I16] = 1,
	[DataType_U8][DataType_I32] = 1,
	[DataType_U8][DataType_I64] = 1,
	[DataType_U16][DataType_I16] = 1,
	[DataType_U16][DataType_I32] = 1,
	[DataType_U16][DataType_I64] = 1,
	[DataType_U32][DataType_I32] = 1,
	[DataType_U32][DataType_I64] = 1,
	[DataType_U64][DataType_I64] = 1,

	[DataType_I8][DataType_U8] = 1,
	[DataType_I8][DataType_U16] = 1,
	[DataType_I8][DataType_U32] = 1,
	[DataType_I8][DataType_U64] = 1,
	[DataType_I16][DataType_U16] = 1,
	[DataType_I16][DataType_U32] = 1,
	[DataType_I16][DataType_U64] = 1,
	[DataType_I32][DataType_U32] = 1,
	[DataType_I32][DataType_U64] = 1,
	[DataType_I64][DataType_U64] = 1,
};

typedef struct {
	Scope *previousScope;
	Scope *currentScope;
	size_t currentPhysicalIndex;
	bool hadError;
	bool canContinue;
	bool canBreak;
	DataType *caseExpressionType;
	DataType *functionReturnType;
} Analyzer;

static Analyzer analyzer;

void analyze (Ast *ast)
{
	analyzer.previousScope = NULL;
	analyzer.currentScope = NULL;
	analyzer.currentPhysicalIndex = 0;
	analyzer.hadError = false;
	analyzer.canContinue = false;
	analyzer.canBreak = false;

	visitAst(ast);

	if (analyzer.hadError) {
		exit(1);
	}
}

static void visitAst (Ast *ast)
{
	visitRoot(ast->root);
}

static void visitRoot (AstRoot *node)
{
	analyzer.currentScope = node->scope = scope_init();
	node->scope->parent = analyzer.previousScope;
	bool canContinue = analyzer.canContinue;
	bool canBreak = analyzer.canBreak;
	DataType *caseExpressionType = analyzer.caseExpressionType;
	for (AstDeclaration *decl = node->declarations; decl != NULL; decl = decl->next) {
		analyzer.previousScope = node->scope;
		visitDeclaration(decl);
		analyzer.currentScope = node->scope;
		analyzer.canContinue = canContinue;
		analyzer.canBreak = canBreak;
		analyzer.caseExpressionType = caseExpressionType;
	}
	if (node->scope->parent != NULL) {
		node->scope->parent->physicalSize += node->scope->physicalSize;
	}
	if (!scope_get(node->scope, (Token){
		.type = TT_Identifier,
		.lexeme = "main",
		.length = 4,
		.line = 0,
	})) {
		analyzer.hadError = true;
		error((Token){
			.type = TT_EOF,
		}, "missing main");
	}
}

static void visitDeclaration (AstDeclaration *node)
{
	switch (node->type) {
		case AstDeclaration_Function: visitDeclarationFunction(node->as.function); break;
	}
}

static void visitDeclarationFunction (AstDeclarationFunction *node)
{
	Symbol *symbol = symbol_init(node->identifier, node->dataType);
	if (scope_add(analyzer.currentScope, symbol)) {
		symbol->physicalIndex = analyzer.currentPhysicalIndex;
		analyzer.currentPhysicalIndex += dataType_getSize(symbol->type);
	} else {
		error(node->identifier, "identifier already exists in scope");
	}
	analyzer.functionReturnType = node->dataType->as.function->returnType;
	visitStatement(node->body);
}

static void visitStatement (AstStatement *node)
{
	switch (node->type) {
		case AstStatement_Block: visitStatementBlock(node->as.block); break;
		case AstStatement_BreakL: visitStatementBreakL(node->as.breakL); break;
		case AstStatement_CaseL: visitStatementCaseL(node->as.caseL); break;
		case AstStatement_ContinueL: visitStatementContinueL(node->as.continueL); break;
		case AstStatement_DoWhile: visitStatementDoWhile(node->as.doWhile); break;
		case AstStatement_Expr: visitStatementExpr(node->as.expr); break;
		case AstStatement_ForI: visitStatementForI(node->as.forI); break;
		case AstStatement_IfE: visitStatementIfE(node->as.ifE); break;
		case AstStatement_Init: visitStatementInit(node->as.init); break;
		case AstStatement_ReturnE: visitStatementReturnE(node->as.returnE); break;
		case AstStatement_SwitchC: visitStatementSwitchC(node->as.switchC); break;
		case AstStatement_Var: visitStatementVar(node->as.var); break;
		case AstStatement_WhileC: visitStatementWhileC(node->as.whileC); break;
	}
}

static void visitStatementBlock (AstStatementBlock *node)
{
	analyzer.currentScope = node->scope = scope_init();
	node->scope->parent = analyzer.previousScope;
	bool canContinue = analyzer.canContinue;
	bool canBreak = analyzer.canBreak;
	DataType *caseExpressionType = analyzer.caseExpressionType;
	for (AstStatement *stmt = node->children; stmt != NULL; stmt = stmt->next) {
		analyzer.previousScope = node->scope;
		visitStatement(stmt);
		analyzer.currentScope = node->scope;
		analyzer.canContinue = canContinue;
		analyzer.canBreak = canBreak;
		analyzer.caseExpressionType = caseExpressionType;
	}
	if (node->scope->parent != NULL) {
		node->scope->parent->physicalSize += node->scope->physicalSize;
	}
}

static void visitStatementBreakL (AstStatementBreakL *node)
{
	if (!analyzer.canBreak) {
		analyzer.hadError = true;
		error(node->keyword, "cannot be used here");
	}
}

static void visitStatementCaseL (AstStatementCaseL *node)
{
	if (node->e != NULL) {
		visitExpression(node->e);
		if (!dataType_equal(node->e->dataType, analyzer.caseExpressionType)) {
			error(node->keyword, "case expression type must match type of expression in switch");
		}
		node->e->modifiable = false;
	}
	for (AstStatement *s = node->body; s != NULL; s = s->next) {
		visitStatement(s);
	}
}

static void visitStatementContinueL (AstStatementContinueL *node)
{
	if (!analyzer.canContinue) {
		analyzer.hadError = true;
		error(node->keyword, "cannot be used here");
	}
}

static void visitStatementDoWhile (AstStatementDoWhile *node)
{
	analyzer.canContinue = true;
	analyzer.canBreak = true;
	visitStatement(node->a);
	visitExpression(node->condition);
	if (!dataType_isBoolean(node->condition->dataType)) {
		error(node->keyword, "condition must be a boolean");
	}
}

static void visitStatementExpr (AstStatementExpr *node)
{
	visitExpression(node->expression);
	node->expression->modifiable = false;
}

static void visitStatementForI (AstStatementForI *node)
{
	if (node->init != NULL) {
		visitStatement(node->init);
	}
	analyzer.canContinue = true;
	analyzer.canBreak = true;
	if (node->condition != NULL) {
		visitExpression(node->condition);
		if (!dataType_isBoolean(node->condition->dataType)) {
			error(node->keyword, "condition must be a boolean");
		}
	}
	if (node->update != NULL) {
		visitExpression(node->update);
	}
	if (node->body != NULL) {
		visitStatement(node->body);
	}
}

static void visitStatementIfE (AstStatementIfE *node)
{
	visitExpression(node->condition);
	if (!dataType_isBoolean(node->condition->dataType)) {
		error(node->keyword, "condition must be a boolean");
	}
	visitStatement(node->a);
	if (node->b != NULL) {
		visitStatement(node->b);
	}
}

static void visitStatementInit (AstStatementInit *node)
{
	AstStatement *statementVar = ast_initStatementVar(node->identifier, node->type);
	AstExpression *expressionVar = ast_initExpressionVar(node->identifier);
	AstExpression *expressionAssign = ast_initExpressionAssign(expressionVar, node->expression, node->operator);

	visitStatement(statementVar);
	Symbol *symbol = scope_get(analyzer.currentScope, node->identifier);
	Mutability mutability = symbol->type->mutability;
	symbol->type->mutability = M_Mutable;
	visitExpression(expressionAssign);
	symbol->type->mutability = mutability;
}

static void visitStatementReturnE (AstStatementReturnE *node)
{
	if (node->expression != NULL) {
		visitExpression(node->expression);
		node->expression->modifiable = false;
		if (!dataType_equal(node->expression->dataType, analyzer.functionReturnType)) {
			if (!coerce(node->expression, analyzer.functionReturnType)) {
				analyzer.hadError = true;
				error(node->keyword, "type of expression in return does not match function return type");
			}
		}
	}
}

static void visitStatementSwitchC (AstStatementSwitchC *node)
{
	visitExpression(node->e);
	analyzer.caseExpressionType = node->e->dataType;
	analyzer.canBreak = true;
	visitStatement(node->body);
	node->e->modifiable = false;
}

static void visitStatementVar (AstStatementVar *node)
{
	Symbol *symbol = symbol_init(node->identifier, node->type);
	if (scope_add(analyzer.currentScope, symbol)) {
		symbol->physicalIndex = analyzer.currentPhysicalIndex;
		analyzer.currentPhysicalIndex += dataType_getSize(symbol->type);
	} else {
		error(node->identifier, "variable has already been declared in scope");
	}
}

static void visitStatementWhileC (AstStatementWhileC *node)
{
	analyzer.canContinue = true;
	analyzer.canBreak = true;
	visitExpression(node->condition);
	if (!dataType_isBoolean(node->condition->dataType)) {
		error(node->keyword, "condition must be a boolean");
	}
	if (node->a != NULL) {
		visitStatement(node->a);
	}
}

static void visitExpression (AstExpression *node)
{
	switch (node->type) {
		case AstExpression_Assign:
			visitExpressionAssign(node->as.assign);
			node->dataType = node->as.assign->a->dataType;
			break;
		case AstExpression_Binary:
			visitExpressionBinary(node->as.binary);
			switch (node->as.binary->operator.type) {
				case TT_And:
				case TT_Caret:
				case TT_Greater_Greater:
				case TT_Less_Less:
				case TT_Pipe:
				case TT_Plus:
				case TT_Minus:
				case TT_Star:
					node->dataType = node->as.binary->a->dataType;
					break;
				case TT_And_And:
				case TT_Bang_Equal:
				case TT_Equal_Equal:
				case TT_Less:
				case TT_Pipe_Pipe:
					node->dataType = dataType_initBoolean();
					break;
				default:
			}
			break;
		case AstExpression_Boolean:
			visitExpressionBoolean(node->as.boolean);
			node->dataType = dataType_initBoolean();
			break;
		case AstExpression_Call:
			visitExpressionCall(node->as.call);
			node->dataType = node->as.call->e->dataType->as.function->returnType;
			break;
		case AstExpression_Cast:
			visitExpressionCast(node->as.cast);
			node->dataType = node->as.cast->to;
			break;
		case AstExpression_Number:
			visitExpressionNumber(node->as.number);
			char *buffer = mem_alloc(node->as.number->value.length + 1);
			sprintf(buffer, "%.*s", node->as.number->value.length, node->as.number->value.lexeme);
			size_t value = atoi(buffer);
			node->dataType = dataType_smallestInt(value);
			break;
		case AstExpression_Postfix:
			switch (node->as.postfix->operator.type) {
				case TT_Plus_Plus:
					node->dataType = node->as.postfix->e->dataType;
					break;
				default:
			}
			visitExpressionPostfix(node->as.postfix);
			break;
		case AstExpression_Prefix:
			visitExpressionPrefix(node->as.prefix);
			switch (node->as.prefix->operator.type) {
				case TT_Bang:
					node->dataType = dataType_initBoolean();
					break;
				case TT_Minus:
				case TT_Plus_Plus:
				case TT_Tilde:
					node->dataType = node->as.prefix->e->dataType;
					break;
				case TT_And:
					node->dataType = dataType_initPointer(node->as.prefix->e->dataType);
					break;
				case TT_Star:
					node->dataType = node->as.prefix->e->dataType->as.pointer->to;
					node->modifiable = true;
					break;
				default:
			}
			break;
		case AstExpression_Ternary:
			visitExpressionTernary(node->as.ternary);
			node->dataType = node->as.ternary->a->dataType;
			break;
		case AstExpression_Var:
			Symbol *symbol = telescope_get(analyzer.currentScope, node->as.var->identifier);
			if (symbol == NULL) {
				error(node->as.var->identifier, "undeclared identifier");
				analyzer.hadError = true;
				break;
			}
			node->dataType = symbol->type;
			visitExpressionVar(node->as.var);
			node->modifiable = true;
			break;
	}
}

static void visitExpressionAssign (AstExpressionAssign *node)
{
	visitExpression(node->a);
	visitExpression(node->b);
	if (!dataType_mutable(node->a->dataType)) {
		analyzer.hadError = true;
		error(node->operator, "left operand is immutable");
	} else if (!dataType_equal(node->a->dataType, node->b->dataType)) {
		if (!coerce(node->b, node->a->dataType)) {
			analyzer.hadError = true;
			error(node->operator, "operands must have the same type");
		}
	}
	if (!node->a->modifiable) {
		error(node->operator, "assignee must be modifiable");
		analyzer.hadError = true;
	}
	node->b->modifiable = false;
}

static void visitExpressionBinary (AstExpressionBinary *node)
{
	visitExpression(node->a);
	visitExpression(node->b);
	switch (node->operator.type) {
		case TT_And:
		case TT_Caret:
		case TT_Greater_Greater:
		case TT_Less:
		case TT_Less_Less:
		case TT_Pipe:
		case TT_Plus:
		case TT_Minus:
		case TT_Star:
			if (!dataType_isInt(node->a->dataType) || !dataType_isInt(node->b->dataType)) {
				error(node->operator, "operands must be numbers");
			}
			break;
		case TT_And_And:
		case TT_Pipe_Pipe:
			if (!dataType_isBoolean(node->a->dataType) || !dataType_isBoolean(node->b->dataType)) {
				error(node->operator, "operands must be booleans");
			}
			break;
		case TT_Bang_Equal:
		case TT_Equal_Equal:
			break;
		default:
	}
	if (!dataType_equal(node->a->dataType, node->b->dataType)) {
		if (!coerce(node->a, node->b->dataType) && !coerce(node->b, node->a->dataType)) {
			error(node->operator, "operands must have the same type");
		}
	}
	node->a->modifiable = false;
	node->b->modifiable = false;
}

static void visitExpressionBoolean (AstExpressionBoolean *node)
{ }

static void visitExpressionCall (AstExpressionCall *node)
{
	visitExpression(node->e);
	if (!dataType_isFunction(node->e->dataType)) {
		error(node->lparen, "cannot call non-function");
		exit(1);
	}
}

static void visitExpressionCast (AstExpressionCast *node)
{
	visitExpression(node->e);
	if (!dataType_castable(node->e->dataType, node->to)) {
		analyzer.hadError = true;
		error(node->operator, "invalid cast");
	}
	node->e->modifiable = false;
}

static void visitExpressionNumber (AstExpressionNumber *node)
{ }

static void visitExpressionPostfix (AstExpressionPostfix *node)
{
	visitExpression(node->e);
	switch (node->operator.type) {
		case TT_Minus_Minus:
		case TT_Plus_Plus:
			if (!dataType_isInt(node->e->dataType)) {
				error(node->operator, "operand must be a number");
			}
			if (!node->e->modifiable) {
				error(node->e->as.var->identifier, "expression must be modifiable");
				analyzer.hadError = true;
			}
			if (!dataType_mutable(node->e->dataType)) {
				analyzer.hadError = true;
				error(node->operator, "operand is immutable");
			}
			break;
		default:
	}
}

static void visitExpressionPrefix (AstExpressionPrefix *node)
{
	visitExpression(node->e);
	switch (node->operator.type) {
		case TT_Bang:
			if (!dataType_isBoolean(node->e->dataType)) {
				error(node->operator, "operand must be a boolean");
			}
			break;
		case TT_Minus:
		case TT_Tilde:
			if (!dataType_isInt(node->e->dataType)) {
				error(node->operator, "operand must be a number");
			}
			break;
		case TT_Minus_Minus:
		case TT_Plus_Plus:
			if (!dataType_isInt(node->e->dataType)) {
				error(node->operator, "operand must be a number");
			}
			if (!node->e->modifiable) {
				error(node->e->as.var->identifier, "expression must be modifiable");
				analyzer.hadError = true;
			}
			if (!dataType_mutable(node->e->dataType)) {
				analyzer.hadError = true;
				error(node->operator, "operand is immutable");
			}
			break;
		case TT_And:
			if (!node->e->modifiable) {
				analyzer.hadError = true;
				error(node->operator, "operand must be modifiable");
			}
			break;
		case TT_Star:
			if (!dataType_isPointer(node->e->dataType)) {
				error(node->operator, "operand must be a pointer");
				exit(1);
			}
			break;
		default:
	}
}

static void visitExpressionTernary (AstExpressionTernary *node)
{
	AstExpression *c = node->condition;
	AstExpression *a = node->a;
	AstExpression *b = node->b;
	visitExpression(c);
	visitExpression(a);
	visitExpression(b);
	if (!dataType_isBoolean(c->dataType)) {
		error(node->operator, "condition must be a boolean");
	}
	if (!dataType_equal(a->dataType, b->dataType)) {
		if (!coerce(node->a, node->b->dataType) && !coerce(node->b, node->a->dataType)) {
			error(node->operator, "operands must have the same type");
		}
	}
	c->modifiable = false;
	a->modifiable = false;
	b->modifiable = false;
}

static void visitExpressionVar (AstExpressionVar *node)
{ }

static bool canCoerce (DataType *from, DataType *to)
{
	return coercionMatrix[from->type][to->type];
}

static Token getCoercionToken (void)
{
	return (Token){
		.type = TT_Minus_Greater,
		.lexeme = "->",
		.length = 2,
		.line = 0,
	};
}

static bool coerce (AstExpression *e, DataType *target)
{
	if (!canCoerce(e->dataType, target)) return false;
	if (!dataType_castable(e->dataType, target)) {
		fprintf(stderr, "deverr: attempted to use an invalid cast during coercion");
		analyzer.hadError = true;
	}
	e = ast_initExpressionCast(e, getCoercionToken(), target);
	return true;
}
