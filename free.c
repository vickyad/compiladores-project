#include "free.h"

extern Node* tree;
extern SymbolTableStack* symbolTableStack;
extern int yylex_destroy(void);

void freeTableValueArguments(FunctionArgument* argument) 
{
    if (!argument) return;

    freeTableValueArguments(argument->nextArgument);

    freeLexicalValue(argument->lexicalValue);
    free(argument);

    argument = NULL;
}

void freeTableValue(SymbolTableValue value)
{
    freeTableValueArguments(value.arguments);
    freeLexicalValue(value.lexicalValue);
}

void freeTableEntry(SymbolTableEntry entry) 
{
    freeTableValue(entry.value);
    if (entry.key) {
        free(entry.key);
        entry.key = NULL;
    }
}

void freeTableEntries(SymbolTable* table) 
{
    for (size_t index = 0; index < table->capacity; index++) 
    {
        freeTableEntry(table->entries[index]);
    }
    free(table->entries);
    table->entries = NULL;
}

void freeSymbolTable(SymbolTable* table) 
{
    if (!table) return;

    freeTableEntries(table);

    free(table);
    table = NULL;
}

void freeSymbolTableStack(SymbolTableStack* symbolTableStack)
{
    if (!symbolTableStack) return;

    freeSymbolTableStack(symbolTableStack->nextItem);

    freeSymbolTable(symbolTableStack->symbolTable);

    free(symbolTableStack);
    symbolTableStack = NULL;
}

SymbolTableStack* freeFirstTableFromSymbolTableStack(SymbolTableStack* symbolTableStack)
{
    if (!symbolTableStack) return NULL;

    freeSymbolTable(symbolTableStack->symbolTable);

    SymbolTableStack* nextItem = symbolTableStack->nextItem;

    free(symbolTableStack);
    symbolTableStack = NULL;

    return nextItem;
}

void freeLexicalValue(LexicalValue lexicalValue)
{
    if (!lexicalValue.label) return;

    free(lexicalValue.label);
    
    lexicalValue.label = NULL;
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
    node = NULL;
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

void freeIlocList(IlocOperationList* list)
{
    if (!list) return;
    
    IlocOperationList* child = list->nextItem;
    freeIlocList(child);

    free(list);
    list = NULL;
}

void freeGlobalVariables()
{
  freeGlobalTree();
  freeGlobalSymbolTableStack();
  yylex_destroy();
}

