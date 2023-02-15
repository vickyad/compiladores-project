#include "symbol_table.h"

SymbolTable* createSymbolTable() 
{
    SymbolTable* table = malloc(sizeof(SymbolTable));
    if (!table) 
    {
        printError("[SymbolTable] Fail to create symbol table!");
        return NULL;
    }
    table->capacity = SYMBOL_TABLE_INITIAL_CAPACITY;
    table->size = 0;
    table->entries = calloc(table->capacity, sizeof(SymbolTableEntry));
    if (!table->entries) 
    {
        destroySymbolTable(table);
        printError("[SymbolTable] Fail to initialize entries for symbol table!");
        return NULL;
    }
    return table;
}

void destroyTableValueArguments(FunctionArgument* argument) 
{
    if (argument == NULL) return;

    destroyTableValueArguments(argument->nextArgument);

    free(argument);
}

void destroyTableValue(SymbolTableValue value)
{
    destroyTableValueArguments(value.firstArgument);
}

void destroyTableEntry(SymbolTableEntry entry) 
{
    destroyTableValue(entry.value);
    free(entry.key);
}

void destroyTableEntries(SymbolTable* table) 
{
    for (size_t index = 0; index < table->capacity; index++) 
    {
        destroyTableEntry(table->entries[index]);
    }
    free(table->entries);
}

void destroySymbolTable(SymbolTable* table) 
{
    if (!table) return;

    destroyTableEntries(table);

    free(table);
}

// Fowler–Noll–Vo-1a hash: https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function

size_t getIndex(int capacity, char* key) 
{
    uint64_t hash = FNV_OFFSET;
    for (char* p = key; *p; p++) 
    {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return (size_t)(hash & (uint64_t)(capacity - 1));
}

int isSameKey(SymbolTableEntry entry, char* key) 
{
    return strcmp(key, entry.key) == 0;
}

SymbolTableValue getEmptyValue()
{
    SymbolTableValue value;
    value.symbolType = SYMBOL_TYPE_NON_EXISTENT;
    return value;
}

SymbolTableValue getSymbolTableValueByKey(SymbolTable* table, char* key) 
{
    if (!table) return getEmptyValue();

    size_t index = getIndex(table->capacity, key);

    SymbolTableEntry possibleEntry = table->entries[index];
    while (possibleEntry.key) {
        if (isSameKey(possibleEntry, key)) 
        {
            return possibleEntry.value;
        }
        
        index++;
        if (index >= table->capacity) 
        {
            index = 0;
        }
        possibleEntry = table->entries[index];
    }

    return getEmptyValue();
}

void addEntryOnList(SymbolTableEntry* entries, int capacity, char* key, int* size, SymbolTableValue value)
{
    if (!key) {
        printError("[SymbolTable] Trying to add value with empty key");
        return;
    }
    size_t index = getIndex(capacity, key);

    SymbolTableEntry possibleAlreadyExistentEntry = entries[index];
    while (possibleAlreadyExistentEntry.key) {
        printDebug("Colision between: ");
        printDebug(key);
        printDebug(possibleAlreadyExistentEntry.key);
        printDebug("==================");
        if(isSameKey(possibleAlreadyExistentEntry, key))
        {
            possibleAlreadyExistentEntry.value = value;
            return;
        }

        index++;

        if (index >= capacity) 
        {
            index = 0;
        }
        possibleAlreadyExistentEntry = entries[index];
    }

    entries[index].key = malloc(strlen(key));
    strcpy(entries[index].key, key);
    
    entries[index].value = value;

    if (size) {
        (*size)++;
    }
}

SymbolTableValue createSymbolTableValue(SymbolType symbolType, LexicalValue lexicalValue)
{
    SymbolTableValue value;
    value.lexicalValue = lexicalValue;
    value.symbolType = symbolType;
    value.lineNumber = lexicalValue.lineNumber;
    // value.symbolDataType = 0; // TODO
    // value.size = 0; // TODO
    value.firstArgument = NULL; // TODO
    return value;
}

void addValueToSymbolTable(SymbolTable* table, SymbolTableValue value)
{
    if (!table)
    {
        printError("Trying to add symbol on null table");
        return;
    }
    if (!value.lexicalValue.label)
    {
        printError("Trying to add symbol without label");
        return;
    }

    if (table->size >= table->capacity / TABLE_EXPAND_FACTOR)
    {
        printDebug("Expanding table");
        if (!expandSymbolTable(table)) {
            printError("Fail to expand the table, symbol will not be added");
            return;
        }
    }
    
    addEntryOnList(table->entries, table->capacity, value.lexicalValue.label, &table->size, value);
}

int expandSymbolTable(SymbolTable* table) 
{
    int newCapacity = table->capacity * 2;
    if (newCapacity < table->capacity)
    {
        printError("[SymbolTable] Overflow! SymbolTable capacity has reach it's limit.");
        return 0;
    }
    
    SymbolTableEntry* newEntries = calloc(newCapacity, sizeof(SymbolTableEntry));
    if (!newEntries) {
        printError("[SymbolTable] Fail to create new entries list during expansion");
        return 0;
    }

    for (size_t index = 0; index < table->capacity; index++) {
        SymbolTableEntry entry = table->entries[index];
        if (entry.key) {
            addEntryOnList(newEntries, newCapacity, entry.key, NULL, entry.value);
        }
    }

    destroyTableEntries(table);

    table->entries = newEntries;
    table->capacity = newCapacity;
    return 1;
}

SymbolTableStack* createSymbolTableStack() 
{
    SymbolTableStack* tableStack = malloc(sizeof(SymbolTableStack));
    if (!tableStack) 
    {
        printError("[SymbolTableStack] Fail to create symbol table stack!");
        return NULL;
    }
    tableStack->symbolTable = NULL;
    tableStack->nextItem = NULL;
    return tableStack;
}

void destroySymbolTableStack(SymbolTableStack* symbolTableStack)
{
    if (!symbolTableStack) return;

    destroySymbolTableStack(symbolTableStack->nextItem);

    destroySymbolTable(symbolTableStack->symbolTable);

    free(symbolTableStack);
}

SymbolTable* getFirstTableFromSymbolTableStack(SymbolTableStack* symbolTableStack) 
{
    if (!symbolTableStack) return NULL;

    return symbolTableStack->symbolTable;
}

SymbolTableStack* destroyFirstTableFromSymbolTableStack(SymbolTableStack* symbolTableStack)
{
    if (!symbolTableStack) return NULL;

    printf("Destroying table from stack... \n");

    destroySymbolTable(symbolTableStack->symbolTable);

    SymbolTableStack* nextItem = symbolTableStack->nextItem;

    free(symbolTableStack);

    return nextItem;
}

SymbolTableStack* createNewTableOnSymbolTableStack(SymbolTableStack* symbolTableStack) {
    SymbolTable* newTable = createSymbolTable();
    return addTableToSymbolTableStack(symbolTableStack, newTable);
}

SymbolTableStack* addTableToSymbolTableStack(SymbolTableStack* currentFirstTable, SymbolTable* symbolTable)
{
    if (!currentFirstTable) {
        printf("Adding table to null stack \n");
        return NULL;
    }

    if (!symbolTable) {
        printf("Adding null table to stack \n");
        return NULL;
    }

    printf("Adding new table to stack... \n");

    SymbolTableStack* newFirstTable = createSymbolTableStack();
    newFirstTable->symbolTable = symbolTable;
    newFirstTable->nextItem = currentFirstTable;

    return newFirstTable;
}

SymbolTableValue getByKeyOnSymbolTableStack(SymbolTableStack* symbolTableStack, char* key)
{
    if (!symbolTableStack) 
    {
        return getEmptyValue();
    }
    
    SymbolTableValue value = getSymbolTableValueByKey(symbolTableStack->symbolTable, key);
    if (value.symbolType == SYMBOL_TYPE_NON_EXISTENT) 
    {
        return getByKeyOnSymbolTableStack(symbolTableStack->nextItem, key);
    }
    else
    {
        return value;
    }
}

void addValueToSymbolTableStack(SymbolTableStack* stack, SymbolTableValue value) 
{
	if (!stack) return;

	addValueToSymbolTable(stack->symbolTable, value);
}

int checkValueIsOnFirstSymbolTable(SymbolTableStack* symbolTableStack, char* key)
{
	if (!symbolTableStack) return 0;

	SymbolTable* firstSymbolTable = getFirstTableFromSymbolTableStack(symbolTableStack);

	SymbolTableValue value = getSymbolTableValueByKey(firstSymbolTable, key);
    if (value.symbolType == SYMBOL_TYPE_NON_EXISTENT) 
    {
		return 0;
	}
	else
	{
		return 1;
	}
}

void printSymbolTableStack(SymbolTableStack* symbolTableStack) {
    if (!symbolTableStack) return;
    if (!symbolTableStack->symbolTable) return;

    SymbolTable* table = symbolTableStack->symbolTable;

    printf("============TABLE============ \n");
    for (size_t index = 0; index < table->capacity; index++) {
        SymbolTableEntry entry = table->entries[index];
        if (entry.key) {
            printf("%s | %s \n", entry.key, entry.value.lexicalValue.label);
        }
    }
    printf("=============END============= \n \n");

    printSymbolTableStack(symbolTableStack->nextItem);
}
