#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"

typedef struct {
	char *start;
	char *current;
	int line;
	Token *tokens;
	int count;
} Scanner;

Scanner scanner;

void addToken (TokenType type);

Token *scan (char *source)
{
	scanner.current = source;
	scanner.line = 1;
	scanner.tokens = malloc(sizeof(Token));
	scanner.count = 0;

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
		}

		if (isdigit(ch)) {
			while (isdigit(*scanner.current)) {
				scanner.current++;
			}
			addToken(TT_Number);
			continue;
		}

		fprintf(stderr, "%d: error: unexpected character '%c'\n", scanner.line, ch);
	}

	scanner.tokens[scanner.count] = (Token){
		.type = TT_EOF,
		.lexeme = scanner.current,
		.length = 1,
		.line = scanner.line,
	};
	return scanner.tokens;
}

void addToken (TokenType type)
{
	scanner.tokens[scanner.count++] = (Token){
		.type = type,
		.lexeme = scanner.start,
		.length = scanner.current - scanner.start,
		.line = scanner.line,
	};
	scanner.tokens = realloc(scanner.tokens, sizeof(Token) * (scanner.count + 1));
}
