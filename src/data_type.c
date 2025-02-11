#include "data_type.h"

DataType getTypeFromKey (Token keyword)
{
	switch (keyword.type) {
		case TT_Bool: return DT_Boolean;
		case TT_Int: return DT_Number;
		default:
	}
	return -1;
}

size_t getDtSize (DataType type)
{
	switch (type) {
		case DT_Boolean: return 1;
		case DT_Number: return 4;
	}
	return -1;
}
