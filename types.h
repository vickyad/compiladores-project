#ifndef TYPES_HEADER
#define TYPES_HEADER

// ==============================
// Dimension
// ==============================
typedef struct Dimension
{
    int size;
    int quantity;
} Dimension;

// ==============================
// Data type
// ==============================
typedef enum DataType {
    DATA_TYPE_INT,
    DATA_TYPE_FLOAT,
    DATA_TYPE_CHAR,
    DATA_TYPE_BOOL,
    DATA_TYPE_NON_DECLARED
} DataType;

// ==============================
// Lexical value
// ==============================
typedef enum TokenType {
    TOKEN_TYPE_SPECIAL_CHARACTER,
    TOKEN_TYPE_RESERVED_WORD,
    TOKEN_TYPE_COMPOUND_OPERATOR,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_LITERAL
} TokenType;

typedef enum LiteralType {
    LITERAL_TYPE_INT,
    LITERAL_TYPE_FLOAT,
    LITERAL_TYPE_CHAR,
    LITERAL_TYPE_BOOL,
    LITERAL_TYPE_IS_NOT_LITERAL
} LiteralType;

typedef struct LexicalValue {
    int lineNumber;
    int functionLabel;
    TokenType type;
    LiteralType literalType;
    
    union value {
        int value_int;
        float value_float;
        char value_char;
        int value_bool;
    } value;
    char* label;
} LexicalValue;

// ==============================
// Lexical Tree
// ==============================
typedef struct Node {
    LexicalValue lexicalValue;
	DataType dataType;
    IlocOperationList* operationList;
    int outRegister;
    struct Node* brother;
    struct Node* child;
    struct Node* parent;
} Node;

// ==============================
// SYMBOL TABLE
// ==============================
typedef struct FunctionArgument {
    DataType type;
    LexicalValue lexicalValue;
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
    int lineNumber;
    int numberOfDimensions;
    int isGlobal;
    int position;
    SymbolType symbolType;
    DataType dataType;
    LexicalValue lexicalValue;
    FunctionArgument* arguments;
} SymbolTableValue;

typedef struct SymbolTableEntry {
    char* key;
    SymbolTableValue value;
} SymbolTableEntry;

typedef struct SymbolTable {
    int size;
    int capacity;
    int lastPosition;
    SymbolTableEntry* entries;
} SymbolTable;

typedef struct SymbolTableStack {
    SymbolTable* symbolTable;
    struct SymbolTableStack* nextItem;
    int isGlobal;
} SymbolTableStack;

// ==============================
// ILOC
// ==============================
typedef enum IlocOperationType {
    OP_ADD,
    OP_SUB,
    OP_MULT,
    OP_DIV,
    OP_CMP_GE,
    OP_CBR,
    OP_JUMPI,
    OP_CMP_LE,
    OP_CMP_LT,
    OP_CMP_GT,
    OP_CMP_NE,
    OP_CMP_EQ,
    OP_AND,
    OP_OR,
    OP_LOADI_GLOBAL,
    OP_LOADI_LOCAL,
    OP_LOADI,
    OP_STOREAI_GLOBAL,
    OP_STOREAI_LOCAL,
    OP_NOP
} IlocOperationType;

typedef struct IlocOperation {
    IlocOperationType type;
    int label;
    int op1;
    int op2;
    int out1;
    int out2;
} IlocOperation;

typedef struct IlocOperationList {
    IlocOperation operation;
    struct IlocOperationList* nextItem;
} IlocOperationList;

#endif