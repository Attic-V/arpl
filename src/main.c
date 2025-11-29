#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "scanner.h"

int main (int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s <path>\n", argv[0]);
		return 1;
	}

	char *path = argv[1];

	struct file_reader *reader = file_open(path);
	struct scanner_scanner *scanner = scanner_create();

	scanner_attach(scanner, reader);

	for (;;) {
		struct token_token token = scanner_getToken(scanner);
		printf("%s ", token_getLabel(token.type));
		switch (token.type) {
			case token_type_eof:
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

	scanner_detach(scanner);

	file_close(reader);
	scanner_destroy(scanner);

	return 0;
}

