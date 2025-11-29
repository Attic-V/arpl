#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "parser.h"
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
	struct parser_parser *parser = parser_create();

	scanner_attach(scanner, reader);
	parser_attach(parser, scanner);

	parser_displayTokens(parser);

	scanner_detach(scanner);
	parser_detach(parser);

	file_close(reader);
	scanner_destroy(scanner);
	parser_destroy(parser);

	return 0;
}

