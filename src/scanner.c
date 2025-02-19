#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "scanner.h"

typedef struct {
	char *start;
	char *current;
	Token *tokens;
	int count;
	int line;
} Scanner;

static Scanner scanner;

static void addToken (TokenType type);
static bool check (char ch);
static bool match (char ch);

Token *scan (char *source)
{
	scanner.current = source;
	scanner.tokens = mem_alloc(sizeof(Token));
	scanner.count = 0;
	scanner.line = 1;

	for (;;) {
		if (*scanner.current == '\0') break;

		scanner.start = scanner.current;

		char ch = *scanner.current++;
		switch (ch) {
			case '\n':
				scanner.line++;
			case '\r':
			case '\t':
			case ' ':
				continue;

			case '&': addToken(match('&') ? TT_And_And : TT_And); continue;
			case '!':	addToken(match('=') ? TT_Bang_Equal : TT_Bang); continue;
			case '^': addToken(TT_Caret); continue;
			case ':': addToken(TT_Colon); continue;
			case '=': addToken(match('=') ? TT_Equal_Equal : TT_Equal); continue;
			case '>':
				if (check('>')) {
					scanner.current++;
					addToken(TT_Greater_Greater);
					continue;
				}
				break;
			case '{': addToken(TT_LBrace); continue;
			case '<': addToken(match('<') ? TT_Less_Less : match('=') ? TT_Less_Equal : TT_Less); continue;
			case '(': addToken(TT_LParen); continue;
			case '-': addToken(match('=') ? TT_Minus_Equal : TT_Minus); continue;
			case '|': addToken(match('|') ? TT_Pipe_Pipe : TT_Pipe); continue;
			case '+': addToken(match('=') ? TT_Plus_Equal : match('+') ? TT_Plus_Plus : TT_Plus); continue;
			case '?': addToken(TT_Question); continue;
			case '}': addToken(TT_RBrace); continue;
			case ')': addToken(TT_RParen); continue;
			case ';': addToken(TT_Semicolon); continue;
			case '*': addToken(TT_Star); continue;
			case '~': addToken(TT_Tilde); continue;
		}

		if (isdigit(ch)) {
			while (isdigit(*scanner.current)) {
				scanner.current++;
			}
			addToken(TT_Number);
			continue;
		}

		if (isalpha(ch)) {
			while (isalnum(*scanner.current)) {
				scanner.current++;
			}
			addToken(TT_Identifier);
			Token *token = &scanner.tokens[scanner.count - 1];
			char *buffer = mem_alloc(token->length + 1);
			sprintf(buffer, "%.*s", token->length, token->lexeme);
			if (!strcmp(buffer, "bool")) { token->type = TT_Bool; continue; }
			if (!strcmp(buffer, "do")) { token->type = TT_Do; continue; }
			if (!strcmp(buffer, "else")) { token->type = TT_Else; continue; }
			if (!strcmp(buffer, "false")) { token->type = TT_False; continue; }
			if (!strcmp(buffer, "if")) { token->type = TT_If; continue; }
			if (!strcmp(buffer, "int")) { token->type = TT_Int; continue; }
			if (!strcmp(buffer, "true")) { token->type = TT_True; continue; }
			if (!strcmp(buffer, "var")) { token->type = TT_Var; continue; }
			if (!strcmp(buffer, "while")) { token->type = TT_While; continue; }
			continue;
		}

		fprintf(stderr, "%d: error at '%c': unexpected character\n", scanner.line, ch);
	}

	addToken(TT_EOF);

	return scanner.tokens;
}

static void addToken (TokenType type)
{
	scanner.tokens[scanner.count++] = (Token){
		.type = type,
		.lexeme = scanner.start,
		.length = scanner.current - scanner.start,
		.line = scanner.line,
	};
	scanner.tokens = mem_realloc(scanner.tokens, sizeof(Token) * (scanner.count + 1));
}

static bool check (char ch)
{
	return *scanner.current == ch;
}

static bool match (char ch)
{
	if (check(ch)) {
		scanner.current++;
		return true;
	}
	return false;
}
