#ifndef ast_h
#define ast_h

#include <stdbool.h>

#include "data_type.h"
#include "scope.h"
#include "table.h"
#include "token.h"

typedef struct AstExpression AstExpression;

typedef struct {
	AstExpression *a;
	AstExpression *b;
	Token operator;
} AstExpressionAccessElement;

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
	Token operator;
	AstExpression *e;
} AstExpressionPostfix;

typedef struct {
	Token operator;
	AstExpression *e;
} AstExpressionPrefix;

typedef struct {
	AstExpression *condition;
	AstExpression *a;
	AstExpression *b;
	Token operator;
} AstExpressionTernary;

typedef struct {
	Token identifier;
} AstExpressionVar;

typedef enum {
	AstExpression_AccessElement,
	AstExpression_Assign,
	AstExpression_Binary,
	AstExpression_Boolean,
	AstExpression_Number,
	AstExpression_Postfix,
	AstExpression_Prefix,
	AstExpression_Ternary,
	AstExpression_Var,
} AstExpressionType;

struct AstExpression {
	AstExpressionType type;
	union {
		AstExpressionAccessElement *accessElement;
		AstExpressionAssign *assign;
		AstExpressionBinary *binary;
		AstExpressionBoolean *boolean;
		AstExpressionNumber *number;
		AstExpressionPostfix *postfix;
		AstExpressionPrefix *prefix;
		AstExpressionTernary *ternary;
		AstExpressionVar *var;
	} as;
	DataType *dataType;
	bool modifiable;
};

typedef struct AstStatement AstStatement;

typedef struct {
	AstStatement *children;
	Scope *scope;
} AstStatementBlock;

typedef struct {
	Token keyword;
} AstStatementBreakL;

typedef struct {
	AstExpression *e;
	AstStatement *body;
	Token keyword;
} AstStatementCaseL;

typedef struct {
	Token keyword;
	Token identifier;
	DataType *type;
	Token operator;
	AstExpression *expression;
} AstStatementConstD;

typedef struct {
	Token keyword;
} AstStatementContinueL;

typedef struct {
	AstStatement *a;
	AstExpression *condition;
	Token keyword;
} AstStatementDoWhile;

typedef struct {
	AstExpression *expression;
} AstStatementExpr;

typedef struct {
	AstStatement *init;
	AstExpression *condition;
	AstExpression *update;
	AstStatement *body;
	Token keyword;
} AstStatementForI;

typedef struct {
	AstExpression *condition;
	AstStatement *a;
	AstStatement *b;
	Token keyword;
} AstStatementIfE;

typedef struct {
	Token identifier;
	DataType *type;
	AstExpression *expression;
	Token operator;
} AstStatementInit;

typedef struct {
	AstExpression *expression;
} AstStatementReturnE;

typedef struct {
	AstExpression *e;
	AstStatement *body;
} AstStatementSwitchC;

typedef struct {
	Token identifier;
	DataType *type;
} AstStatementVar;

typedef struct {
	AstExpression *condition;
	AstStatement *a;
	Token keyword;
} AstStatementWhileC;

typedef enum {
	AstStatement_Block,
	AstStatement_BreakL,
	AstStatement_CaseL,
	AstStatement_ConstD,
	AstStatement_ContinueL,
	AstStatement_DoWhile,
	AstStatement_Expr,
	AstStatement_ForI,
	AstStatement_IfE,
	AstStatement_Init,
	AstStatement_ReturnE,
	AstStatement_SwitchC,
	AstStatement_Var,
	AstStatement_WhileC,
} AstStatementType;

struct AstStatement {
	AstStatementType type;
	union {
		AstStatementBlock *block;
		AstStatementBreakL *breakL;
		AstStatementCaseL *caseL;
		AstStatementConstD *constD;
		AstStatementContinueL *continueL;
		AstStatementDoWhile *doWhile;
		AstStatementExpr *expr;
		AstStatementForI *forI;
		AstStatementIfE *ifE;
		AstStatementInit *init;
		AstStatementReturnE *returnE;
		AstStatementSwitchC *switchC;
		AstStatementVar *var;
		AstStatementWhileC *whileC;
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
AstStatement *ast_initStatementBreakL (Token keyword);
AstStatement *ast_initStatementCaseL (AstExpression *e, AstStatement *body, Token keyword);
AstStatement *ast_initStatementConstD (Token keyword, Token identifier, DataType *type, Token operator, AstExpression *expression);
AstStatement *ast_initStatementContinueL (Token keyword);
AstStatement *ast_initStatementDoWhile (AstStatement *a, AstExpression *condition, Token keyword);
AstStatement *ast_initStatementExpr (AstExpression *expression);
AstStatement *ast_initStatementForI (AstStatement *init, AstExpression* condition, AstExpression *update, AstStatement *body, Token keyword);
AstStatement *ast_initStatementIfE (AstExpression *condition, AstStatement *a, AstStatement *b, Token keyord);
AstStatement *ast_initStatementInit (Token identifier, DataType *type, AstExpression *expression, Token operator);
AstStatement *ast_initStatementReturnE (AstExpression *expression);
AstStatement *ast_initStatementSwitchC (AstExpression *e, AstStatement *body);
AstStatement *ast_initStatementVar (Token identifier, DataType *type);
AstStatement *ast_initStatementWhileC (AstExpression *condition, AstStatement* a, Token keyword);

AstExpression *ast_initExpressionAccessElement (AstExpression *a, AstExpression *b, Token operator);
AstExpression *ast_initExpressionAssign (AstExpression *a, AstExpression *b, Token operator);
AstExpression *ast_initExpressionBinary (AstExpression *a, AstExpression *b, Token operator);
AstExpression *ast_initExpressionBoolean (bool value);
AstExpression *ast_initExpressionNumber (Token value);
AstExpression *ast_initExpressionPostfix (Token operator, AstExpression *e);
AstExpression *ast_initExpressionPrefix (Token operator, AstExpression *e);
AstExpression *ast_initExpressionTernary (AstExpression *condition, AstExpression *a, AstExpression *b, Token operator);
AstExpression *ast_initExpressionVar (Token identifier);

#endif
