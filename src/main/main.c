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
	char *contents = file_readFile(path);

	scanner_scan(contents);

	free(contents);

	return 0;
}

