#ifndef data_type_h
#define data_type_h

#include <stdbool.h>

#include "token.h"

typedef struct DataType DataType;

typedef struct {
	DataType *returnType;
	DataType *parameters;
} DataTypeFunction;

typedef struct {
	char _unused;
} DataTypeI8;

typedef enum {
	DataType_Function,
	DataType_I8,

	DataTypeType_Count,
} DataTypeType;

struct DataType {
	DataTypeType type;
	union {
		DataTypeFunction *function;
		DataTypeI8 *i8;
	} as;
	DataType *next;
	DataType *previous;
};

DataType *dataType_initFunction (DataType *returnType, DataType *parameters);
DataType *dataType_initI8 (void);

DataType *dataType_smallestInt (size_t value);

bool dataType_isFunction (DataType *t);
bool dataType_isI8 (DataType *t);

bool dataType_isInt (DataType *t);

bool dataType_castable (DataType *from, DataType *to);
bool dataType_equal (DataType *a, DataType *b);
size_t dataType_getSize (DataType *type);
DataType *dataType_larger (DataType *a, DataType *b);

#endif

