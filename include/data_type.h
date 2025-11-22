#ifndef data_type_h
#define data_type_h

#include <stdbool.h>

#include "token.h"

typedef struct DataType DataType;

typedef struct {
	char _unused;
} DataTypeBoolean;

typedef struct {
	DataType *returnType;
	DataType *parameters;
} DataTypeFunction;

typedef struct {
	char _unused;
} DataTypeI8;

typedef struct DataTypeMember DataTypeMember;

typedef struct {
	Token identifier;
	DataTypeMember *members;
} DataTypeStruct;

struct DataTypeMember {
	Token identifier;
	DataType *dataType;
	DataTypeMember *next;
	DataTypeMember *previous;
	size_t index;
};

typedef enum {
	DataType_Boolean,
	DataType_Function,
	DataType_I8,
	DataType_Struct,

	DataTypeType_Count,
} DataTypeType;

struct DataType {
	DataTypeType type;
	union {
		DataTypeBoolean *boolean;
		DataTypeFunction *function;
		DataTypeI8 *i8;
		DataTypeStruct *struct_;
	} as;
	DataType *next;
	DataType *previous;
};

DataType *dataType_initBoolean (void);
DataType *dataType_initFunction (DataType *returnType, DataType *parameters);
DataType *dataType_initI8 (void);

DataTypeMember *dataType_initMember (Token identifier, DataType *dataType, size_t index);

DataType *dataType_smallestInt (size_t value);

bool dataType_isBoolean (DataType *t);
bool dataType_isFunction (DataType *t);
bool dataType_isI8 (DataType *t);

bool dataType_isInt (DataType *t);

bool dataType_castable (DataType *from, DataType *to);
bool dataType_equal (DataType *a, DataType *b);
size_t dataType_getSize (DataType *type);
DataType *dataType_larger (DataType *a, DataType *b);

#endif

