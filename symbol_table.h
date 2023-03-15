#ifndef SYMBOL_TABLE_HEADER
#define SYMBOL_TABLE_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "free.h"
#include "types.h"
#include "print.h"
#include "errors.h"
#include "data_type.h"
#include "lexical_value.h"

#define SYMBOL_TABLE_INITIAL_CAPACITY 32
#define TABLE_EXPAND_FACTOR 2
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

void initGlobalSymbolStack();
char* getSymbolTypeName(SymbolType symbolType);
SymbolTable* createSymbolTable();
SymbolTableValue getEmptySymbolValue();
SymbolTableValue getSymbolTableValueByKey(SymbolTable* table, char* key);
SymbolTableValue createSymbolTableValue(SymbolType symbolType, Node* node);
SymbolTableValue createSymbolTableValueWithType(SymbolType symbolType, LexicalValue lexicalValue, DataType dataType);
SymbolTableValue createSymbolTableValueWithTypeAndDimension(SymbolType symbolType, LexicalValue lexicalValue, DataType dataType, Dimension dimension);
SymbolTableValue createSymbolTableValueWithTypeAndArguments(SymbolType symbolType, LexicalValue lexicalValue, DataType dataType, FunctionArgument* arguments, int functionLabel);
int expandSymbolTable(SymbolTable* table);
SymbolTableStack* createSymbolTableStack();
void addBlankSpaceToSymbolTableStack(SymbolTableStack* stack);
SymbolTableValue addValueToSymbolTableStack(SymbolTableStack* stack, SymbolTableValue value);
SymbolTableValue addValueToSecondSymbolTableOnStack(SymbolTableStack* stack, SymbolTableValue value);
SymbolTable* getFirstTableFromSymbolTableStack(SymbolTableStack* symbolTableStack);
SymbolTableStack* createNewTableOnSymbolTableStack(SymbolTableStack* symbolTableStack);
SymbolTableStack* addTableToSymbolTableStack(SymbolTableStack* symbolTableStack, SymbolTable* symbolTable);
SymbolTableValue getByLexicalValueOnSymbolTableStack(SymbolTableStack* symbolTableStack, LexicalValue lexicalValue);
void updateFunctionLastPosition(SymbolTableStack* symbolTableStack, LexicalValue lexicalValue, int functionLastPosition);
SymbolTableStack* getStackByLexicalValue(SymbolTableStack* symbolTableStack, LexicalValue lexicalValue);
int checkValueIsOnFirstSymbolTable(SymbolTableStack* symbolTableStack, char* key);
FunctionArgument* createFunctionArgument(LexicalValue lexicalValue, DataType dataType);
FunctionArgument* addFunctionArgument(FunctionArgument* functionArgument, LexicalValue lexicalValue, DataType dataType);
void validateFunctionCall(SymbolTableValue symbol, LexicalValue lexicalValue);
void validateArrayUse(SymbolTableValue symbol, LexicalValue lexicalValue);
void validateVariableUse(SymbolTableValue symbol, LexicalValue lexicalValue);
void validateArrayDeclaration(SymbolTableValue symbol);
void printSymbolTableStack(SymbolTableStack* symbolTableStack);

#endif
