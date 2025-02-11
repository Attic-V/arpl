#include <stdio.h>

#include "error.h"
#include "semantic.h"

static void visitAst (Ast *ast);
static void visitRoot (AstRoot *node);

static void visitStatement (AstStatement *node);
static void visitStatementBlock (AstStatementBlock *node);
static void visitStatementExpr (AstStatementExpr *node);
static void visitStatementIfE (AstStatementIfE *node);
static void visitStatementVar (AstStatementVar *node);

static void visitExpression (AstExpression *node);
static void visitExpressionAssign (AstExpressionAssign *node);
static void visitExpressionBinary (AstExpressionBinary *node);
static void visitExpressionBoolean (AstExpressionBoolean *node);
static void visitExpressionNumber (AstExpressionNumber *node);
static void visitExpressionTernary (AstExpressionTernary *node);
static void visitExpressionUnary (AstExpressionUnary *node);
static void visitExpressionVar (AstExpressionVar *node);

typedef struct {
	Table *table;
} Analyzer;

static Analyzer analyzer;

void analyze (Ast *ast)
{
	visitAst(ast);
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
		case AstStatement_Expr: visitStatementExpr(node->as.expr); break;
		case AstStatement_IfE: visitStatementIfE(node->as.ifE); break;
		case AstStatement_Var: visitStatementVar(node->as.var); break;
	}
}

static void visitStatementBlock (AstStatementBlock *node)
{
	analyzer.table = node->table = table_init(256);
	for (AstStatement *stmt = node->children; stmt != NULL; stmt = stmt->next) {
		visitStatement(stmt);
		analyzer.table = node->table;
	}
}

static void visitStatementExpr (AstStatementExpr *node)
{
	visitExpression(node->expression);
}

static void visitStatementIfE (AstStatementIfE *node)
{
	visitExpression(node->condition);
	visitStatement(node->a);
	if (node->b != NULL) {
		visitStatement(node->b);
	}
}

static void visitStatementVar (AstStatementVar *node)
{
	if (!table_add(analyzer.table, symbol_init(node->identifier, node->type))) {
		error(node->identifier, "variable has already been declared in scope");
	}
}

static void visitExpression (AstExpression *node)
{
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
		case AstExpression_Ternary:
			visitExpressionTernary(node->as.ternary);
			node->dataType = node->as.ternary->a->dataType;
			break;
		case AstExpression_Unary:
			switch (node->as.unary->operator.type) {
				case TT_Bang:
					node->dataType = DT_Boolean;
					break;
				case TT_Minus:
				case TT_Tilde:
					node->dataType = DT_Number;
					break;
				default:
			}
			visitExpressionUnary(node->as.unary);
			break;
		case AstExpression_Var:
			Symbol *symbol = table_get(analyzer.table, node->as.var->identifier);
			switch (symbol->type.type) {
				case TT_Int: node->dataType = DT_Number; break;
				default:
			}
			visitExpressionVar(node->as.var);
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
	if (node->a->type != AstExpression_Var) {
		error(node->a->as.var->identifier, "expression must be modifiable");
	}
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
}

static void visitExpressionBoolean (AstExpressionBoolean *node)
{ }

static void visitExpressionNumber (AstExpressionNumber *node)
{ }

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
}

static void visitExpressionUnary (AstExpressionUnary *node)
{
	visitExpression(node->right);
	switch (node->operator.type) {
		case TT_Bang:
			if (node->right->dataType != DT_Boolean) {
				error(node->operator, "operand must be a boolean");
			}
			break;
		case TT_Minus:
		case TT_Tilde:
			if (node->right->dataType != DT_Number) {
				error(node->operator, "operand must be a number");
			}
			break;
		default:
	}
}

static void visitExpressionVar (AstExpressionVar *node)
{ }
