#ifndef SYMBOL_TABLE_HEADER
#define SYMBOL_TABLE_HEADER

#include "lexical_value.h"
#include "print.h"
#include <stdio.h>

#define SYMBOL_TABLE_INITIAL_CAPACITY 8
#define TABLE_EXPAND_FACTOR 2
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

typedef struct FunctionArgument {
    LiteralType type;
    struct FunctionArgument* nextArgument;
} FunctionArgument;

typedef enum SymbolType {
    SYMBOL_TYPE_LITERAL,
    SYMBOL_TYPE_VARIABLE,
    SYMBOL_TYPE_ARRAY,
    SYMBOL_TYPE_FUNCTION,
    SYMBOL_TYPE_NON_EXISTENT
} SymbolType;

typedef struct SymbolTableValue {
    int size;
    SymbolType symbolType;
    LexicalValue lexicalValue;
    FunctionArgument* firstArgument;
} SymbolTableValue;

typedef struct SymbolTableEntry {
    char* key;
    SymbolTableValue value;
} SymbolTableEntry;

typedef struct SymbolTable {
    int size;
    int capacity;
    SymbolTableEntry* entries;
} SymbolTable;

SymbolTable* createTable();

void destroyTable(SymbolTable* table);

SymbolTableValue getValue(SymbolTable* table, char* key);

SymbolTableValue createTableValue(SymbolType symbolType, LexicalValue lexicalValue);

void addSymbol(SymbolTable* table, SymbolTableValue value);

int expandTable(SymbolTable* table);

#endif