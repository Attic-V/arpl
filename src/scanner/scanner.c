#include "internal.h"

#include <stdio.h>

void scanner_scan (char *source)
{
	char *current = source;
	for (;;) {
		char ch = *current++;
		if (ch == '\0') break;
		switch (ch) {
			case '\n':
			case '\t':
			case ' ':
				continue;
		}
		printf("%c", ch);
	}
}

