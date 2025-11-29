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
	switch (peekType(scanner)) {
		case token_type_eof:
			file_getChar(scanner->reader);
			return (struct token_token){
				.type = token_type_eof,
			};
		case token_type_newline:
			file_getChar(scanner->reader);
			return (struct token_token){
				.type = token_type_newline,
			};
		case token_type_number:
			int value = file_getChar(scanner->reader) - '0';

			if (peekType(scanner) == token_type_number) {
				struct token_token token = scanner_getToken(scanner);
				int numdigits = token.as.number.value == 0 ? 1
					: (int)floor(log10(fabs((double)token.as.number.value))) + 1;
				int newvalue = value * (int)pow(10, numdigits) + token.as.number.value;
				return (struct token_token){
					.type = token_type_number,
					.as.number.value = newvalue,
				};
			} else {
				return (struct token_token){
					.type = token_type_number,
					.as.number.value = value,
				};
			}
		case token_type_unexpected:
		default:
			return (struct token_token){
				.type = token_type_unexpected,
				.as.unexpected.c = file_getChar(scanner->reader),
			};
	}
}

enum token_type peekType (struct scanner_scanner *scanner)
{
	switch (file_peekChar(scanner->reader)) {
		case EOF:
			return token_type_eof;
		case '\n':
			return token_type_newline;
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
			return token_type_number;
		default:
			return token_type_unexpected;
	}
}

