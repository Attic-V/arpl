#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"

typedef struct {
	char *start;
	char *current;
	int line;
	Token *tokens;
	int count;
	Arena *arena;
} Scanner;

Scanner scanner;

void addToken (TokenType type);

Token *scan (Arena *arena, char *source)
{
	scanner.current = source;
	scanner.line = 1;
	scanner.tokens = arena_allocate(arena, sizeof(Token));
	scanner.count = 0;
	scanner.arena = arena_init();

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

	Token *tokens = arena_copy(arena, scanner.tokens, sizeof(Token) * scanner.count);
	arena_free(scanner.arena);

	return tokens;
}

void addToken (TokenType type)
{
	scanner.tokens[scanner.count++] = (Token){
		.type = type,
		.lexeme = scanner.start,
		.length = scanner.current - scanner.start,
		.line = scanner.line,
	};
	scanner.tokens = arena_reallocate(scanner.arena, scanner.tokens, sizeof(Token) * (scanner.count - 1), sizeof(Token) * (scanner.count + 1));
}
