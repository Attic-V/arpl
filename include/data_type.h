#ifndef data_type_h
#define data_type_h

#include <stdbool.h>

#include "token.h"

typedef struct DataType DataType;

typedef struct {
	size_t length;
	DataType *type;
} DataTypeArray;

typedef struct {
} DataTypeBoolean;

typedef struct {
} DataTypeI32;

typedef struct {
	DataType *to;
} DataTypePointer;

typedef enum {
	DataType_Array,
	DataType_Boolean,
	DataType_I32,
	DataType_Pointer,
} DataTypeType;

struct DataType {
	DataTypeType type;
	union {
		DataTypeArray *array;
		DataTypeBoolean *boolean;
		DataTypeI32 *i32;
		DataTypePointer *pointer;
	} as;
	bool mutable;
};

DataType *dataType_initArray (size_t length, DataType *elementT);
DataType *dataType_initBoolean (void);
DataType *dataType_initI32 (void);
DataType *dataType_initPointer (DataType *to);

bool dataType_isArray (DataType *t);
bool dataType_isBoolean (DataType *t);
bool dataType_isI32 (DataType *t);
bool dataType_isPointer (DataType *t);

bool dataType_equal (DataType *a, DataType *b);
size_t dataType_getSize (DataType *type);

#endif
