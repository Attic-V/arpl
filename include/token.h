#ifndef token_h
#define token_h

#include <stdbool.h>
#include <stddef.h>

typedef enum {
	TT_Comma,
	TT_EOF,
	TT_Equal,
	TT_Fn,
	TT_I8,
	TT_Identifier,
	TT_LBrace,
	TT_LBracket,
	TT_LParen,
	TT_Minus,
	TT_Number,
	TT_Plus,
	TT_RBrace,
	TT_RBracket,
	TT_Return,
	TT_RParen,
	TT_Semicolon,
	TT_Star,
	TT_Var,
} TokenType;

typedef struct {
	int line;
	char *lexeme;
	int length;
	TokenType type;
} Token;

bool token_equal (Token a, Token b);

#endif

