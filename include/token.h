#ifndef token_h
#define token_h

#include <stddef.h>

typedef enum {
	TT_And,
	TT_And_And,
	TT_Bang,
	TT_Bang_Equal,
	TT_Bool,
	TT_Break,
	TT_Caret,
	TT_Case,
	TT_Colon,
	TT_Comma,
	TT_Continue,
	TT_Default,
	TT_Do,
	TT_Else,
	TT_EOF,
	TT_Equal,
	TT_Equal_Equal,
	TT_False,
	TT_Fn,
	TT_For,
	TT_Greater_Greater,
	TT_I16,
	TT_I32,
	TT_I64,
	TT_I8,
	TT_Identifier,
	TT_If,
	TT_LBrace,
	TT_LBracket,
	TT_Less,
	TT_Less_Equal,
	TT_Less_Less,
	TT_LParen,
	TT_Minus,
	TT_Minus_Equal,
	TT_Minus_Greater,
	TT_Minus_Minus,
	TT_Number,
	TT_Pipe,
	TT_Pipe_Pipe,
	TT_Plus,
	TT_Plus_Equal,
	TT_Plus_Plus,
	TT_Question,
	TT_RBrace,
	TT_RBracket,
	TT_Return,
	TT_RParen,
	TT_Semicolon,
	TT_Star,
	TT_Star_Equal,
	TT_Switch,
	TT_Tilde,
	TT_True,
	TT_U8,
	TT_U16,
	TT_U32,
	TT_U64,
	TT_Var,
	TT_While,
} TokenType;

typedef struct {
	int line;
	char *lexeme;
	int length;
	TokenType type;
} Token;

#endif
