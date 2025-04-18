#include "ast.h"
#include "linked_list.h"
#include "memory.h"

static AstDeclaration *ast_initDeclaration (AstDeclarationType type);
static AstStatement *ast_initStatement (AstStatementType type);
static AstExpression *ast_initExpression (AstExpressionType type);

static AstDeclarationFunction *astDeclaration_initFunction (Token keyword, Token identifier, AstStatement *body);

static AstStatementBlock *astStatement_initBlock (AstStatement *children);
static AstStatementBreakL *astStatement_initBreakL (Token keyword);
static AstStatementCaseL *astStatement_initCaseL (AstExpression *e, AstStatement *body, Token keyword);
static AstStatementContinueL *astStatement_initContinueL (Token keyword);
static AstStatementDoWhile *astStatement_initDoWhile (AstStatement *a, AstExpression *condition, Token operator);
static AstStatementExpr *astStatement_initExpr (AstExpression *expression);
static AstStatementForI *astStatement_initForI (AstStatement *init, AstExpression *condition, AstExpression *update, AstStatement *body, Token keyword);
static AstStatementIfE *astStatement_initIfE (AstExpression *condition, AstStatement *a, AstStatement *b, Token keyword);
static AstStatementInit *astStatement_initInit (Token identifier, DataType *type, AstExpression *expression, Token operator);
static AstStatementReturnE *astStatement_initReturnE (AstExpression *expression);
static AstStatementSwitchC *astStatement_initSwitchC (AstExpression *e, AstStatement *body);
static AstStatementVar *astStatement_initVar (Token identifier, DataType *type);
static AstStatementWhileC *astStatement_initWhileC (AstExpression *condition, AstStatement *a, Token keyword);

static AstExpressionAssign *astExpression_initAssign (AstExpression *a, AstExpression *b, Token operator);
static AstExpressionBinary *astExpression_initBinary (AstExpression *a, AstExpression *b, Token operator);
static AstExpressionCast *astExpression_initCast (AstExpression *e, Token operator, DataType *to);
static AstExpressionBoolean *astExpression_initBoolean (bool value);
static AstExpressionNumber *astExpression_initNumber (Token value);
static AstExpressionPostfix *astExpression_initPostfix (Token operator, AstExpression *e);
static AstExpressionPrefix *astExpression_initPrefix (Token operator, AstExpression *e);
static AstExpressionTernary *astExpression_initTernary (AstExpression *condition, AstExpression *a, AstExpression *b, Token operator);
static AstExpressionVar *astExpression_initVar (Token identifier);

Ast *ast_init (AstRoot *root)
{
	Ast *ast = mem_alloc(sizeof(*ast));
	ast->root = root;
	return ast;
}

AstRoot *ast_initRoot (AstDeclaration *declaration)
{
	AstRoot *root = mem_alloc(sizeof(*root));
	root->declaration = declaration;
	return root;
}

AstDeclaration *ast_initDeclaration (AstDeclarationType type)
{
	AstDeclaration *d = mem_alloc(sizeof(*d));
	d->type = type;
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

AstDeclaration *ast_initDeclarationFunction (Token keyword, Token identifier, AstStatement *body)
{
	AstDeclaration *declaration = ast_initDeclaration(AstDeclaration_Function);
	declaration->as.function = astDeclaration_initFunction(keyword, identifier, body);
	return declaration;
}

AstStatement *ast_initStatementBlock (AstStatement *children)
{
	AstStatement *statement = ast_initStatement(AstStatement_Block);
	statement->as.block = astStatement_initBlock(children);
	return statement;
}

AstStatement *ast_initStatementBreakL (Token keyword)
{
	AstStatement *statement = ast_initStatement(AstStatement_BreakL);
	statement->as.breakL = astStatement_initBreakL(keyword);
	return statement;
}

AstStatement *ast_initStatementCaseL (AstExpression *e, AstStatement *body, Token keyword)
{
	AstStatement *statement = ast_initStatement(AstStatement_CaseL);
	statement->as.caseL = astStatement_initCaseL(e, body, keyword);
	return statement;
}

AstStatement *ast_initStatementContinueL (Token keyword)
{
	AstStatement *statement = ast_initStatement(AstStatement_ContinueL);
	statement->as.continueL = astStatement_initContinueL(keyword);
	return statement;
}

AstStatement *ast_initStatementDoWhile (AstStatement *a, AstExpression *condition, Token keyword)
{
	AstStatement *statement = ast_initStatement(AstStatement_DoWhile);
	statement->as.doWhile = astStatement_initDoWhile(a, condition, keyword);
	return statement;
}

AstStatement *ast_initStatementExpr (AstExpression *expression)
{
	AstStatement *statement = ast_initStatement(AstStatement_Expr);
	statement->as.expr = astStatement_initExpr(expression);
	return statement;
}

AstStatement *ast_initStatementForI (AstStatement *init, AstExpression *condition, AstExpression *update, AstStatement *body, Token keyword)
{
	AstStatement *statement = ast_initStatement(AstStatement_ForI);
	statement->as.forI = astStatement_initForI(init, condition, update, body, keyword);
	return statement;
}

AstStatement *ast_initStatementIfE (AstExpression *condition, AstStatement *a, AstStatement *b, Token keyword)
{
	AstStatement *statement = ast_initStatement(AstStatement_IfE);
	statement->as.ifE = astStatement_initIfE(condition, a, b, keyword);
	return statement;
}

AstStatement *ast_initStatementInit (Token identifier, DataType *type, AstExpression *expression, Token operator)
{
	AstStatement *statement = ast_initStatement(AstStatement_Init);
	statement->as.init = astStatement_initInit(identifier, type, expression, operator);
	return statement;
}

AstStatement *ast_initStatementReturnE (AstExpression *expression)
{
	AstStatement *statement = ast_initStatement(AstStatement_ReturnE);
	statement->as.returnE = astStatement_initReturnE(expression);
	return statement;
}

AstStatement *ast_initStatementSwitchC (AstExpression *e, AstStatement *body)
{
	AstStatement *statement = ast_initStatement(AstStatement_SwitchC);
	statement->as.switchC = astStatement_initSwitchC(e, body);
	return statement;
}

AstStatement *ast_initStatementVar (Token identifier, DataType *type)
{
	AstStatement *statement = ast_initStatement(AstStatement_Var);
	statement->as.var = astStatement_initVar(identifier, type);
	return statement;
}

AstStatement *ast_initStatementWhileC (AstExpression *condition, AstStatement *a, Token keyword)
{
	AstStatement *statement = mem_alloc(sizeof(*statement));
	statement->type = AstStatement_WhileC;
	statement->as.whileC = astStatement_initWhileC(condition, a, keyword);
	return statement;
}

AstExpression *ast_initExpressionAssign (AstExpression *a, AstExpression *b, Token operator)
{
	AstExpression *expression = ast_initExpression(AstExpression_Assign);
	expression->as.assign = astExpression_initAssign(a, b, operator);
	return expression;
}

AstExpression *ast_initExpressionBinary (AstExpression *a, AstExpression *b, Token operator)
{
	AstExpression *expression = ast_initExpression(AstExpression_Binary);
	expression->as.binary = astExpression_initBinary(a, b, operator);
	return expression;
}

AstExpression *ast_initExpressionBoolean (bool value)
{
	AstExpression *expression = ast_initExpression(AstExpression_Boolean);
	expression->as.boolean = astExpression_initBoolean(value);
	return expression;
}

AstExpression *ast_initExpressionCast (AstExpression *e, Token operator, DataType *to)
{
	AstExpression *expression = ast_initExpression(AstExpression_Cast);
	expression->as.cast = astExpression_initCast(e, operator, to);
	return expression;
}

AstExpression *ast_initExpressionNumber (Token value)
{
	AstExpression *expression = ast_initExpression(AstExpression_Number);
	expression->as.number = astExpression_initNumber(value);
	return expression;
}

AstExpression *ast_initExpressionPostfix (Token operator, AstExpression *e)
{
	AstExpression *expression = ast_initExpression(AstExpression_Postfix);
	expression->as.postfix = astExpression_initPostfix(operator, e);
	return expression;
}

AstExpression *ast_initExpressionPrefix (Token operator, AstExpression *e)
{
	AstExpression *expression = ast_initExpression(AstExpression_Prefix);
	expression->as.prefix = astExpression_initPrefix(operator, e);
	return expression;
}

AstExpression *ast_initExpressionTernary (AstExpression *condition, AstExpression *a, AstExpression *b, Token operator)
{
	AstExpression *expression = ast_initExpression(AstExpression_Ternary);
	expression->as.ternary = astExpression_initTernary(condition, a, b, operator);
	return expression;
}

AstExpression *ast_initExpressionVar (Token identifier)
{
	AstExpression *expression = ast_initExpression(AstExpression_Var);
	expression->as.var = astExpression_initVar(identifier);
	return expression;
}

static AstDeclarationFunction *astDeclaration_initFunction (Token keyword, Token identifier, AstStatement *body)
{
	AstDeclarationFunction *function = mem_alloc(sizeof(*function));
	function->keyword = keyword;
	function->identifier = identifier;
	function->body = body;
	function->dataType = dataType_initFunction();
	return function;
}

static AstStatementBlock *astStatement_initBlock (AstStatement *children)
{
	AstStatementBlock *block = mem_alloc(sizeof(*block));
	block->children = children;
	return block;
}

static AstStatementBreakL *astStatement_initBreakL (Token keyword)
{
	AstStatementBreakL *breakL = mem_alloc(sizeof(*breakL));
	breakL->keyword = keyword;
	return breakL;
}

static AstStatementCaseL *astStatement_initCaseL (AstExpression *e, AstStatement *body, Token keyword)
{
	AstStatementCaseL *caseL = mem_alloc(sizeof(*caseL));
	caseL->e = e;
	caseL->body = body;
	caseL->keyword = keyword;
	return caseL;
}

static AstStatementContinueL *astStatement_initContinueL (Token keyword)
{
	AstStatementContinueL *continueL = mem_alloc(sizeof(*continueL));
	continueL->keyword = keyword;
	return continueL;
}

static AstStatementDoWhile *astStatement_initDoWhile (AstStatement *a, AstExpression *condition, Token keyword)
{
	AstStatementDoWhile *doWhile = mem_alloc(sizeof(*doWhile));
	doWhile->a = a;
	doWhile->condition = condition;
	doWhile->keyword = keyword;
	return doWhile;
}

static AstStatementExpr *astStatement_initExpr (AstExpression *expression)
{
	AstStatementExpr *expr = mem_alloc(sizeof(*expr));
	expr->expression = expression;
	return expr;
}

static AstStatementForI *astStatement_initForI (AstStatement *init, AstExpression *condition, AstExpression *update, AstStatement *body, Token keyword)
{
	AstStatementForI *forI = mem_alloc(sizeof(*forI));
	forI->init = init;
	forI->condition = condition;
	forI->update = update;
	forI->body = body;
	forI->keyword = keyword;
	return forI;
}

static AstStatementIfE *astStatement_initIfE (AstExpression *condition, AstStatement *a, AstStatement *b, Token keyword)
{
	AstStatementIfE *ifE = mem_alloc(sizeof(*ifE));
	ifE->condition = condition;
	ifE->a = a;
	ifE->b = b;
	ifE->keyword = keyword;
	return ifE;
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

static AstStatementReturnE *astStatement_initReturnE (AstExpression *expression)
{
	AstStatementReturnE *returnE = mem_alloc(sizeof(*returnE));
	returnE->expression = expression;
	return returnE;
}

static AstStatementSwitchC *astStatement_initSwitchC (AstExpression *e, AstStatement *body)
{
	AstStatementSwitchC *switchC = mem_alloc(sizeof(*switchC));
	switchC->e = e;
	switchC->body = body;
	return switchC;
}

static AstStatementVar *astStatement_initVar (Token identifier, DataType *type)
{
	AstStatementVar *var = mem_alloc(sizeof(*var));
	var->identifier = identifier;
	var->type = type;
	return var;
}

static AstStatementWhileC *astStatement_initWhileC (AstExpression *condition, AstStatement *a, Token keyword)
{
	AstStatementWhileC *whileC = mem_alloc(sizeof(*whileC));
	whileC->condition = condition;
	whileC->a = a;
	whileC->keyword = keyword;
	return whileC;
}

static AstExpressionAssign *astExpression_initAssign (AstExpression *a, AstExpression *b, Token operator)
{
	AstExpressionAssign *assign = mem_alloc(sizeof(*assign));
	assign->a = a;
	assign->b = b;
	assign->operator = operator;
	return assign;
}

static AstExpressionBinary *astExpression_initBinary (AstExpression *a, AstExpression *b, Token operator)
{
	AstExpressionBinary *binary = mem_alloc(sizeof(*binary));
	binary->a = a;
	binary->b = b;
	binary->operator = operator;
	return binary;
}

static AstExpressionBoolean *astExpression_initBoolean (bool value)
{
	AstExpressionBoolean *boolean = mem_alloc(sizeof(*boolean));
	boolean->value = value;
	return boolean;
}

static AstExpressionCast *astExpression_initCast (AstExpression *e, Token operator, DataType *to)
{
	AstExpressionCast *cast = mem_alloc(sizeof(*cast));
	cast->e = e;
	cast->operator = operator;
	cast->to = to;
	return cast;
}

static AstExpressionNumber *astExpression_initNumber (Token value)
{
	AstExpressionNumber *number = mem_alloc(sizeof(*number));
	number->value = value;
	return number;
}

static AstExpressionPostfix *astExpression_initPostfix (Token operator, AstExpression *e)
{
	AstExpressionPostfix *postfix = mem_alloc(sizeof(*postfix));
	postfix->operator = operator;
	postfix->e = e;
	return postfix;
}

static AstExpressionPrefix *astExpression_initPrefix (Token operator, AstExpression *e)
{
	AstExpressionPrefix *prefix = mem_alloc(sizeof(*prefix));
	prefix->operator = operator;
	prefix->e = e;
	return prefix;
}

static AstExpressionTernary *astExpression_initTernary (AstExpression *condition, AstExpression *a, AstExpression *b, Token operator)
{
	AstExpressionTernary *ternary = mem_alloc(sizeof(*ternary));
	ternary->condition = condition;
	ternary->a = a;
	ternary->b = b;
	ternary->operator = operator;
	return ternary;
}

static AstExpressionVar *astExpression_initVar (Token identifier)
{
	AstExpressionVar *var = mem_alloc(sizeof(*var));
	var->identifier = identifier;
	return var;
}
