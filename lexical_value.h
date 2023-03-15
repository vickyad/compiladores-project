#ifndef LEXICAL_VALUE_HEADER
#define LEXICAL_VALUE_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "print.h"
#include "types.h"

LexicalValue createLexicalValue(char* text, TokenType type, LiteralType literalType, int lineNumber);
void defineLiteralValue(LexicalValue* lexicalValue, char* text, LiteralType literalType);
int isTextEqualTrue(char* text);
char* getLabelName(char* text, LiteralType literalType);
Dimension getDimension(LexicalValue lexicalValue);
Dimension getDimensionMultipling(LexicalValue lexicalValue, Dimension dimension);
LexicalValue copyLexicalValue(LexicalValue lexicalValue);

#endif