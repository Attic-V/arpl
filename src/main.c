#include <stdio.h>
#include <stdlib.h>

#include "file.h"

int main (int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: %s <path>\n", argv[0]);
		return 1;
	}

	char *path = argv[1];

	char *source = readFile(path);
	printf("%s\n", source);

	free(source);

	return 0;
}
