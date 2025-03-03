#include "data.h"
#include "data_type.h"
#include "memory.h"

DataTypeArray *dataTypeArray_init (size_t length, DataType *elementT);
DataTypePointer *dataTypePointer_init (DataType *to);
DataTypePrimitive *dataTypePrimitive_init (Primitive pType);

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

DataType *dataType_initBoolean (void)
{
	DataType *t = mem_alloc(sizeof(*t));
	t->type = DataType_Primitive;
	t->as.primitive = dataTypePrimitive_init(Primitive_Boolean);
	return t;
}

DataType *dataType_initNumber (void)
{
	DataType *t = mem_alloc(sizeof(*t));
	t->type = DataType_Primitive;
	t->as.primitive = dataTypePrimitive_init(Primitive_Number);
	return t;
}

DataTypePrimitive *dataTypePrimitive_init (Primitive pType)
{
	DataTypePrimitive *t = mem_alloc(sizeof(*t));
	t->type = pType;
	return t;
}

bool dataType_equal (DataType *a, DataType *b)
{
	if (a->type != b->type) {
		return false;
	}
	if (a->type == DataType_Array) {
		return dataType_equal(a->as.array->type, b->as.array->type);
	}
	if (a->type == DataType_Pointer) {
		return dataType_equal(a->as.pointer->to, b->as.pointer->to);
	}
	if (a->type == DataType_Primitive) {
		return a->as.primitive->type == b->as.primitive->type;
	}
	return false;
}

bool dataType_isPrimitive (DataType *t)
{
	return t->type == DataType_Primitive;
}

bool dataType_isArray (DataType *t)
{
	return t->type == DataType_Array;
}

bool dataType_isBoolean (DataType *t)
{
	return dataType_isPrimitive(t) && t->as.primitive->type == Primitive_Boolean;
}

bool dataType_isNumber (DataType *t)
{
	return dataType_isPrimitive(t) && t->as.primitive->type == Primitive_Number;
}

bool dataType_isPointer (DataType *t)
{
	return t->type == DataType_Pointer;
}

size_t dataType_getSize (DataType *t)
{
	if (dataType_isPrimitive(t)) {
		if (dataType_isBoolean(t)) return BYTE;
		if (dataType_isNumber(t)) return DWORD;
	}
	if (dataType_isArray(t)) {
		return t->as.array->length * dataType_getSize(t->as.array->type);
	}
	if (dataType_isPointer(t)) {
		return QWORD;
	}
	return -1;
}
