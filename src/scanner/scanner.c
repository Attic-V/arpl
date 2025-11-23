#include "internal.h"

void scanner_scan (struct file_reader *reader)
{
	for (;;) {
		char ch = file_getChar(reader);
		if (ch == EOF) break;
		switch (ch) {
			case '\n':
			case '\t':
			case ' ':
				continue;
		}
		printf("%c", ch);
	}
}

