#include "lexical_value.h"

LexicalValue createLexicalValue(char* text, TokenType type, LiteralType literalType, int lineNumber)
{
    LexicalValue lexicalValue;
    lexicalValue.lineNumber = lineNumber;
    lexicalValue.type = type;
    lexicalValue.label = getLabelName(text, literalType);
    
    if (literalType) 
    {
        defineLiteralValue(&lexicalValue, text, literalType);
    }
    else
    {
        lexicalValue.literalType = LITERAL_TYPE_IS_NOT_LITERAL;
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

void freeLexicalValue(LexicalValue lexicalValue)
{
    if (!lexicalValue.label) return;
    free(lexicalValue.label);
}