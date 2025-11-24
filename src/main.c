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
		enum token_tokentype tokentype = scanner_getTokentype(scanner);
		printf("%s\n", token_getLabel(tokentype));

		if (tokentype == token_eof) break;
	}

	scanner_detach(scanner);

	file_close(reader);
	scanner_destroy(scanner);

	return 0;
}

