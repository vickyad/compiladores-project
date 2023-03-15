#include "symbol_table.h"

extern Node* tree;
extern SymbolTableStack* symbolTableStack;

void initGlobalSymbolStack() 
{
    symbolTableStack = createSymbolTableStack();
    SymbolTable* globalTable = createSymbolTable();
    symbolTableStack = addTableToSymbolTableStack(symbolTableStack, globalTable);
    symbolTableStack->isGlobal = 1;
    symbolTableStack->lastPosition = 0;
}

char* getSymbolTypeName(SymbolType symbolType)
{
    if (symbolType == SYMBOL_TYPE_ARRAY) return "arranjo";
    if (symbolType == SYMBOL_TYPE_FUNCTION) return "função";
    if (symbolType == SYMBOL_TYPE_LITERAL) return "literal";
    if (symbolType == SYMBOL_TYPE_VARIABLE) return "variável";
    
    return "inválido";
}

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
    table->lastPosition = 0;
    table->entries = calloc(table->capacity, sizeof(SymbolTableEntry));
    if (!table->entries) 
    {
        freeSymbolTable(table);
        printError("[SymbolTable] Fail to initialize entries for symbol table!");
        return NULL;
    }
    return table;
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

SymbolTableValue getEmptySymbolValue()
{
    SymbolTableValue value;
    value.symbolType = SYMBOL_TYPE_NON_EXISTENT;
    return value;
}

void updateFunctionLastPositionOnSymbleTable(SymbolTable* table, char* key, int functionLastPosition)
{
    if (!table) return;

    size_t index = getIndex(table->capacity, key);

    SymbolTableEntry possibleEntry = table->entries[index];
    while (possibleEntry.key) {
        if (isSameKey(possibleEntry, key)) 
        {
            table->entries[index].value.functionLastPosition = functionLastPosition;
            return;
        }
        
        index++;
        if (index >= table->capacity) 
        {
            index = 0;
        }
        possibleEntry = table->entries[index];
    }
}

SymbolTableValue getSymbolTableValueByKey(SymbolTable* table, char* key) 
{
    if (!table) return getEmptySymbolValue();

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

    return getEmptySymbolValue();
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

    entries[index].key = malloc((strlen(key) + 1) * sizeof(char));
    strcpy(entries[index].key, key);
    
    entries[index].value = value;

    if (size) {
        (*size)++;
    }
}

SymbolTableValue createSymbolTableValue(SymbolType symbolType, Node* node)
{
    return createSymbolTableValueWithType(symbolType, node->lexicalValue, node->dataType);
}

int calculateSymbolSize(SymbolType symbolType, DataType dataType)
{
    if (symbolType == SYMBOL_TYPE_LITERAL || symbolType == SYMBOL_TYPE_VARIABLE) 
    {
        if (dataType == DATA_TYPE_BOOL) return 1;
        if (dataType == DATA_TYPE_CHAR) return 1;
        if (dataType == DATA_TYPE_INT) return 4;
        if (dataType == DATA_TYPE_FLOAT) return 8;
    } 

    if (symbolType == SYMBOL_TYPE_ARRAY || symbolType == SYMBOL_TYPE_FUNCTION || symbolType == SYMBOL_TYPE_NON_EXISTENT)
    {
        return 0;
    }

    printf("Trying to calculate size of invalid symbol or data type");
    return 0;
}

SymbolTableValue createSymbolTableValueWithType(SymbolType symbolType, LexicalValue lexicalValue, DataType dataType)
{
    SymbolTableValue value;
    value.lexicalValue = lexicalValue;
    value.symbolType = symbolType;
    value.lineNumber = lexicalValue.lineNumber;
    value.dataType = dataType;
    value.size = calculateSymbolSize(symbolType, dataType);
    value.numberOfDimensions = 0;
    value.arguments = NULL;
    value.functionLastPosition = 0;
    return value;
}

SymbolTableValue createSymbolTableValueWithTypeAndDimension(SymbolType symbolType, LexicalValue lexicalValue, DataType dataType, Dimension dimension) 
{
    SymbolTableValue value = createSymbolTableValueWithType(symbolType, lexicalValue, dataType);
    value.size = dimension.size;
    value.numberOfDimensions = dimension.quantity;
    return value;
}

SymbolTableValue createSymbolTableValueWithTypeAndArguments(SymbolType symbolType, LexicalValue lexicalValue, DataType dataType, FunctionArgument* arguments, int functionLabel) 
{
    SymbolTableValue value = createSymbolTableValueWithType(symbolType, lexicalValue, dataType);
    value.arguments = arguments;
    value.lexicalValue.functionLabel = functionLabel;
    return value;
}

void addBlankSpaceToSymbolTableStack(SymbolTableStack* stack)
{
    if (!stack) {
        printError("Trying to add blank space on null stack");
        return;
    }

    SymbolTable* table = stack->symbolTable;

    if (!table)
    {
        printError("Trying to add blank space on null table \n");
        return;
    }
    
    table->lastPosition = table->lastPosition + 4;
    stack->lastPosition = table->lastPosition;
}

SymbolTableValue addValueToSymbolTableStack(SymbolTableStack* stack, SymbolTableValue value)
{
    if (!stack) {
        printError("Trying to add symbol on null stack");
        return value;
    }

    SymbolTable* table = stack->symbolTable;

    if (!table)
    {
        printError("Trying to add symbol on null table \n");
        printf("Symbol: %s", value.lexicalValue.label);
        return value;
    }

    if (!value.lexicalValue.label)
    {
        printError("Trying to add symbol without label");
        return value;
    }

    SymbolTableValue existentValue = getSymbolTableValueByKey(table, value.lexicalValue.label);

    if (existentValue.symbolType == SYMBOL_TYPE_LITERAL) return value;

    if (existentValue.symbolType != SYMBOL_TYPE_NON_EXISTENT) 
    {
        printf("ERRO! Falha ao declarar %s na linha %d. A declaração já foi feita na linha %d", 
            value.lexicalValue.label, 
            value.lineNumber,
            existentValue.lineNumber
        );
        freeGlobalVariables();
        exit(ERR_DECLARED);
    }

    if (table->size >= table->capacity / TABLE_EXPAND_FACTOR)
    {
        printDebug("Expanding table");
        if (!expandSymbolTable(table)) {
            printError("Fail to expand the table, symbol will not be added");
            return value;
        }
    }

    value.isGlobal = stack->isGlobal;
    value.position = table->lastPosition;
    
    if (value.symbolType != SYMBOL_TYPE_LITERAL) {
        table->lastPosition = table->lastPosition + value.size;
    }
    stack->lastPosition = table->lastPosition;
    
    addEntryOnList(table->entries, table->capacity, value.lexicalValue.label, &table->size, value);

    return value;
}

SymbolTableValue addValueToSecondSymbolTableOnStack(SymbolTableStack* stack, SymbolTableValue value)
{
    if (!stack) 
    {
        printError("Adding value to empty stack");
        return value;
    }
    else
    {
        return addValueToSymbolTableStack(stack->nextItem, value);
    }
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

    freeTableEntries(table);

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
    tableStack->isGlobal = 0;
    tableStack->lastPosition = 0;
    return tableStack;
}

SymbolTable* getFirstTableFromSymbolTableStack(SymbolTableStack* symbolTableStack) 
{
    if (!symbolTableStack) return NULL;

    return symbolTableStack->symbolTable;
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

    SymbolTableStack* newFirstTable = createSymbolTableStack();
    newFirstTable->symbolTable = symbolTable;
    newFirstTable->nextItem = currentFirstTable;
    if (currentFirstTable->isGlobal) 
    {
        newFirstTable->lastPosition = 0;
    }
    else 
    {
        symbolTable->lastPosition = currentFirstTable->lastPosition;
        newFirstTable->lastPosition = currentFirstTable->lastPosition;
    }

    return newFirstTable;
}

SymbolTableValue getByLexicalValueOnSymbolTableStack(SymbolTableStack* symbolTableStack, LexicalValue lexicalValue)
{
    if (!symbolTableStack) 
    {
        printf("ERRO! Variável %s não foi declarada antes do seu uso na linha %d", lexicalValue.label, lexicalValue.lineNumber);
        freeGlobalVariables();
        exit(ERR_UNDECLARED);
    }
    
    SymbolTableValue value = getSymbolTableValueByKey(symbolTableStack->symbolTable, lexicalValue.label);
    if (value.symbolType == SYMBOL_TYPE_NON_EXISTENT) 
    {
        return getByLexicalValueOnSymbolTableStack(symbolTableStack->nextItem, lexicalValue);
    }
    else
    {
        return value;
    }
}

void updateFunctionLastPosition(SymbolTableStack* symbolTableStack, LexicalValue lexicalValue, int functionLastPosition)
{
    if (!symbolTableStack) 
    {
        printf("ERRO! Variável %s não foi declarada antes do seu uso na linha %d", lexicalValue.label, lexicalValue.lineNumber);
        freeGlobalVariables();
        exit(ERR_UNDECLARED);
    }

    updateFunctionLastPositionOnSymbleTable(symbolTableStack->symbolTable, lexicalValue.label, functionLastPosition);
}

SymbolTableStack* getStackByLexicalValue(SymbolTableStack* symbolTableStack, LexicalValue lexicalValue)
{
    if (!symbolTableStack) 
    {
        printf("ERRO! Variável %s não foi declarada antes do seu uso na linha %d", lexicalValue.label, lexicalValue.lineNumber);
        freeGlobalVariables();
        exit(ERR_UNDECLARED);
    }
    
    SymbolTableValue value = getSymbolTableValueByKey(symbolTableStack->symbolTable, lexicalValue.label);
    if (value.symbolType == SYMBOL_TYPE_NON_EXISTENT) 
    {
        return getStackByLexicalValue(symbolTableStack->nextItem, lexicalValue);
    }
    else
    {
        return symbolTableStack;
    }
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

FunctionArgument* createFunctionArgument(LexicalValue lexicalValue, DataType dataType) 
{
    FunctionArgument* functionArgument = malloc(sizeof(FunctionArgument));
    if (!functionArgument) 
    {
        printf("Fail to create function argument.");
    }
    functionArgument->nextArgument = NULL;
    functionArgument->type = dataType;
    functionArgument->lexicalValue = copyLexicalValue(lexicalValue);
    functionArgument->position = -1;
    functionArgument->tempRegister = -1;

    return functionArgument;
}

FunctionArgument* addFunctionArgument(FunctionArgument* functionArgument, LexicalValue lexicalValue, DataType dataType)
{
    FunctionArgument* newFunctionArgument = createFunctionArgument(lexicalValue, dataType);
    newFunctionArgument->nextArgument = functionArgument;
    return newFunctionArgument;
}

void validateFunctionCall(SymbolTableValue symbol, LexicalValue lexicalValue)
{
    if (symbol.symbolType != SYMBOL_TYPE_FUNCTION) 
    {
        printf("ERRO! Utilização de %s do tipo %s como chamada de função na linha %d", 
            symbol.lexicalValue.label, 
            getSymbolTypeName(symbol.symbolType), 
            lexicalValue.lineNumber
        );
        freeGlobalVariables();
        if (symbol.symbolType == SYMBOL_TYPE_VARIABLE) exit(ERR_VARIABLE);
        if (symbol.symbolType == SYMBOL_TYPE_ARRAY) exit(ERR_ARRAY);    
    }
}

void validateArrayUse(SymbolTableValue symbol, LexicalValue lexicalValue)
{
    if (symbol.symbolType != SYMBOL_TYPE_ARRAY)
    {
        printf("ERRO! Utilização de %s do tipo %s como arranjo na linha %d", 
            symbol.lexicalValue.label, 
            getSymbolTypeName(symbol.symbolType), 
            lexicalValue.lineNumber
        );
        freeGlobalVariables();
        if (symbol.symbolType == SYMBOL_TYPE_VARIABLE) exit(ERR_VARIABLE);
        if (symbol.symbolType == SYMBOL_TYPE_FUNCTION) exit(ERR_FUNCTION);    
    }
}

void validateVariableUse(SymbolTableValue symbol, LexicalValue lexicalValue)
{
    if (symbol.symbolType != SYMBOL_TYPE_VARIABLE)
    {
        printf("ERRO! Utilização de %s do tipo %s como variável na linha %d", 
            symbol.lexicalValue.label, 
            getSymbolTypeName(symbol.symbolType), 
            lexicalValue.lineNumber
        );
        freeGlobalVariables();
        if (symbol.symbolType == SYMBOL_TYPE_ARRAY) exit(ERR_ARRAY);
        if (symbol.symbolType == SYMBOL_TYPE_FUNCTION) exit(ERR_FUNCTION);    
    }
}

void validateArrayDeclaration(SymbolTableValue symbol)
{
    if (symbol.dataType == DATA_TYPE_CHAR)
    {
        printf("ERRO! Declaração inválida de array %s com o tipo char na linha %d", 
            symbol.lexicalValue.label, 
            symbol.lexicalValue.lineNumber
        );
        freeGlobalVariables();
        exit(ERR_CHAR_VECTOR);
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
            printf("[key=%s] [type=%s] [size=%d] [isGlobal=%d] [position=%d]", entry.key, getDataTypeName(entry.value.dataType), entry.value.size, entry.value.isGlobal, entry.value.position);
            FunctionArgument* currentArgument = entry.value.arguments;
            while(currentArgument) 
            {
                printf("\n=> argument %s [type=%s]", currentArgument->lexicalValue.label, getDataTypeName(currentArgument->type));
                currentArgument = currentArgument->nextArgument;
            }
            printf("\n");
        }
    }
    printf("=============END============= \n \n");

    printSymbolTableStack(symbolTableStack->nextItem);
}