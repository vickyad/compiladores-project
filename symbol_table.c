#include "symbol_table.h"

SymbolTable* createTable() 
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
        destroyTable(table);
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

void destroyTable(SymbolTable* table) 
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

SymbolTableValue getValue(SymbolTable* table, char* key) 
{
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

SymbolTableValue createTableValue(SymbolType symbolType, LexicalValue lexicalValue)
{
    SymbolTableValue value;
    value.lexicalValue = lexicalValue;
    value.symbolType = symbolType;
    value.size = 0; // TODO: Calculate size
    value.firstArgument = NULL; // TODO: Method to add arguments
    return value;
}

void addSymbol(SymbolTable* table, SymbolTableValue value)
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
        if (!expandTable(table)) {
            printError("Fail to expand the table, symbol will not be added");
            return;
        }
    }
    
    addEntryOnList(table->entries, table->capacity, value.lexicalValue.label, &table->size, value);
}

int expandTable(SymbolTable* table) 
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