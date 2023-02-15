#include "data_type.h"

char* getDataTypeName(DataType dataType) 
{
    if (dataType == DATA_TYPE_FLOAT) return "float";
    if (dataType == DATA_TYPE_INT) return "int";
    if (dataType == DATA_TYPE_BOOL) return "boolean";
    if (dataType == DATA_TYPE_CHAR) return "char";
    return "non declared";
}
