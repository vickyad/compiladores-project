#include "lexical_value.h"

LexicalValue createLexicalValue(char* text, TokenType type, LiteralType literalType, int lineNumber)
{
    LexicalValue lexicalValue;
    lexicalValue.lineNumber = lineNumber;
    lexicalValue.type = type;
    lexicalValue.label = strdup(text);
    
    if (literalType != NULL) 
    {
        defineLiteralValue(&lexicalValue, text, literalType);
    }
    else
    {
        lexicalValue.literalType = NULL;
    }
}

void defineLiteralValue(LexicalValue* lexicalValue, char* text, LiteralType literalType) {
    if (literalType == INT)
    {
        lexicalValue->value.value_int = atoi(text);
    }
    else if (literalType == FLOAT)
    {
        lexicalValue->value.value_float = atof(text);
    }
    else if (literalType == CHAR)
    {
        lexicalValue->value.value_char = text[1];
    }
    else if (literalType == BOOL)
    {
        lexicalValue->value.value_bool = isTextEqualTrue(text);
    }
}

int isTextEqualTrue(char* text)
{
    return strncmp(text, "true", 4) == 0;
}

char* getLexicalValueLabel(LexicalValue lexicalValue) 
{
    if (lexicalValue.type == SPECIAL_CHARACTER || lexicalValue.type == RESERVED_WORD 
        || lexicalValue.type == COMPOUND_OPERATOR || lexicalValue.type == IDENTIFIER
        || lexicalValue.literalType == CHAR) 
    {
        return lexicalValue.value.value_char;
    } 
    else if (lexicalValue.literalType == INT)
    {
        return getIntFromLexicalValue(lexicalValue);
    }
    else if (lexicalValue.literalType == FLOAT)
    {
        return getFloatFromLexicalValue(lexicalValue);
    }
    else if (lexicalValue.literalType == BOOL)
    {
        return getBooleanFromLexicalValue(lexicalValue);
    }
}

char* getIntFromLexicalValue(LexicalValue lexicalValue) 
{
    char stringInteger[10];
    itoa(lexicalValue.value.value_int, stringInteger, 10);
    return stringInteger;
}

char* getFloatFromLexicalValue(LexicalValue lexicalValue) 
{
    char stringFloat[48];
    snprintf(stringFloat, sizeof(stringFloat), "%f", lexicalValue.value.value_float);
    return stringFloat;
}

char* getBooleanFromLexicalValue(LexicalValue lexicalValue)
{
    if (lexicalValue.value.value_bool == 1) 
    {
        return "true";
    } 
    else 
    {
        return "false";
    }
}
