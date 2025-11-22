#include "ast.h"
#include "linked_list.h"
#include "memory.h"

static AstDeclaration *ast_initDeclaration (AstDeclarationType type);
static AstStatement *ast_initStatement (AstStatementType type);
static AstExpression *ast_initExpression (AstExpressionType type);

static AstDeclarationFunction *astDeclaration_initFunction (Token keyword, Token identifier, AstStatement *body, DataType *returnType, AstParameter *parameters);

static AstStatementBlock *astStatement_initBlock (AstStatement *children);
static AstStatementExpr *astStatement_initExpr (AstExpression *expression);
static AstStatementInit *astStatement_initInit (Token identifier, DataType *type, AstExpression *expression, Token operator);
static AstStatementReturnE *astStatement_initReturnE (Token keyword, AstExpression *expression);
static AstStatementVar *astStatement_initVar (Token identifier, DataType *type);

static AstExpressionAssign *astExpression_initAssign (AstExpression *a, AstExpression *b, Token operator);
static AstExpressionNumber *astExpression_initNumber (Token value);
static AstExpressionVar *astExpression_initVar (Token identifier);

Ast *ast_init (AstRoot *root)
{
	Ast *ast = mem_alloc(sizeof(*ast));
	ast->root = root;
	return ast;
}

AstRoot *ast_initRoot (AstDeclaration *declarations)
{
	AstRoot *root = mem_alloc(sizeof(*root));
	root->declarations = declarations;
	return root;
}

AstDeclaration *ast_initDeclaration (AstDeclarationType type)
{
	AstDeclaration *d = mem_alloc(sizeof(*d));
	d->type = type;
	dll_init(d);
	return d;
}

AstStatement *ast_initStatement (AstStatementType type)
{
	AstStatement *s = mem_alloc(sizeof(*s));
	s->type = type;
	dll_init(s);
	return s;
}

AstExpression *ast_initExpression (AstExpressionType type)
{
	AstExpression *e = mem_alloc(sizeof(*e));
	e->type = type;
	e->dataType = mem_alloc(sizeof(*e->dataType));
	e->modifiable = false;
	return e;
}

AstDeclaration *ast_initDeclarationFunction (Token keyword, Token identifier, AstStatement *body, DataType *returnType, AstParameter *parameters)
{
	AstDeclaration *declaration = ast_initDeclaration(AstDeclaration_Function);
	declaration->as.function = astDeclaration_initFunction(keyword, identifier, body, returnType, parameters);
	return declaration;
}

AstStatement *ast_initStatementBlock (AstStatement *children)
{
	AstStatement *statement = ast_initStatement(AstStatement_Block);
	statement->as.block = astStatement_initBlock(children);
	return statement;
}

AstStatement *ast_initStatementExpr (AstExpression *expression)
{
	AstStatement *statement = ast_initStatement(AstStatement_Expr);
	statement->as.expr = astStatement_initExpr(expression);
	return statement;
}

AstStatement *ast_initStatementInit (Token identifier, DataType *type, AstExpression *expression, Token operator)
{
	AstStatement *statement = ast_initStatement(AstStatement_Init);
	statement->as.init = astStatement_initInit(identifier, type, expression, operator);
	return statement;
}

AstStatement *ast_initStatementReturnE (Token keyword, AstExpression *expression)
{
	AstStatement *statement = ast_initStatement(AstStatement_ReturnE);
	statement->as.returnE = astStatement_initReturnE(keyword, expression);
	return statement;
}

AstStatement *ast_initStatementVar (Token identifier, DataType *type)
{
	AstStatement *statement = ast_initStatement(AstStatement_Var);
	statement->as.var = astStatement_initVar(identifier, type);
	return statement;
}

AstExpression *ast_initExpressionAssign (AstExpression *a, AstExpression *b, Token operator)
{
	AstExpression *expression = ast_initExpression(AstExpression_Assign);
	expression->as.assign = astExpression_initAssign(a, b, operator);
	return expression;
}

AstExpression *ast_initExpressionNumber (Token value)
{
	AstExpression *expression = ast_initExpression(AstExpression_Number);
	expression->as.number = astExpression_initNumber(value);
	return expression;
}

AstExpression *ast_initExpressionVar (Token identifier)
{
	AstExpression *expression = ast_initExpression(AstExpression_Var);
	expression->as.var = astExpression_initVar(identifier);
	return expression;
}

AstArgument *ast_initArgument (AstExpression *expression)
{
	AstArgument *argument = mem_alloc(sizeof(*argument));
	argument->expression = expression;
	dll_init(argument);
	return argument;
}

AstParameter *ast_initParameter (Token identifier, DataType *type)
{
	AstParameter *parameter = mem_alloc(sizeof(*parameter));
	parameter->identifier = identifier;
	parameter->type = type;
	dll_init(parameter);
	return parameter;
}

static AstDeclarationFunction *astDeclaration_initFunction (Token keyword, Token identifier, AstStatement *body, DataType *returnType, AstParameter *parameters)
{
	AstDeclarationFunction *function = mem_alloc(sizeof(*function));
	function->keyword = keyword;
	function->identifier = identifier;
	function->body = body;
	DataType *params = NULL;
	for (AstParameter *parameter = parameters; parameter != NULL; parameter = parameter->next) {
		dll_shove(params, parameter->type);
	}
	dll_rewind(params);
	function->dataType = dataType_initFunction(returnType, params);
	function->parameters = parameters;
	return function;
}

static AstStatementBlock *astStatement_initBlock (AstStatement *children)
{
	AstStatementBlock *block = mem_alloc(sizeof(*block));
	block->children = children;
	return block;
}

static AstStatementExpr *astStatement_initExpr (AstExpression *expression)
{
	AstStatementExpr *expr = mem_alloc(sizeof(*expr));
	expr->expression = expression;
	return expr;
}

static AstStatementInit *astStatement_initInit (Token identifier, DataType *type, AstExpression *expression, Token operator)
{
	AstStatementInit *init = mem_alloc(sizeof(*init));
	init->identifier = identifier;
	init->type = type;
	init->expression = expression;
	init->operator = operator;
	return init;
}

static AstStatementReturnE *astStatement_initReturnE (Token keyword, AstExpression *expression)
{
	AstStatementReturnE *returnE = mem_alloc(sizeof(*returnE));
	returnE->keyword = keyword;
	returnE->expression = expression;
	return returnE;
}

static AstStatementVar *astStatement_initVar (Token identifier, DataType *type)
{
	AstStatementVar *var = mem_alloc(sizeof(*var));
	var->identifier = identifier;
	var->type = type;
	return var;
}

static AstExpressionAssign *astExpression_initAssign (AstExpression *a, AstExpression *b, Token operator)
{
	AstExpressionAssign *assign = mem_alloc(sizeof(*assign));
	assign->a = a;
	assign->b = b;
	assign->operator = operator;
	return assign;
}

static AstExpressionNumber *astExpression_initNumber (Token value)
{
	AstExpressionNumber *number = mem_alloc(sizeof(*number));
	number->value = value;
	return number;
}

static AstExpressionVar *astExpression_initVar (Token identifier)
{
	AstExpressionVar *var = mem_alloc(sizeof(*var));
	var->identifier = identifier;
	return var;
}
