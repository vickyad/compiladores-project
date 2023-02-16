#include "free.h"

void freeTableValueArguments(FunctionArgument* argument) 
{
    if (argument == NULL) return;

    freeTableValueArguments(argument->nextArgument);

    free(argument);
}

void freeTableValue(SymbolTableValue value)
{
    freeTableValueArguments(value.arguments);
}

void freeTableEntry(SymbolTableEntry entry) 
{
    freeTableValue(entry.value);
    free(entry.key);
}

void freeTableEntries(SymbolTable* table) 
{
    for (size_t index = 0; index < table->capacity; index++) 
    {
        freeTableEntry(table->entries[index]);
    }
    free(table->entries);
}

void freeSymbolTable(SymbolTable* table) 
{
    if (!table) return;

    freeTableEntries(table);

    free(table);
}

void freeSymbolTableStack(SymbolTableStack* symbolTableStack)
{
    if (!symbolTableStack) return;

    freeSymbolTableStack(symbolTableStack->nextItem);

    freeSymbolTable(symbolTableStack->symbolTable);

    free(symbolTableStack);
}

SymbolTableStack* freeFirstTableFromSymbolTableStack(SymbolTableStack* symbolTableStack)
{
    if (!symbolTableStack) return NULL;

    freeSymbolTable(symbolTableStack->symbolTable);

    SymbolTableStack* nextItem = symbolTableStack->nextItem;

    free(symbolTableStack);

    return nextItem;
}

void freeLexicalValue(LexicalValue lexicalValue)
{
    if (!lexicalValue.label) return;
    free(lexicalValue.label);
}

void freeTree(Node* node)
{
    if (!node) return;
    
    freeLexicalValue(node->lexicalValue);

    Node* child = node->child;
    freeTree(child);

    Node* brother = node->brother;
    freeTree(brother);

    free(node);
}

void freeGlobalTree() 
{
  freeTree(tree);
  tree = NULL;
}

void freeGlobalSymbolTableStack() 
{
  freeSymbolTableStack(symbolTableStack);
  symbolTableStack = NULL;
}

void freeGlobalVariables()
{
  freeGlobalTree();
  freeGlobalSymbolTableStack();
}