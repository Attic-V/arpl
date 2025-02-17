#ifndef table_h
#define table_h

#include <stdbool.h>

#include "symbol.h"

typedef struct Table Table;

Table *table_init (int capacity);
void table_free (Table *table);
bool table_add (Table *table, Symbol *symbol);
Symbol *table_get (Table *table, Token identifier);

#endif
