#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "arena.h"
#include "data.h"
#include "hash.h"
#include "table.h"

struct Table {
	Arena *arena;
	Symbol **symbols;
	int count;
	int capacity;
	int currentIdx;
};

void table_grow (Table *table);

Table *table_init (int capacity)
{
	Arena *arena = arena_init(4 * MB);
	Table *table = arena_allocate(arena, sizeof(*table));
	table->arena = arena;

	table->count = 0;
	table->capacity = capacity;
	table->symbols = arena_allocate(table->arena, sizeof(Symbol *) * table->capacity);
	for (int i = 0; i < table->capacity; i++) {
		table->symbols[i] = NULL;
	}
	table->currentIdx = 0;

	return table;
}

void table_free (Table *table)
{
	arena_free(table->arena);
}

bool table_add (Table *table, Symbol *symbol)
{
	uint32_t hash = getHash(symbol->identifier.lexeme, symbol->identifier.length);
	uint32_t idx = hash % table->capacity;
	if (table_get(table, symbol->identifier) != NULL) {
		return false;
	}
	for (; table->symbols[idx] != NULL; idx++, idx %= table->capacity);

	symbol->n = table->count++;
	symbol->index = table->currentIdx;
	table->currentIdx += getDtSize(symbol->type);

	table->symbols[idx] = symbol;
	if (table->count * 2 >= table->capacity) {
		table_grow(table);
	}
	return true;
}

Symbol *table_get (Table *table, Token identifier)
{
	uint32_t hash = getHash(identifier.lexeme, identifier.length);
	for (int i = hash % table->capacity; table->symbols[i] != NULL; i++) {
		i %= table->capacity;
		char *bufA = arena_allocate(table->arena, identifier.length + 1);
		char *bufB = arena_allocate(table->arena, table->symbols[i]->identifier.length + 1);
		sprintf(bufA, "%.*s", identifier.length, identifier.lexeme);
		sprintf(bufB, "%.*s", table->symbols[i]->identifier.length, table->symbols[i]->identifier.lexeme);
		if (!strcmp(bufA, bufB)) {
			return table->symbols[i];
		}
	}
	return NULL;
}

void table_apply (Table *table, void (*callback)(Symbol *symbol))
{
	for (int i = 0; i < table->capacity; i++) {
		if (table->symbols[i] == NULL) continue;
		callback(table->symbols[i]);
	}
}

void table_grow (Table *table)
{
	Table *new = table_init(table->capacity * 2);
	new->count = table->count;
	for (int i = 0; i < table->capacity; i++) {
		if (table->symbols[i] == NULL) continue;
		table_add(new, table->symbols[i]);
	}
	table_free(table);
	table = new;
}
