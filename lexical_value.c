#include "lexical_value.h"

LexicalValue createLexicalValue(char* text, TokenType type, LiteralType literalType, int lineNumber)
{
    LexicalValue lexicalValue;
    lexicalValue.lineNumber = lineNumber;
    lexicalValue.type = type;
    lexicalValue.label = strdup(text);
    
    if (literalType) 
    {
        defineLiteralValue(&lexicalValue, text, literalType);
    }
    else
    {
        lexicalValue.literalType = IS_NOT_LITERAL;
    }

    return lexicalValue;
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
        return getCharFromLexicalValue(lexicalValue);
    }
    else if (lexicalValue.literalType == INT)
    {
        return getIntFromLexicalValue(lexicalValue);
    }
    else if (lexicalValue.literalType == FLOAT)
    {
        return getFloatFromLexicalValue(lexicalValue);
    }
    else
    {
        return getBooleanFromLexicalValue(lexicalValue);
    }
}

char* getIntFromLexicalValue(LexicalValue lexicalValue) 
{
    char* stringInteger = malloc(10 * sizeof(char));
    snprintf(stringInteger, sizeof(stringInteger), "%d", lexicalValue.value.value_int);
    return stringInteger;
}

char* getCharFromLexicalValue(LexicalValue lexicalValue) 
{
    char* stringChar = malloc(48 * sizeof(char));
    snprintf(stringChar, sizeof(stringChar), "%c", lexicalValue.value.value_char);
    return stringChar;
}

char* getFloatFromLexicalValue(LexicalValue lexicalValue) 
{
    char* stringFloat = malloc(48 * sizeof(char));
    snprintf(stringFloat, sizeof(stringFloat), "%f", lexicalValue.value.value_float);
    return stringFloat;
}

char* getBooleanFromLexicalValue(LexicalValue lexicalValue)
{
    char* stringBoolean = malloc(5 * sizeof(char));
    if (lexicalValue.value.value_bool == 1) 
    {
        snprintf(stringBoolean, sizeof(stringBoolean), "%s", "true");
    } 
    else 
    {
        snprintf(stringBoolean, sizeof(stringBoolean), "%s", "false");
    }
    return stringBoolean;
}
