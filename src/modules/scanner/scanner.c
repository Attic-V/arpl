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
	scanner->row = 1;
}

void scanner_detach (struct scanner_scanner *scanner)
{
	scanner->reader = NULL;
}

enum token_tokentype scanner_getTokentype (struct scanner_scanner *scanner)
{
	char ch = file_getChar(scanner->reader);

	if (ch == EOF) {
		return token_eof;
	}
	if (ch == '\n') {
		scanner->row++;
		return scanner_getTokentype(scanner);
	}

	fprintf(stderr, "%d: unexpected character: '%c'\n", scanner->row, ch);
	return scanner_getTokentype(scanner);
}

