#include "data.h"
#include "data_type.h"
#include "linked_list.h"
#include "memory.h"
#include "x86_gen.h"

static DataType *dataType_init (DataTypeType type);

static DataTypeFunction *dataTypeFunction_init (DataType *returnType, DataType *parameters);
static DataTypeI8 *dataTypeI8_init (void);

static DataType *dataType_init (DataTypeType type)
{
	DataType *t = mem_alloc(sizeof(*t));
	t->type = type;
	dll_init(t);
	return t;
}

DataTypeMember *dataType_initMember (Token identifier, DataType *dataType, size_t index)
{
	DataTypeMember *member = mem_alloc(sizeof(*member));
	member->identifier = identifier;
	member->dataType = dataType;
	member->index = index;
	dll_init(member);
	return member;
}

DataType *dataType_initFunction (DataType *returnType, DataType *parameters)
{
	DataType *t = dataType_init(DataType_Function);
	t->as.function = dataTypeFunction_init(returnType, parameters);
	return t;
}

static DataTypeFunction *dataTypeFunction_init (DataType *returnType, DataType *parameters)
{
	DataTypeFunction *t = mem_alloc(sizeof(*t));
	t->returnType = returnType;
	t->parameters = parameters;
	return t;
}

DataType *dataType_initI8 (void)
{
	DataType *t = dataType_init(DataType_I8);
	t->as.i8 = dataTypeI8_init();
	return t;
}

static DataTypeI8 *dataTypeI8_init (void)
{
	DataTypeI8 *t = mem_alloc(sizeof(*t));
	return t;
}

bool dataType_isFunction (DataType *t)
{
	return t->type == DataType_Function;
}

bool dataType_isI8 (DataType *t)
{
	return t->type == DataType_I8;
}

bool dataType_isInt (DataType *t)
{
	return dataType_isI8(t);
}

bool dataType_castable (DataType *from, DataType *to)
{
	return CastTable[from->type][to->type] != NULL;
}

bool dataType_equal (DataType *a, DataType *b)
{
	if (a->type != b->type) {
		return false;
	}
	switch (a->type) {
		case DataType_I8: return true;
		default: return false;
	}
}

size_t dataType_getSize (DataType *t)
{
	switch (t->type) {
		case DataType_I8: return BYTE;
		default: return 0;
	}
}

DataType *dataType_smallestInt (size_t value)
{
	size_t numBits = 0;
	for (size_t n = value; n != 0; n >>= 1, numBits++);
	if (numBits <= 8 * BYTE) return dataType_initI8();
	return NULL;
}

DataType *dataType_larger (DataType *a, DataType *b)
{
	return dataType_getSize(a) > dataType_getSize(b) ? a : b;
}

