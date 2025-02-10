#ifndef ast_h
#define ast_h

#include <stdbool.h>

#include "data_type.h"
#include "table.h"
#include "token.h"

typedef struct AstExpression AstExpression;

typedef struct {
	AstExpression *a;
	AstExpression *b;
	Token operator;
} AstExpressionAssign;

typedef struct {
	AstExpression *a;
	AstExpression *b;
	Token operator;
} AstExpressionBinary;

typedef struct {
	bool value;
} AstExpressionBoolean;

typedef struct {
	Token value;
} AstExpressionNumber;

typedef struct {
	AstExpression *condition;
	AstExpression *a;
	AstExpression *b;
	Token operator;
} AstExpressionTernary;

typedef struct {
	Token operator;
	AstExpression *right;
} AstExpressionUnary;

typedef struct {
	Token identifier;
} AstExpressionVar;

typedef enum {
	AstExpression_Assign,
	AstExpression_Binary,
	AstExpression_Boolean,
	AstExpression_Number,
	AstExpression_Ternary,
	AstExpression_Unary,
	AstExpression_Var,
} AstExpressionType;

struct AstExpression {
	AstExpressionType type;
	union {
		AstExpressionAssign *assign;
		AstExpressionBinary *binary;
		AstExpressionBoolean *boolean;
		AstExpressionNumber *number;
		AstExpressionTernary *ternary;
		AstExpressionUnary *unary;
		AstExpressionVar *var;
	} as;
	DataType dataType;
};

typedef struct AstStatement AstStatement;

typedef struct {
	AstStatement *children;
	Table *table;
} AstStatementBlock;

typedef struct {
	AstExpression *expression;
} AstStatementExpr;

typedef struct {
	AstExpression *condition;
	AstStatement *a;
	AstStatement *b;
} AstStatementIfE;

typedef struct {
	Token identifier;
	Token type;
} AstStatementVar;

typedef enum {
	AstStatement_Block,
	AstStatement_Expr,
	AstStatement_IfE,
	AstStatement_Var,
} AstStatementType;

struct AstStatement {
	AstStatementType type;
	union {
		AstStatementBlock *block;
		AstStatementExpr *expr;
		AstStatementIfE *ifE;
		AstStatementVar *var;
	} as;
	AstStatement* next;
	AstStatement* previous;
};

typedef struct {
	AstStatement *statement;
} AstRoot;

typedef struct {
	AstRoot *root;
} Ast;

Ast *ast_init (AstRoot *root);
AstRoot *ast_initRoot (AstStatement *statement);

AstStatement *ast_initStatementBlock (AstStatement *children);
AstStatement *ast_initStatementExpr (AstExpression *expression);
AstStatement *ast_initStatementIfE (AstExpression *condition, AstStatement *a, AstStatement *b);
AstStatement *ast_initStatementVar (Token identifier, Token type);

AstExpression *ast_initExpressionAssign (AstExpression *a, AstExpression *b, Token operator);
AstExpression *ast_initExpressionBinary (AstExpression *a, AstExpression *b, Token operator);
AstExpression *ast_initExpressionBoolean (bool value);
AstExpression *ast_initExpressionNumber (Token value);
AstExpression *ast_initExpressionTernary (AstExpression *condition, AstExpression *a, AstExpression *b, Token operator);
AstExpression *ast_initExpressionUnary (Token operator, AstExpression *right);
AstExpression *ast_initExpressionVar (Token identifier);

#endif
