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
