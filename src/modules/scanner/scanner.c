#include "internal.h"

#include <math.h>
#include <string.h>

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
	int c = file_peekChar(scanner->reader);

	if (c == EOF) {
		file_getChar(scanner->reader);
		return (struct token_token){
			.type = token_type_eof,
		};
	}

	if (c == '\n') {
		file_getChar(scanner->reader);
		return (struct token_token){
			.type = token_type_newline,
		};
	}

	if ('0' <= c && c <= '9') {
		int value = 0;
		for (;;) {
			int ch = file_peekChar(scanner->reader);
			if (ch < '0' || '9' < ch) break;

			file_getChar(scanner->reader);
			value *= 10;
			value += ch - '0';
		}
		return (struct token_token){
			.type = token_type_number,
			.as.number.value = value,
		};
	}

	return (struct token_token){
		.type = token_type_unexpected,
		.as.unexpected.c = file_getChar(scanner->reader),
	};
}

