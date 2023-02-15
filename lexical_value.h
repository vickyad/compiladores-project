#ifndef LEXICAL_VALUE_HEADER
#define LEXICAL_VALUE_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "print.h"

typedef enum TokenType {
    TOKEN_TYPE_SPECIAL_CHARACTER,
    TOKEN_TYPE_RESERVED_WORD,
    TOKEN_TYPE_COMPOUND_OPERATOR,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_LITERAL
} TokenType;

typedef enum LiteralType {
    LITERAL_TYPE_INT,
    LITERAL_TYPE_FLOAT,
    LITERAL_TYPE_CHAR,
    LITERAL_TYPE_BOOL,
    LITERAL_TYPE_IS_NOT_LITERAL
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

LexicalValue createLexicalValue(char* text, TokenType type, LiteralType literalType, int lineNumber);
void defineLiteralValue(LexicalValue* lexicalValue, char* text, LiteralType literalType);
int isTextEqualTrue(char* text);
void freeLexicalValue(LexicalValue lexicalValue);
char* getLabelName(char* text, LiteralType literalType);

#endif