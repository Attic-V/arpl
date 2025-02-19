#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "linked_list.h"
#include "semantic.h"

static void visitAst (Ast *ast);
static void visitRoot (AstRoot *node);

static void visitStatement (AstStatement *node);
static void visitStatementBlock (AstStatementBlock *node);
static void visitStatementDoWhile (AstStatementDoWhile *node);
static void visitStatementExpr (AstStatementExpr *node);
static void visitStatementIfE (AstStatementIfE *node);
static void visitStatementInit (AstStatementInit *node);
static void visitStatementVar (AstStatementVar *node);
static void visitStatementWhileC (AstStatementWhileC *node);

static void visitExpression (AstExpression *node);
static void visitExpressionAssign (AstExpressionAssign *node);
static void visitExpressionBinary (AstExpressionBinary *node);
static void visitExpressionBoolean (AstExpressionBoolean *node);
static void visitExpressionNumber (AstExpressionNumber *node);
static void visitExpressionPostfix (AstExpressionPostfix *node);
static void visitExpressionPrefix (AstExpressionPrefix *node);
static void visitExpressionTernary (AstExpressionTernary *node);
static void visitExpressionVar (AstExpressionVar *node);

typedef struct {
	Scope *previousScope;
	Scope *currentScope;
	size_t currentPhysicalIndex;
	bool hadError;
} Analyzer;

static Analyzer analyzer;

void analyze (Ast *ast)
{
	analyzer.previousScope = NULL;
	analyzer.currentScope = NULL;
	analyzer.currentPhysicalIndex = 0;
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
	visitStatement(node->statement);
}

static void visitStatement (AstStatement *node)
{
	switch (node->type) {
		case AstStatement_Block: visitStatementBlock(node->as.block); break;
		case AstStatement_DoWhile: visitStatementDoWhile(node->as.doWhile); break;
		case AstStatement_Expr: visitStatementExpr(node->as.expr); break;
		case AstStatement_IfE: visitStatementIfE(node->as.ifE); break;
		case AstStatement_Init: visitStatementInit(node->as.init); break;
		case AstStatement_Var: visitStatementVar(node->as.var); break;
		case AstStatement_WhileC: visitStatementWhileC(node->as.whileC); break;
	}
}

static void visitStatementBlock (AstStatementBlock *node)
{
	analyzer.currentScope = node->scope = scope_init();
	node->scope->parent = analyzer.previousScope;
	for (AstStatement *stmt = node->children; stmt != NULL; stmt = stmt->next) {
		analyzer.previousScope = node->scope;
		visitStatement(stmt);
		analyzer.currentScope = node->scope;
	}
	if (node->scope->parent != NULL) {
		node->scope->parent->physicalSize += node->scope->physicalSize;
	}
}

static void visitStatementDoWhile (AstStatementDoWhile *node)
{
	visitStatement(node->a);
	visitExpression(node->condition);
	if (node->condition->dataType != DT_Boolean) {
		error(node->keyword, "condition must be a boolean");
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
	if (node->condition->dataType != DT_Boolean) {
		error(node->keyword, "condition must be a boolean");
	}
	visitStatement(node->a);
	if (node->b != NULL) {
		visitStatement(node->b);
	}
}

static void visitStatementInit (AstStatementInit *node)
{
	visitStatement(ast_initStatementVar(node->identifier, node->type));
	visitExpression(ast_initExpressionAssign(ast_initExpressionVar(node->identifier), node->expression, node->operator));
}

static void visitStatementVar (AstStatementVar *node)
{
	Symbol *symbol = symbol_init(node->identifier, getTypeFromKey(node->type));
	if (scope_add(analyzer.currentScope, symbol)) {
		symbol->physicalIndex = analyzer.currentPhysicalIndex;
		analyzer.currentPhysicalIndex += getDtSize(symbol->type);
	} else {
		error(node->identifier, "variable has already been declared in scope");
	}
}

static void visitStatementWhileC (AstStatementWhileC *node)
{
	visitExpression(node->condition);
	if (node->condition->dataType != DT_Boolean) {
		error(node->keyword, "condition must be a boolean");
	}
	if (node->a != NULL) {
		visitStatement(node->a);
	}
}

static void visitExpression (AstExpression *node)
{
	node->modifiable = false;
	switch (node->type) {
		case AstExpression_Assign:
			visitExpressionAssign(node->as.assign);
			node->dataType = node->as.assign->b->dataType;
			break;
		case AstExpression_Binary:
			switch (node->as.binary->operator.type) {
				case TT_And:
				case TT_Caret:
				case TT_Greater_Greater:
				case TT_Less_Less:
				case TT_Pipe:
				case TT_Plus:
				case TT_Minus:
				case TT_Star:
					node->dataType = DT_Number;
					break;
				case TT_And_And:
				case TT_Bang_Equal:
				case TT_Equal_Equal:
				case TT_Less:
				case TT_Pipe_Pipe:
					node->dataType = DT_Boolean;
					break;
				default:
			}
			visitExpressionBinary(node->as.binary);
			break;
		case AstExpression_Boolean:
			node->dataType = DT_Boolean;
			visitExpressionBoolean(node->as.boolean);
			break;
		case AstExpression_Number:
			node->dataType = DT_Number;
			visitExpressionNumber(node->as.number);
			break;
		case AstExpression_Postfix:
			switch (node->as.postfix->operator.type) {
				case TT_Plus_Plus:
					node->dataType = DT_Number;
					break;
				default:
			}
			visitExpressionPostfix(node->as.postfix);
			break;
		case AstExpression_Prefix:
			switch (node->as.prefix->operator.type) {
				case TT_Bang:
					node->dataType = DT_Boolean;
					break;
				case TT_Minus:
				case TT_Plus_Plus:
				case TT_Tilde:
					node->dataType = DT_Number;
					break;
				default:
			}
			visitExpressionPrefix(node->as.prefix);
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
			switch (symbol->type) {
				case DT_Number: node->dataType = DT_Number; break;
				default:
			}
			visitExpressionVar(node->as.var);
			node->modifiable = true;
			break;
	}
}

static void visitExpressionAssign (AstExpressionAssign *node)
{
	visitExpression(node->a);
	visitExpression(node->b);
	if (node->a->dataType != node->b->dataType) {
		error(node->operator, "operands must have the same type");
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
			if (node->a->dataType != DT_Number || node->b->dataType != DT_Number) {
				error(node->operator, "operand must be a number");
			}
			break;
		case TT_And_And:
		case TT_Pipe_Pipe:
			if (node->a->dataType != DT_Boolean || node->b->dataType != DT_Boolean) {
				error(node->operator, "operand must be a boolean");
			}
			break;
		case TT_Bang_Equal:
		case TT_Equal_Equal:
			break;
		default:
	}
	if (node->a->dataType != node->b->dataType) {
		error(node->operator, "operands must have the same type");
	}
	node->a->modifiable = false;
	node->b->modifiable = false;
}

static void visitExpressionBoolean (AstExpressionBoolean *node)
{ }

static void visitExpressionNumber (AstExpressionNumber *node)
{ }

static void visitExpressionPostfix (AstExpressionPostfix *node)
{
	visitExpression(node->e);
	switch (node->operator.type) {
		case TT_Minus_Minus:
		case TT_Plus_Plus:
			if (node->e->dataType != DT_Number) {
				error(node->operator, "operand must be a number");
			}
			if (!node->e->modifiable) {
				error(node->e->as.var->identifier, "expression must be modifiable");
				analyzer.hadError = true;
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
			if (node->e->dataType != DT_Boolean) {
				error(node->operator, "operand must be a boolean");
			}
			break;
		case TT_Minus:
		case TT_Tilde:
			if (node->e->dataType != DT_Number) {
				error(node->operator, "operand must be a number");
			}
			break;
		case TT_Minus_Minus:
		case TT_Plus_Plus:
			if (node->e->dataType != DT_Number) {
				error(node->operator, "operand must be a number");
			}
			if (!node->e->modifiable) {
				error(node->e->as.var->identifier, "expression must be modifiable");
				analyzer.hadError = true;
			}
			break;
		default:
	}
}

static void visitExpressionTernary (AstExpressionTernary *node)
{
	visitExpression(node->condition);
	visitExpression(node->a);
	visitExpression(node->b);
	if (node->condition->dataType != DT_Boolean) {
		error(node->operator, "condition must be a boolean");
	}
	if (node->a->dataType != node->b->dataType) {
		error(node->operator, "operands must have the same type");
	}
	node->condition->modifiable = false;
	node->a->modifiable = false;
	node->b->modifiable = false;
}

static void visitExpressionVar (AstExpressionVar *node)
{ }
