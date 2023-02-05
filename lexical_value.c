#include "lexical_value.h"

LexicalValue createLexicalValue(char* text, TokenType type, LiteralType literalType, int lineNumber)
{
    LexicalValue lexicalValue;
    lexicalValue.lineNumber = lineNumber;
    lexicalValue.type = type;
    lexicalValue.label = strdup(getLabelName(text));
    
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

char* getLabelName(char* text) {
    if (strcmp(text, "[") || strcmp(text, "]")) {
        return "[]";
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

void freeLexicalValue(LexicalValue lexicalValue)
{
    if (!lexicalValue.label) return;
    free(lexicalValue.label);
}