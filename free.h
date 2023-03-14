#ifndef FREE_HEADER
#define FREE_HEADER

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "types.h"

void freeLexicalValue(LexicalValue lexicalValue);

void freeTree(Node* node);

void freeTableEntries(SymbolTable* table);

void freeSymbolTable(SymbolTable* table);

void freeSymbolTableStack(SymbolTableStack* symbolTableStack);

void freeTableValueArguments(FunctionArgument* argument);

SymbolTableStack* freeFirstTableFromSymbolTableStack(SymbolTableStack* symbolTableStack);

void freeGlobalVariables();

void freeIlocList(IlocOperationList* list);

#endif