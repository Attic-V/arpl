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

struct token_token scanner_getToken (struct scanner_scanner *scanner)
{
	switch (file_peekChar(scanner->reader)) {
		case EOF:
			file_getChar(scanner->reader);
			return (struct token_token){token_type_eof};
		case '\n':
			file_getChar(scanner->reader);
			scanner->row++;
			return scanner_getToken(scanner);
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			int value = 0;
			for (;;) {
				char c = file_peekChar(scanner->reader);
				if (c < '0' || '9' < c) break;
				file_getChar(scanner->reader);

				value *= 10;
				value += c - '0';
			}
			return (struct token_token){token_type_number,
				.as.number.value = value,
			};
		default:
			fprintf(stderr, "%d: unexpected character: '%c'\n", scanner->row, file_getChar(scanner->reader));
			return scanner_getToken(scanner);
	}
}

