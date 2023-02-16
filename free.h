#ifndef FREE_HEADER
#define FREE_HEADER

#include <stdlib.h>
#include "types.h"
#include "global_declarations.h"

void freeTree(Node* node);

void freeTableEntries(SymbolTable* table);

void freeSymbolTable(SymbolTable* table);

void freeSymbolTableStack(SymbolTableStack* symbolTableStack);

SymbolTableStack* freeFirstTableFromSymbolTableStack(SymbolTableStack* symbolTableStack);

void freeGlobalVariables();

#endif