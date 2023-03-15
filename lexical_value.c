#include "lexical_value.h"

LexicalValue createLexicalValue(char* text, TokenType type, LiteralType literalType, int lineNumber)
{
    LexicalValue lexicalValue;
    lexicalValue.lineNumber = lineNumber;
    lexicalValue.type = type;
    lexicalValue.label = getLabelName(text, literalType);
    lexicalValue.functionLabel = -1;
    
    if (literalType != LITERAL_TYPE_IS_NOT_LITERAL) 
    {
        defineLiteralValue(&lexicalValue, text, literalType);
    }

    return lexicalValue;
}

char* getLabelName(char* text, LiteralType literalType) {
    if (!strcmp(text, "[") || !strcmp(text, "]")) 
    {
        return strdup("[]");
    }
    if (literalType && literalType == LITERAL_TYPE_CHAR)
    {        
        char* label = malloc(2 * sizeof(char));
        label[0] = text[1];
        label[1] = '\0';
        return label;
    }
    return strdup(text);
}

void defineLiteralValue(LexicalValue* lexicalValue, char* text, LiteralType literalType) {
    if (literalType == LITERAL_TYPE_INT)
    {
        lexicalValue->value.value_int = atoi(text);
    }
    else if (literalType == LITERAL_TYPE_FLOAT)
    {
        lexicalValue->value.value_float = atof(text);
    }
    else if (literalType == LITERAL_TYPE_CHAR)
    {
        lexicalValue->value.value_char = text[1];
    }
    else if (literalType == LITERAL_TYPE_BOOL)
    {
        lexicalValue->value.value_bool = isTextEqualTrue(text);
    }
}

int isTextEqualTrue(char* text)
{
    return strncmp(text, "true", 4) == 0;
}

Dimension getDimension(LexicalValue lexicalValue)
{
    Dimension dimension;
    dimension.quantity = 0;
    if (lexicalValue.type != TOKEN_TYPE_LITERAL && lexicalValue.literalType != LITERAL_TYPE_INT) 
    {
        printf("Calculating dimension with value different than integer");
        dimension.size = 0;
        return dimension;
    } 

    dimension.size = lexicalValue.value.value_int;

    return dimension;
}

Dimension getDimensionMultipling(LexicalValue lexicalValue, Dimension previousDimension)
{
    Dimension dimensionFromLexicalValue = getDimension(lexicalValue);
    Dimension newDimension;
    newDimension.size = dimensionFromLexicalValue.size * previousDimension.size;
    newDimension.quantity = previousDimension.quantity + 1;
    return newDimension;
}

LexicalValue copyLexicalValue(LexicalValue lexicalValue)
{
    LexicalValue copy;
    copy.label = malloc(strlen(lexicalValue.label) + 1);
    strcpy(copy.label, lexicalValue.label);
    copy.lineNumber = lexicalValue.lineNumber;
    copy.literalType = lexicalValue.literalType;
    copy.type = lexicalValue.type;
    copy.value = lexicalValue.value;
    copy.functionLabel = lexicalValue.functionLabel;
    return copy;
}