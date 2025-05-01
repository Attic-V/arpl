#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "err.h"
#include "linked_list.h"
#include "memory.h"
#include "parser.h"

static AstRoot *getRoot (void);

static AstDeclaration *getDeclaration (void);
static AstDeclaration *getDeclarationFunction (void);

static AstStatement *getStatement (void);
static AstStatement *getStatementBlock (void);
static AstStatement *getStatementBreakL (void);
static AstStatement *getStatementCaseL (void);
static AstStatement *getStatementContinueL (void);
static AstStatement *getStatementDoWhile (void);
static AstStatement *getStatementExpr (void);
static AstStatement *getStatementForI (void);
static AstStatement *getStatementIfE (void);
static AstStatement *getStatementReturnE (void);
static AstStatement *getStatementSwitchC (void);
static AstStatement *getStatementVar (void);
static AstStatement *getStatementWhileC (void);

static AstExpression *getExpression (void);
static AstExpression *getExpressionAndBitwise (void);
static AstExpression *getExpressionAndLogical (void);
static AstExpression *getExpressionAssign (void);
static AstExpression *getExpressionCall (void);
static AstExpression *getExpressionCast (void);
static AstExpression *getExpressionEquality (void);
static AstExpression *getExpressionOrBitwise (void);
static AstExpression *getExpressionOrLogical (void);
static AstExpression *getExpressionPrimary (void);
static AstExpression *getExpressionProduct (void);
static AstExpression *getExpressionRelational (void);
static AstExpression *getExpressionShift (void);
static AstExpression *getExpressionSum (void);
static AstExpression *getExpressionTernary (void);
static AstExpression *getExpressionUnaryPostfix (void);
static AstExpression *getExpressionUnaryPrefix (void);
static AstExpression *getExpressionXor (void);

static AstArgument *getArgument (void);
static AstParameter *getParameter (void);

static DataType *getType (void);

static bool check (TokenType type);
static bool match (TokenType type);
static Token consume (TokenType type, char *format, ...);

#define expect(ttype, str) \
	consume(TT_##ttype, "expected "#str)

typedef struct {
	Token *tokens;
	int current;
} Parser;

static Parser parser;

Ast *parse (Token *tokens)
{
	parser.tokens = tokens;
	parser.current = 0;

	AstRoot *root = getRoot();
	Ast *ast = ast_init(root);

	return ast;
}

static AstRoot *getRoot (void)
{
	AstDeclaration *declarations = NULL;
	while (!check(TT_EOF)) {
		AstDeclaration *declaration = getDeclaration();
		dll_shove(declarations, declaration);
	}
	dll_rewind(declarations);
	return ast_initRoot(declarations);
}

static AstDeclaration *getDeclaration (void)
{
	switch (parser.tokens[parser.current].type) {
		case TT_Fn: return getDeclarationFunction();
		default:
	}
	err(parser.tokens[parser.current], "expected start of declaration");
	exit(1);
}

static AstDeclaration *getDeclarationFunction (void)
{
	Token keyword = expect(Fn, 'fn');
	Token identifier = expect(Identifier, identifier);
	expect(LParen, '(');
	AstParameter *parameters = NULL;
	if (!check(TT_RParen)) {
		do {
			AstParameter *parameter = getParameter();
			dll_shove(parameters, parameter);
		} while (match(TT_Comma));
	}
	dll_rewind(parameters);
	expect(RParen, ')');
	DataType *returnType = getType();
	return ast_initDeclarationFunction(keyword, identifier, getStatementBlock(), returnType, parameters);
}

static AstStatement *getStatement (void)
{
	switch (parser.tokens[parser.current].type) {
		case TT_Break: return getStatementBreakL();
		case TT_Continue: return getStatementContinueL();
		case TT_Do: return getStatementDoWhile();
		case TT_For: return getStatementForI();
		case TT_If: return getStatementIfE();
		case TT_LBrace: return getStatementBlock();
		case TT_Return: return getStatementReturnE();
		case TT_Switch: return getStatementSwitchC();
		case TT_Var: return getStatementVar();
		case TT_While: return getStatementWhileC();
		default:
	}
	return getStatementExpr();
}

static AstStatement *getStatementBlock (void)
{
	expect(LBrace, '{');
	AstStatement *statements = NULL;
	while (!check(TT_RBrace)) {
		AstStatement *statement = getStatement();
		dll_shove(statements, statement);
	}
	expect(RBrace, '}');
	dll_rewind(statements);
	return ast_initStatementBlock(statements);
}

static AstStatement *getStatementBreakL (void)
{
	Token keyword = expect(Break, 'break');
	expect(Semicolon, ';');
	return ast_initStatementBreakL(keyword);
}

static AstStatement *getStatementCaseL (void)
{
	Token keyword;
	AstExpression *expression = NULL;
	if (check(TT_Case)) {
		keyword = expect(Case, 'case');
		expression = getExpression();
	} else {
		keyword = expect(Default, 'default');
	}
	expect(Colon, ':');
	AstStatement *statements = NULL;
	while (!(check(TT_Case) || check(TT_Default) || check(TT_RBrace))) {
		AstStatement *statement = getStatement();
		dll_shove(statements, statement);
	}
	dll_rewind(statements);
	return ast_initStatementCaseL(expression, statements, keyword);
}

static AstStatement *getStatementContinueL (void)
{
	Token keyword = expect(Continue, 'continue');
	expect(Semicolon, ';');
	return ast_initStatementContinueL(keyword);
}

static AstStatement *getStatementDoWhile (void)
{
	expect(Do, 'do');
	AstStatement *statement = getStatement();
	Token keyword = expect(While, 'while');
	expect(LParen, '(');
	AstExpression *condition = getExpression();
	expect(RParen, ')');
	expect(Semicolon, ';');
	return ast_initStatementDoWhile(statement, condition, keyword);
}

static AstStatement *getStatementExpr (void)
{
	AstStatement *statement = ast_initStatementExpr(getExpression());
	expect(Semicolon, ';');
	return statement;
}

static AstStatement *getStatementForI (void)
{
	Token keyword = expect(For, 'for');
	expect(LParen, '(');
	AstStatement *init = NULL;
	if (check(TT_Semicolon)) {
		parser.current++;
	} else {
		init = getStatement();
	}
	AstExpression *condition = NULL;
	if (!check(TT_Semicolon)) {
		condition = getExpression();
	}
	expect(Semicolon, ';');
	AstExpression *update = NULL;
	if (!check(TT_RParen)) {
		update = getExpression();
	}
	expect(RParen, ')');
	AstStatement *body = NULL;
	if (check(TT_Semicolon)) {
		parser.current++;
	} else {
		body = getStatement();
	}
	return ast_initStatementBlock(ast_initStatementForI(init, condition, update, body, keyword));
}

static AstStatement *getStatementIfE (void)
{
	Token keyword = expect(If, 'if');
	expect(LParen, '(');
	AstExpression *condition = getExpression();
	expect(RParen, ')');
	AstStatement *a = getStatement();
	AstStatement *b = NULL;
	if (check(TT_Else)) {
		parser.current++;
		b = getStatement();
	}
	return ast_initStatementIfE(condition, a, b, keyword);
}

static AstStatement *getStatementReturnE (void)
{
	Token keyword = expect(Return, 'return');
	AstExpression *expression = NULL;
	if (!check(TT_Semicolon)) {
		expression = getExpression();
	}
	expect(Semicolon, ';');
	return ast_initStatementReturnE(keyword, expression);
}

static AstStatement *getStatementSwitchC (void)
{
	expect(Switch, 'switch');
	expect(LParen, '(');
	AstExpression *expression = getExpression();
	expect(RParen, ')');
	expect(LBrace, '{');
	AstStatement *statements = NULL;
	while (!check(TT_RBrace)) {
		AstStatement *statement = getStatementCaseL();
		dll_shove(statements, statement);
	}
	dll_rewind(statements);
	expect(RBrace, '}');
	AstStatement *body = ast_initStatementBlock(statements);
	return ast_initStatementSwitchC(expression, body);
}

static AstStatement *getStatementVar (void)
{
	expect(Var, 'var');
	Token identifier = expect(Identifier, identifier);
	DataType *type = getType();
	if (check(TT_Equal)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *expression = getExpression();
		expect(Semicolon, ';');
		return ast_initStatementInit(identifier, type, expression, operator);
	} else {
		expect(Semicolon, ';');
		return ast_initStatementVar(identifier, type);
	}
}

static AstStatement *getStatementWhileC (void)
{
	Token keyword = expect(While, 'while');
	expect(LParen, '(');
	AstExpression *condition = getExpression();
	expect(RParen, ')');
	AstStatement *statement = NULL;
	if (check(TT_Semicolon)) {
		parser.current++;
	} else {
		statement = getStatement();
	}
	return ast_initStatementWhileC(condition, statement, keyword);
}

static AstExpression *getExpression (void)
{
	return getExpressionAssign();
}

static AstExpression *getExpressionAssign (void)
{
	AstExpression *expression = getExpressionTernary();
	while (check(TT_Equal) || check(TT_Plus_Equal) || check(TT_Minus_Equal) || check(TT_Star_Equal)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpression();
		expression = ast_initExpressionAssign(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionTernary (void)
{
	AstExpression *expression = getExpressionCast();
	if (check(TT_Question)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *a = getExpressionCast();
		expect(Colon, ':');
		AstExpression *b = getExpressionTernary();
		expression = ast_initExpressionTernary(expression, a, b, operator);
	}
	return expression;
}

static AstExpression *getExpressionCast (void)
{
	AstExpression *left = getExpressionOrLogical();
	while (check(TT_Minus_Greater)) {
		Token operator = parser.tokens[parser.current++];
		DataType *to = getType();
		left = ast_initExpressionCast(left, operator, to);
	}
	return left;
}

static AstExpression *getExpressionOrLogical (void)
{
	AstExpression *expression = getExpressionAndLogical();
	while (check(TT_Pipe_Pipe)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionAndLogical();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionAndLogical (void)
{
	AstExpression *expression = getExpressionEquality();
	while (check(TT_And_And)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionEquality();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionEquality (void)
{
	AstExpression *expression = getExpressionRelational();
	while (check(TT_Equal_Equal) || check(TT_Bang_Equal)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionRelational();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionRelational (void)
{
	AstExpression *expression = getExpressionShift();
	while (check(TT_Less) || check(TT_Less_Equal)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionShift();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionShift (void)
{
	AstExpression *expression = getExpressionOrBitwise();
	while (check(TT_Less_Less) || check(TT_Greater_Greater)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionOrBitwise();
		expression  = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionOrBitwise (void)
{
	AstExpression *expression = getExpressionXor();
	while (check(TT_Pipe)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionXor();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionXor (void)
{
	AstExpression *expression = getExpressionAndBitwise();
	while (check(TT_Caret)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionAndBitwise();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionAndBitwise (void)
{
	AstExpression *expression = getExpressionSum();
	while (check(TT_And)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionSum();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionSum (void)
{
	AstExpression *expression = getExpressionProduct();
	while (check(TT_Plus) || check(TT_Minus)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionProduct();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionProduct (void)
{
	AstExpression *expression = getExpressionUnaryPostfix();
	while (check(TT_Star)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionUnaryPostfix();
		expression = ast_initExpressionBinary(expression, right, operator);
	}
	return expression;
}

static AstExpression *getExpressionUnaryPostfix (void)
{
	AstExpression *left = getExpressionUnaryPrefix();
	while (check(TT_Plus_Plus) || check(TT_Minus_Minus)) {
		Token operator = parser.tokens[parser.current++];
		left = ast_initExpressionPostfix(operator, left);
	}
	return left;
}

static AstExpression *getExpressionUnaryPrefix (void)
{
	if (check(TT_Minus) || check(TT_Bang) || check(TT_Tilde) || check(TT_Plus_Plus) || check(TT_Minus_Minus) || check(TT_And) || check(TT_Star)) {
		Token operator = parser.tokens[parser.current++];
		AstExpression *right = getExpressionUnaryPrefix();
		return ast_initExpressionPrefix(operator, right);
	}
	return getExpressionCall();
}

static AstExpression *getExpressionCall (void)
{
	AstExpression *e = getExpressionPrimary();
	if (match(TT_LParen)) {
		Token lparen = parser.tokens[parser.current - 1];
		AstArgument *arguments = NULL;
		if (!check(TT_RParen)) {
			do {
				AstArgument *argument = getArgument();
				dll_shove(arguments, argument);
			} while (match(TT_Comma));
		}
		dll_rewind(arguments);
		Token rparen = expect(RParen, ')');
		return ast_initExpressionCall(e, lparen, rparen, arguments);
	}
	return e;
}

static AstExpression *getExpressionPrimary (void)
{
	Token token = parser.tokens[parser.current++];
	switch (token.type) {
		case TT_Number:
			return ast_initExpressionNumber(token);
		case TT_LParen:
			AstExpression *expression = getExpression();
			expect(RParen, ')');
			return expression;
		case TT_True:
			return ast_initExpressionBoolean(true);
		case TT_False:
			return ast_initExpressionBoolean(false);
		case TT_Identifier:
			return ast_initExpressionVar(token);
		default:
	}
	err(token, "expected expression");
	exit(1);
}

static AstArgument *getArgument (void)
{
	return ast_initArgument(getExpression());
}

static AstParameter *getParameter (void)
{
	Token identifier = expect(Identifier, identifier);
	DataType *type = getType();
	return ast_initParameter(identifier, type);
}

static DataType *getType (void)
{
	Token token = parser.tokens[parser.current++];
	switch (token.type) {
		case TT_U16: return dataType_initU16();
		case TT_U32: return dataType_initU32();
		case TT_U64: return dataType_initU64();
		case TT_U8: return dataType_initU8();
		case TT_I16: return dataType_initI16();
		case TT_I32: return dataType_initI32();
		case TT_I64: return dataType_initI64();
		case TT_I8: return dataType_initI8();
		case TT_Bool: return dataType_initBoolean();
		case TT_Star: return dataType_initPointer(getType());
		default:
	}
	err(token, "expected type");
	exit(1);
}

static bool check (TokenType type)
{
	return parser.tokens[parser.current].type == type;
}

static bool match (TokenType type)
{
	if (check(type)) {
		parser.current++;
		return true;
	}
	return false;
}

static Token consume (TokenType type, char *format, ...)
{
	Token token = parser.tokens[parser.current++];
	if (token.type != type) {
		va_list args;
		va_start(args, format);
		verr(token, format, args);
		va_end(args);
		exit(1);
	}
	return token;
}
