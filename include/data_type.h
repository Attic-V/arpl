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
} DataTypeNumber;

typedef struct {
	DataType *to;
} DataTypePointer;

typedef enum {
	DataType_Array,
	DataType_Boolean,
	DataType_Number,
	DataType_Pointer,
} DataTypeType;

struct DataType {
	DataTypeType type;
	union {
		DataTypeArray *array;
		DataTypeBoolean *boolean;
		DataTypeNumber *number;
		DataTypePointer *pointer;
	} as;
};

DataType *dataType_initArray (size_t length, DataType *elementT);
DataType *dataType_initBoolean (void);
DataType *dataType_initNumber (void);
DataType *dataType_initPointer (DataType *to);

bool dataType_isArray (DataType *t);
bool dataType_isBoolean (DataType *t);
bool dataType_isNumber (DataType *t);
bool dataType_isPointer (DataType *t);

bool dataType_equal (DataType *a, DataType *b);
size_t dataType_getSize (DataType *type);

#endif
