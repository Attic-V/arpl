#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "memory.h"

char *readFile (char *path)
{
	FILE *file = fopen(path, "rb");
	if (file == NULL) {
		fprintf(stderr, "could not open file\n");
		exit(1);
	}

	fseek(file, 0L, SEEK_END);
	size_t size = ftell(file);
	rewind(file);

	char *buffer = (char *)mem_alloc(size + 1);
	if (buffer == NULL) {
		fprintf(stderr, "memory allocation failed\n");
		fclose(file);
		exit(1);
	}

	size_t bytesRead = fread(buffer, sizeof(char), size, file);
	if (bytesRead < size) {
		fprintf(stderr, "could not read file\n");
		fclose(file);
		exit(1);
	}

	buffer[bytesRead] = '\0';

	fclose(file);
	return buffer;
}
