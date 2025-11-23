#include "internal.h"

struct scanner_scanner *scanner_create (void)
{
	struct scanner_scanner *scanner = malloc(sizeof(struct scanner_scanner));

	scanner->reader = NULL;

	return scanner;
}

void scanner_destroy (struct scanner_scanner *scanner)
{
	free(scanner);
}

void scanner_attach (struct scanner_scanner *scanner, struct file_reader *reader)
{
	scanner->reader = reader;
}

void scanner_detach (struct scanner_scanner *scanner)
{
	scanner->reader = NULL;
}

struct token_token scanner_getToken (struct scanner_scanner *scanner)
{
	char ch = file_getChar(scanner->reader);

	struct token_token token;
	token.type = ch == EOF ? token_eof : token_character;
	token.ch = ch;

	return token;
}

