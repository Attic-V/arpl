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
	if (!reader) return 1;

	scanner_scan(reader);

	file_close(reader);

	return 0;
}

