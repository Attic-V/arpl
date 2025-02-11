#ifndef table_h
#define table_h

#include <stdbool.h>

#include "data_type.h"
#include "token.h"

typedef struct Table Table;

typedef struct {
	Token identifier;
	DataType type;
	size_t n;
	size_t index;
} Symbol;

Table *table_init (int capacity);
void table_free (Table *table);
bool table_add (Table *table, Symbol *symbol);
Symbol *table_get (Table *table, Token identifier);
void table_apply (Table *table, void (*callback)(Symbol *symbol));

Symbol *symbol_init (Token identifier, DataType type);

#endif
