#include "memory.h"
#include "scope.h"

Scope *scope_init (void)
{
	Scope *scope = mem_alloc(sizeof(*scope));
	scope->parent = NULL;
	scope->table = table_init(64);
	scope->physicalSize = 0;
	return scope;
}

void scope_free (Scope *scope)
{
	table_free(scope->table);
}

bool scope_add (Scope *scope, Symbol *symbol)
{
	scope->physicalSize += getDtSize(symbol->type);
	return table_add(scope->table, symbol);
}

Symbol *scope_get (Scope *scope, Token identifier)
{
	return table_get(scope->table, identifier);
}

size_t scope_physicalSize (Scope *scope)
{
	return scope->physicalSize;
}

Symbol *telescope_get (Scope *scope, Token identifier)
{
	for (Scope *s = scope; s != NULL; s = s->parent) {
		Symbol *symbol = scope_get(s, identifier);
		if (symbol == NULL) {
			continue;
		}
		return symbol;
	}
	return NULL;
}
