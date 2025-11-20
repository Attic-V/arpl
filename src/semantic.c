#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "err.h"
#include "linked_list.h"
#include "memory.h"
#include "semantic.h"

static void visitAst (Ast *ast);
static void visitRoot (AstRoot *node);

static void visitDeclaration (AstDeclaration *node);
static void visitDeclarationFunction (AstDeclarationFunction *node);
static void visitDeclarationStructD (AstDeclarationStructD *node);

static void visitStatement (AstStatement *node);
static void visitStatementBlock (AstStatementBlock *node);
static void visitStatementExpr (AstStatementExpr *node);
static void visitStatementIfE (AstStatementIfE *node);
static void visitStatementInit (AstStatementInit *node);
static void visitStatementReturnE (AstStatementReturnE *node);
static void visitStatementVar (AstStatementVar *node);

static void visitExpression (AstExpression *node);
static void visitExpressionAccess (AstExpressionAccess *node);
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

static bool coercionMatrix[DataTypeType_Count][DataTypeType_Count] = { };

#define e(token, message) \
	do { \
		analyzer.hadError = true; \
		err(token, message); \
	} while (0)

typedef struct {
	Scope *previousScope;
	Scope *currentScope;
	size_t currentPhysicalIndex;
	bool hadError;
	DataType *caseExpressionType;
	DataType *functionReturnType;
} Analyzer;

static Analyzer analyzer;

static Analyzer getSnapshot (void)
{
	return analyzer;
}

static void restoreSnapshot_caseExpressionType (Analyzer snapshot)
{
	analyzer.caseExpressionType = snapshot.caseExpressionType;
}

void analyze (Ast *ast)
{
	analyzer.previousScope = NULL;
	analyzer.currentScope = NULL;
	analyzer.hadError = false;

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
	Analyzer snapshot = getSnapshot();
	for (AstDeclaration *decl = node->declarations; decl != NULL; decl = decl->next) {
		analyzer.previousScope = node->scope;
		visitDeclaration(decl);
		analyzer.currentScope = node->scope;
		restoreSnapshot_caseExpressionType(snapshot);
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
		e((Token){ .type = TT_EOF }, "missing main");
	}
}

static void visitDeclaration (AstDeclaration *node)
{
	switch (node->type) {
		case AstDeclaration_Function: visitDeclarationFunction(node->as.function); break;
		case AstDeclaration_StructD: visitDeclarationStructD(node->as.structD); break;
	}
}

static void visitDeclarationFunction (AstDeclarationFunction *node)
{
	analyzer.currentPhysicalIndex = 0;
	Symbol *symbol = symbol_init(node->identifier, node->dataType);
	if (scope_add(analyzer.currentScope, symbol)) {
		symbol->physicalIndex = analyzer.currentPhysicalIndex;
		analyzer.currentPhysicalIndex += dataType_getSize(symbol->type);
	} else {
		e(node->identifier, "identifier already exists in scope");
	}
	analyzer.functionReturnType = node->dataType->as.function->returnType;
	analyzer.currentScope = node->scope = scope_init();
	node->scope->parent = analyzer.previousScope;
	for (AstParameter *parameter = node->parameters; parameter != NULL; parameter = parameter->next) {
		analyzer.previousScope = node->scope;
		if (dataType_isStruct(parameter->type)) {
			e(parameter->identifier, "cannot pass structs. please use a pointer to a struct instead");
		}
		AstStatement *paraminit = ast_initStatementVar(parameter->identifier, parameter->type);
		visitStatement(paraminit);
		analyzer.currentScope = node->scope;
	}
	if (dataType_isStruct(node->dataType->as.function->returnType)) {
		e(node->identifier, "cannot return a struct");
	}
	analyzer.previousScope = node->scope;
	visitStatement(node->body);
	analyzer.currentScope = node->scope;
	if (node->scope->parent != NULL) {
		node->scope->parent->physicalSize += node->scope->physicalSize;
	}
}

static void visitDeclarationStructD (AstDeclarationStructD *node)
{
	Symbol *symbol = symbol_init(node->identifier, node->dataType);
	if (scope_add(analyzer.currentScope, symbol)) {
		symbol->physicalIndex = analyzer.currentPhysicalIndex;
		analyzer.currentPhysicalIndex += dataType_getSize(symbol->type);
	} else {
		e(node->identifier, "identifier has already been defined in scope");
	}
	analyzer.currentScope = node->scope = scope_init();
	analyzer.currentPhysicalIndex = 0;
	node->scope->parent = analyzer.previousScope;
	for (AstParameter *member = node->members; member != NULL; member = member->next) {
		analyzer.previousScope = node->scope;
		AstStatement *memberinit = ast_initStatementVar(member->identifier, member->type);
		visitStatement(memberinit);
		analyzer.currentScope = node->scope;
	}
	if (node->scope->parent != NULL) {
		node->scope->parent->physicalSize += node->scope->physicalSize;
	}
}

static void visitStatement (AstStatement *node)
{
	switch (node->type) {
		case AstStatement_Block: visitStatementBlock(node->as.block); break;
		case AstStatement_Expr: visitStatementExpr(node->as.expr); break;
		case AstStatement_IfE: visitStatementIfE(node->as.ifE); break;
		case AstStatement_Init: visitStatementInit(node->as.init); break;
		case AstStatement_ReturnE: visitStatementReturnE(node->as.returnE); break;
		case AstStatement_Var: visitStatementVar(node->as.var); break;
	}
}

static void visitStatementBlock (AstStatementBlock *node)
{
	analyzer.currentScope = node->scope = scope_init();
	node->scope->parent = analyzer.previousScope;
	Analyzer snapshot = getSnapshot();
	for (AstStatement *stmt = node->children; stmt != NULL; stmt = stmt->next) {
		analyzer.previousScope = node->scope;
		visitStatement(stmt);
		analyzer.currentScope = node->scope;
		restoreSnapshot_caseExpressionType(snapshot);
	}
	if (node->scope->parent != NULL) {
		node->scope->parent->physicalSize += node->scope->physicalSize;
	}
}

static void visitStatementExpr (AstStatementExpr *node)
{
	visitExpression(node->expression);
	node->expression->modifiable = false;
}

static void visitStatementIfE (AstStatementIfE *node)
{
	visitExpression(node->condition);
	if (!dataType_isBoolean(node->condition->dataType)) {
		e(node->keyword, "condition must be a boolean");
	}
	visitStatement(node->a);
	if (node->b != NULL) {
		visitStatement(node->b);
	}
}

static void visitStatementInit (AstStatementInit *node)
{
	Symbol *symbol = symbol_init(node->identifier, node->type);
	if (scope_add(analyzer.currentScope, symbol)) {
		symbol->physicalIndex = analyzer.currentPhysicalIndex;
		analyzer.currentPhysicalIndex += dataType_getSize(symbol->type);
	} else {
		e(node->identifier, "variable has already been declared in scope");
	}

	visitExpression(node->expression);

	if (!dataType_equal(symbol->type, node->expression->dataType)) {
		if (!coerce(node->expression, symbol->type)) {
			e(node->operator, "operands must have the same type");
		}
	}
	node->expression->modifiable = false;
}

static void visitStatementReturnE (AstStatementReturnE *node)
{
	if (node->expression != NULL) {
		visitExpression(node->expression);
		node->expression->modifiable = false;
		if (!dataType_equal(node->expression->dataType, analyzer.functionReturnType)) {
			if (!coerce(node->expression, analyzer.functionReturnType)) {
				e(node->keyword, "type of expression in return does not match function return type");
			}
		}
	}
}

static void visitStatementVar (AstStatementVar *node)
{
	Symbol *symbol = symbol_init(node->identifier, node->type);
	if (dataType_isStruct(symbol->type)) {
		Symbol *s = telescope_get(analyzer.currentScope, symbol->type->as.struct_->identifier);
		if (s == NULL) {
			e(symbol->type->as.struct_->identifier, "unrecognized type");
			exit(1);
		}
		symbol->type->as.struct_->members = s->type->as.struct_->members;
	}
	if (scope_add(analyzer.currentScope, symbol)) {
		symbol->physicalIndex = analyzer.currentPhysicalIndex;
		analyzer.currentPhysicalIndex += dataType_getSize(symbol->type);
	} else {
		e(node->identifier, "variable has already been declared in scope");
	}
}

static void visitExpression (AstExpression *node)
{
	switch (node->type) {
		case AstExpression_Access:
			visitExpressionAccess(node->as.access);
			node->dataType = node->as.access->mDataType;
			node->modifiable = true;
			break;
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
				default:;
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
			visitExpressionPostfix(node->as.postfix);
			switch (node->as.postfix->operator.type) {
				case TT_Plus_Plus:
					node->dataType = node->as.postfix->e->dataType;
					break;
				default:;
			}
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
				default:;
			}
			break;
		case AstExpression_Ternary:
			visitExpressionTernary(node->as.ternary);
			node->dataType = node->as.ternary->a->dataType;
			break;
		case AstExpression_Var: {
			Symbol *symbol = telescope_get(analyzer.currentScope, node->as.var->identifier);
			if (symbol == NULL) {
				e(node->as.var->identifier, "undeclared identifier");
				exit(1);
				break;
			}
			node->dataType = symbol->type;
			visitExpressionVar(node->as.var);
			node->modifiable = true;
			break;
		}
	}
}

static void visitExpressionAccess (AstExpressionAccess *node)
{
	visitExpression(node->e);
	Symbol *s = NULL;
	switch (node->op.type) {
		case TT_Dot:
			if (!dataType_isStruct(node->e->dataType)) {
				e(node->op, "operand must be of type struct");
				exit(1);
			}
			if (!node->e->modifiable) {
				e(node->op, "operand must be modifiable");
			}
			s = telescope_get(analyzer.currentScope, node->e->dataType->as.struct_->identifier);
			break;
		case TT_Dot_Dot:
			if (!dataType_isPointer(node->e->dataType) || !dataType_isStruct(node->e->dataType->as.pointer->to)) {
				e(node->op, "operand must be a pointer to a struct");
				exit(1);
			}
			if (!node->e->modifiable) {
				e(node->op, "operand must be modifiable");
			}
			s = telescope_get(analyzer.currentScope, node->e->dataType->as.pointer->to->as.struct_->identifier);
			break;
		default:;
	}
	node->mDataType = NULL;
	for (DataTypeMember *member = s->type->as.struct_->members; member != NULL; member = member->next) {
		if (token_equal(node->mToken, member->identifier)) {
			node->mDataType = member->dataType;
			break;
		}
	}
	if (node->mDataType == NULL) {
		e(node->mToken, "unrecognized member");
		exit(1);
	}
}

static void visitExpressionAssign (AstExpressionAssign *node)
{
	visitExpression(node->a);
	visitExpression(node->b);
	if (!dataType_equal(node->a->dataType, node->b->dataType)) {
		if (!coerce(node->b, node->a->dataType)) {
			e(node->operator, "operands must have the same type");
		}
	}
	if (!node->a->modifiable) {
		e(node->operator, "assignee must be modifiable");
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
				e(node->operator, "operands must be numbers");
			}
			break;
		case TT_And_And:
		case TT_Pipe_Pipe:
			if (!dataType_isBoolean(node->a->dataType) || !dataType_isBoolean(node->b->dataType)) {
				e(node->operator, "operands must be booleans");
			}
			break;
		case TT_Bang_Equal:
		case TT_Equal_Equal:
			break;
		default:;
	}
	if (!dataType_equal(node->a->dataType, node->b->dataType)) {
		if (!coerce(node->a, node->b->dataType) && !coerce(node->b, node->a->dataType)) {
			e(node->operator, "operands must have the same type");
		}
	}
	node->a->modifiable = false;
	node->b->modifiable = false;
}

static void visitExpressionBoolean (AstExpressionBoolean *node)
{
	(void)node;
}

static void visitExpressionCall (AstExpressionCall *node)
{
	visitExpression(node->e);
	if (!dataType_isFunction(node->e->dataType)) {
		e(node->lparen, "cannot call non-function");
		exit(1);
	}
	if (node->e->type != AstExpression_Var) {
		e(node->lparen, "cannot perform call here");
		return;
	}
	Symbol *fn = telescope_get(analyzer.currentScope, node->e->as.var->identifier);
	DataType *parameter = fn->type->as.function->parameters;
	AstArgument *argument = node->arguments;
	for (; argument != NULL && parameter != NULL; argument = argument->next, parameter = parameter->next) {
		visitExpression(argument->expression);
		if (!dataType_equal(argument->expression->dataType, parameter)) {
			if (!coerce(argument->expression, parameter)) {
				e(node->lparen, "call does not match function signature");
			}
		}
	}
	if (argument != NULL || parameter != NULL) {
		e(node->lparen, "call does not match function signature");
	}
}

static void visitExpressionCast (AstExpressionCast *node)
{
	visitExpression(node->e);
	if (!dataType_castable(node->e->dataType, node->to)) {
		e(node->operator, "invalid cast");
	}
	node->e->modifiable = false;
}

static void visitExpressionNumber (AstExpressionNumber *node)
{
	(void)node;
}

static void visitExpressionPostfix (AstExpressionPostfix *node)
{
	visitExpression(node->e);
	switch (node->operator.type) {
		case TT_Minus_Minus:
		case TT_Plus_Plus:
			if (!dataType_isInt(node->e->dataType)) {
				e(node->operator, "operand must be a number");
			}
			if (!node->e->modifiable) {
				e(node->e->as.var->identifier, "expression must be modifiable");
			}
			break;
		default:;
	}
}

static void visitExpressionPrefix (AstExpressionPrefix *node)
{
	visitExpression(node->e);
	switch (node->operator.type) {
		case TT_Bang:
			if (!dataType_isBoolean(node->e->dataType)) {
				e(node->operator, "operand must be a boolean");
			}
			node->e->modifiable = false;
			break;
		case TT_Minus:
		case TT_Tilde:
			if (!dataType_isInt(node->e->dataType)) {
				e(node->operator, "operand must be a number");
			}
			node->e->modifiable = false;
			break;
		case TT_Minus_Minus:
		case TT_Plus_Plus:
			if (!dataType_isInt(node->e->dataType)) {
				e(node->operator, "operand must be a number");
			}
			if (!node->e->modifiable) {
				e(node->e->as.var->identifier, "expression must be modifiable");
			}
			break;
		case TT_And:
			if (!node->e->modifiable) {
				e(node->operator, "operand must be modifiable");
			}
			break;
		case TT_Star:
			if (!dataType_isPointer(node->e->dataType)) {
				e(node->operator, "operand must be a pointer");
				exit(1);
			}
			break;
		default:;
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
		e(node->operator, "condition must be a boolean");
	}
	if (!dataType_equal(a->dataType, b->dataType)) {
		if (!coerce(node->a, node->b->dataType) && !coerce(node->b, node->a->dataType)) {
			e(node->operator, "operands must have the same type");
		}
	}
	c->modifiable = false;
	a->modifiable = false;
	b->modifiable = false;
}

static void visitExpressionVar (AstExpressionVar *node)
{
	(void)node;
}

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
		exit(1);
	}
	e = ast_initExpressionCast(e, getCoercionToken(), target);
	return true;
}
