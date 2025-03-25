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
			case '[': addToken(TT_LBracket); continue;
			case '<': addToken(match('<') ? TT_Less_Less : match('=') ? TT_Less_Equal : TT_Less); continue;
			case '(': addToken(TT_LParen); continue;
			case '-': addToken(match('>') ? TT_Minus_Greater : match('-') ? TT_Minus_Minus : match('=') ? TT_Minus_Equal : TT_Minus); continue;
			case '|': addToken(match('|') ? TT_Pipe_Pipe : TT_Pipe); continue;
			case '+': addToken(match('=') ? TT_Plus_Equal : match('+') ? TT_Plus_Plus : TT_Plus); continue;
			case '?': addToken(TT_Question); continue;
			case '}': addToken(TT_RBrace); continue;
			case ']': addToken(TT_RBracket); continue;
			case ')': addToken(TT_RParen); continue;
			case ';': addToken(TT_Semicolon); continue;
			case '*': addToken(match('=') ? TT_Star_Equal : TT_Star); continue;
			case '~': addToken(match('>') ? TT_Tilde_Greater : TT_Tilde); continue;
			case '/':
				if (match('/')) {
					while (!check('\0') && !match('\n')) {
						scanner.current++;
					}
					scanner.line++;
					continue;
				}
				if (match('*')) {
					while (!check('\0') && !(match('*') && match('/'))) {
						if (match('\n')) scanner.line++;
						else scanner.current++;
					}
					if (check('\0')) {
						fprintf(stderr, "%d: error at EOF: unclosed comment\n", scanner.line);
					}
					continue;
				}
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

			#define keyword(lexeme, tokentype) if (!strcmp(buffer, #lexeme)) { token->type = TT_##tokentype; continue; }
			keyword(bool, Bool)
			keyword(break, Break)
			keyword(case, Case)
			keyword(continue, Continue)
			keyword(default, Default)
			keyword(do, Do)
			keyword(else, Else)
			keyword(false, False)
			keyword(for, For)
			keyword(i16, I16)
			keyword(i32, I32)
			keyword(i64, I64)
			keyword(i8, I8)
			keyword(if, If)
			keyword(mut, Mut)
			keyword(return, Return)
			keyword(switch, Switch)
			keyword(true, True)
			keyword(u16, U16)
			keyword(u32, U32)
			keyword(u64, U64)
			keyword(u8, U8)
			keyword(var, Var)
			keyword(while, While)
			#undef keyword
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
