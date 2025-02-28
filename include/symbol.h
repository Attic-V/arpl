#ifndef symbol_h
#define symbol_h

#include "data_type.h"
#include "token.h"

typedef struct {
	Token identifier;
	DataType *type;
	size_t n;
	size_t physicalIndex;
} Symbol;

Symbol *symbol_init (Token identifier, DataType *type);

#endif
