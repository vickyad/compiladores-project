#ifndef LEXICAL_VALUE_HEADER
#define LEXICAL_VALUE_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "debug.h"

typedef enum TokenType {
    SPECIAL_CHARACTER,
    RESERVED_WORD,
    COMPOUND_OPERATOR,
    IDENTIFIER,
    LITERAL
} TokenType;

typedef enum LiteralType {
    INT,
    FLOAT,
    CHAR,
    BOOL,
    IS_NOT_LITERAL
} LiteralType;

typedef struct LexicalValue {
    int lineNumber;
    TokenType type;
    LiteralType literalType;
    
    union value {
        int value_int;
        float value_float;
        char value_char;
        int value_bool;
    } value;
    char* label;
} LexicalValue;

char* getLexicalValueLabel(LexicalValue value);
LexicalValue createLexicalValue(char* text, TokenType type, LiteralType literalType, int lineNumber);
void defineLiteralValue(LexicalValue* lexicalValue, char* text, LiteralType literalType);
int isTextEqualTrue(char* text);
void freeLexicalValue(LexicalValue lexicalValue);
char* getLabelName(char* text);

#endif