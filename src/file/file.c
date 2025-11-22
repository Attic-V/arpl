#include "internal.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *file_readFile (char *path)
{
	FILE *file = fopen(path, "rb");
	if (file == NULL) {
		fprintf(stderr, "could not open file '%s': %s\n", path, strerror(errno));
		exit(1);
	}

	fseek(file, 0L, SEEK_END);
	size_t size = ftell(file);
	rewind(file);

	char *buffer = malloc(size + 1);
	if (buffer == NULL) {
		perror("memory allocation failed");
		fclose(file);
		exit(1);
	}

	size_t bytesRead = fread(buffer, 1, size, file);
	if (bytesRead < size) {
		fprintf(stderr, "could not read file '%s': %s\n", path, strerror(errno));
		fclose(file);
		free(buffer);
		exit(1);
	}

	buffer[bytesRead] = '\0';

	fclose(file);
	return buffer;
}

