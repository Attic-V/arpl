#include "data.h"
#include "data_type.h"
#include "memory.h"

static DataType *dataType_init (DataTypeType type);

static DataTypeArray *dataTypeArray_init (size_t length, DataType *elementT);
static DataTypeBoolean *dataTypeBoolean_init (void);
static DataTypeI32 *dataTypeI32_init (void);
static DataTypePointer *dataTypePointer_init (DataType *to);

static DataType *dataType_init (DataTypeType type)
{
	DataType *t = mem_alloc(sizeof(*t));
	t->type = type;
	t->mutable = true;
	return t;
}

DataType *dataType_initArray (size_t length, DataType *elementT)
{
	DataType *t = dataType_init(DataType_Array);
	t->as.array = dataTypeArray_init(length, elementT);
	return t;
}

static DataTypeArray *dataTypeArray_init (size_t length, DataType *elementT)
{
	DataTypeArray *t = mem_alloc(sizeof(*t));
	t->length = length;
	t->type = elementT;
	return t;
}

DataType *dataType_initBoolean (void)
{
	DataType *t = dataType_init(DataType_Boolean);
	t->as.boolean = dataTypeBoolean_init();
	return t;
}

static DataTypeBoolean *dataTypeBoolean_init (void)
{
	DataTypeBoolean *t = mem_alloc(sizeof(*t));
	return t;
}

DataType *dataType_initI32 (void)
{
	DataType *t = dataType_init(DataType_I32);
	t->as.i32 = dataTypeI32_init();
	return t;
}

static DataTypeI32 *dataTypeI32_init (void)
{
	DataTypeI32 *t = mem_alloc(sizeof(*t));
	return t;
}

DataType *dataType_initPointer (DataType *to)
{
	DataType *t = dataType_init(DataType_Pointer);
	t->as.pointer = dataTypePointer_init(to);
	return t;
}

static DataTypePointer *dataTypePointer_init (DataType *to)
{
	DataTypePointer *t = mem_alloc(sizeof(*t));
	t->to = to;
	return t;
}

bool dataType_isArray (DataType *t)
{
	return t->type == DataType_Array;
}

bool dataType_isBoolean (DataType *t)
{
	return t->type == DataType_Boolean;
}

bool dataType_isI32 (DataType *t)
{
	return t->type == DataType_I32;
}

bool dataType_isPointer (DataType *t)
{
	return t->type == DataType_Pointer;
}

bool dataType_equal (DataType *a, DataType *b)
{
	if (a->mutable != b->mutable) {
		return false;
	}
	if (a->type != b->type) {
		return false;
	}
	switch (a->type) {
		case DataType_Array: return dataType_equal(a->as.array->type, b->as.array->type);
		case DataType_Boolean: return true;
		case DataType_I32: return true;
		case DataType_Pointer: return dataType_equal(a->as.pointer->to, b->as.pointer->to);
	}
	return false;
}

size_t dataType_getSize (DataType *t)
{
	switch (t->type) {
		case DataType_Array: return t->as.array->length * dataType_getSize(t->as.array->type);
		case DataType_Boolean: return BYTE;
		case DataType_I32: return DWORD;
		case DataType_Pointer: return QWORD;
	}
	return -1;
}
