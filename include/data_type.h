#ifndef data_type_h
#define data_type_h

#include <stdbool.h>

#include "token.h"

typedef struct DataType DataType;

typedef struct {
	size_t length;
	DataType *type;
} DataTypeArray;

typedef enum {
	Primitive_Boolean,
	Primitive_Number,
} Primitive;

typedef struct {
	Primitive type;
} DataTypePrimitive;

typedef enum {
	DataType_Array,
	DataType_Primitive,
} DataTypeType;

struct DataType {
	DataTypeType type;
	union {
		DataTypeArray *array;
		DataTypePrimitive *primitive;
	} as;
};

DataType *dataType_initArray (size_t length, DataType *elementT);
DataType *dataType_initBoolean (void);
DataType *dataType_initNumber (void);

bool dataType_equal (DataType *a, DataType *b);
bool dataType_isPrimitive (DataType *t);
bool dataType_isArray (DataType *t);
bool dataType_isBoolean (DataType *t);
bool dataType_isNumber (DataType *t);
size_t dataType_getSize (DataType *type);

#endif
