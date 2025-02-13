#include "memory.h"
#include "symbol.h"

Symbol *symbol_init (Token identifier, DataType type)
{
	Symbol *symbol = mem_alloc(sizeof(*symbol));
	symbol->n = ~0;
	symbol->identifier = identifier;
	symbol->type = type;
	return symbol;
}
