#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"

typedef struct {
	Arena *arena;
	char *start;
	char *current;
	Token *tokens;
	int count;
	int line;
} Scanner;

Scanner scanner;

void addToken (TokenType type);

Token *scan (Arena *arena, char *source)
{
	scanner.arena = arena;
	scanner.current = source;
	scanner.tokens = arena_allocate(arena, sizeof(Token));
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

	addToken(TT_EOF);

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
	scanner.tokens = arena_reallocate(scanner.arena, scanner.tokens, sizeof(Token) * (scanner.count + 1));
}
