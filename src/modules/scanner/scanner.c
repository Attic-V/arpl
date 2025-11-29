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
	scanner->row = 1;

	scanner->delayed = queue_create();
}

void scanner_detach (struct scanner_scanner *scanner)
{
	scanner->reader = NULL;

	queue_destroy(scanner->delayed);
}

struct token_token scanner_getToken (struct scanner_scanner *scanner)
{
	if (!queue_isEmpty(scanner->delayed)) {
		struct token_token *tokenptr = queue_dequeue(scanner->delayed);
		struct token_token token;
		memcpy(&token, tokenptr, sizeof(struct token_token));
		free(tokenptr);
		return token;
	}

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
			char c = file_getChar(scanner->reader);
			int value = c - '0';

			struct token_token token = scanner_getToken(scanner);
			if (token.type != token_type_number) {
				struct token_token *tokenptr = malloc(sizeof(struct token_token));
				memcpy(tokenptr, &token, sizeof(struct token_token));
				queue_enqueue(scanner->delayed, tokenptr);
				return (struct token_token){token_type_number,
					.as.number.value = value,
				};
			} else {
				int numdigits = token.as.number.value == 0
					? 1
					: (int)floor(log10(fabs((double)token.as.number.value))) + 1;
				int newvalue = value * (int)pow(10, numdigits) + token.as.number.value;
				return (struct token_token){token_type_number,
					.as.number.value = newvalue,
				};
			}
		default:
			return (struct token_token){token_type_unexpected,
				.as.unexpected.c = file_getChar(scanner->reader),
			};
	}
}

