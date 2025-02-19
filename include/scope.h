#ifndef scope_h
#define scope_h

#include "table.h"

typedef struct Scope Scope;

struct Scope {
	Table *table;
	Scope *parent;
	Scope *children;
	size_t physicalSize;
};

Scope *scope_init (void);
void scope_free (Scope *scope);
bool scope_add (Scope *scope, Symbol *symbol);
Symbol *scope_get (Scope *scope, Token identifier);
size_t scope_physicalSize (Scope *scope);
/**
 * `scope_get` that also searches parent scopes
 */
Symbol *telescope_get (Scope *scope, Token identifier);

#endif
