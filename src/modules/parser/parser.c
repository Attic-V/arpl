#include "internal.h"

#include <stdio.h>
#include <stdlib.h>

struct parser_parser *parser_create (void)
{
	struct parser_parser *parser = malloc(sizeof(struct parser_parser));
	parser->scanner = NULL;
	return parser;
}

void parser_destroy (struct parser_parser *parser)
{
	free(parser);
}

void parser_attach (struct parser_parser *parser, struct scanner_scanner *scanner)
{
	parser->scanner = scanner;
}

void parser_detach (struct parser_parser *parser)
{
	parser->scanner = NULL;
}

void parser_displayTokens (struct parser_parser *parser)
{
	for (;;) {
		struct token_token token = scanner_getToken(parser->scanner);
		printf("%s ", token_getLabel(token.type));
		switch (token.type) {
			case token_type_eof:
				break;
			case token_type_newline:
				break;
			case token_type_number:
				printf("%d", token.as.number.value);
				break;
			case token_type_unexpected:
				printf("'%c'", token.as.unexpected.c);
				break;
		}
		printf("\n");

		if (token.type == token_type_eof) break;
	}
}

