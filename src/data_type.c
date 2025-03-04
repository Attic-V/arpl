#include "data.h"
#include "data_type.h"
#include "memory.h"

DataTypeArray *dataTypeArray_init (size_t length, DataType *elementT);
DataTypeBoolean *dataTypeBoolean_init (void);
DataTypeNumber *dataTypeNumber_init (void);
DataTypePointer *dataTypePointer_init (DataType *to);

DataType *dataType_initArray (size_t length, DataType *elementT)
{
	DataType *t = mem_alloc(sizeof(*t));
	t->type = DataType_Array;
	t->as.array = dataTypeArray_init(length, elementT);
	return t;
}

DataTypeArray *dataTypeArray_init (size_t length, DataType *elementT)
{
	DataTypeArray *t = mem_alloc(sizeof(*t));
	t->length = length;
	t->type = elementT;
	return t;
}

DataType *dataType_initBoolean (void)
{
	DataType *t = mem_alloc(sizeof(*t));
	t->type = DataType_Boolean;
	t->as.boolean = dataTypeBoolean_init();
	return t;
}

DataTypeBoolean *dataTypeBoolean_init (void)
{
	DataTypeBoolean *t = mem_alloc(sizeof(*t));
	return t;
}

DataType *dataType_initNumber (void)
{
	DataType *t = mem_alloc(sizeof(*t));
	t->type = DataType_Number;
	t->as.number = dataTypeNumber_init();
	return t;
}

DataTypeNumber *dataTypeNumber_init (void)
{
	DataTypeNumber *t = mem_alloc(sizeof(*t));
	return t;
}

DataType *dataType_initPointer (DataType *to)
{
	DataType *t = mem_alloc(sizeof(*t));
	t->type = DataType_Pointer;
	t->as.pointer = dataTypePointer_init(to);
	return t;
}

DataTypePointer *dataTypePointer_init (DataType *to)
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

bool dataType_isNumber (DataType *t)
{
	return t->type == DataType_Number;
}

bool dataType_isPointer (DataType *t)
{
	return t->type == DataType_Pointer;
}

bool dataType_equal (DataType *a, DataType *b)
{
	if (a->type != b->type) {
		return false;
	}
	switch (a->type) {
		case DataType_Array: return dataType_equal(a->as.array->type, b->as.array->type);
		case DataType_Boolean: return true;
		case DataType_Number: return true;
		case DataType_Pointer: return dataType_equal(a->as.pointer->to, b->as.pointer->to);
	}
	return false;
}

size_t dataType_getSize (DataType *t)
{
	switch (t->type) {
		case DataType_Array: return t->as.array->length * dataType_getSize(t->as.array->type);
		case DataType_Boolean: return BYTE;
		case DataType_Number: return DWORD;
		case DataType_Pointer: return QWORD;
	}
	return -1;
}
