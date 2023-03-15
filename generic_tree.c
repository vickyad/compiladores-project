#include "generic_tree.h"

Node* createNode(LexicalValue lexicalValue) 
{
    Node* node = malloc(sizeof(Node));

    node->lexicalValue = lexicalValue;
    node->brother = NULL;
    node->child = NULL;
    node->parent = NULL;
    node->dataType = DATA_TYPE_NON_DECLARED;
    node->operationList = NULL;
    node->outRegister = -1;

    return node;
}


Node* createNodeForFunctionCall(LexicalValue lexicalValue)
{
    Node* node = createNode(lexicalValue);

    char* prefix = "call ";
    char* newLabel = malloc(strlen(prefix)+strlen(node->lexicalValue.label)+1);

    strcpy(newLabel, prefix);
    strcat(newLabel, node->lexicalValue.label);

    free(node->lexicalValue.label);

    node->lexicalValue.label = newLabel;

    return node;
}

Node* createNodeForFunctionCallFromSymbol(LexicalValue lexicalValue, SymbolTableValue symbol)
{
    Node* node = createNodeForFunctionCall(lexicalValue);
    node->dataType = symbol.dataType;
    return node;
}

Node* createNodeWithType(LexicalValue lexicalValue, DataType dataType)
{
	Node* node = createNode(lexicalValue);
	node->dataType = dataType;
    return node;
}

int dataTypesAreIntAndFloat(DataType typeOne, DataType typeTwo)
{
    if (typeOne == DATA_TYPE_INT && typeTwo == DATA_TYPE_FLOAT) 
    {
        return 1;
    }
    if (typeOne == DATA_TYPE_FLOAT && typeTwo == DATA_TYPE_INT)
    {
        return 1;
    }
    return 0;
}

int dataTypesAreBooleanAndInt(DataType typeOne, DataType typeTwo)
{
    if (typeOne == DATA_TYPE_INT && typeTwo == DATA_TYPE_BOOL) 
    {
        return 1;
    }
    if (typeOne == DATA_TYPE_BOOL && typeTwo == DATA_TYPE_INT)
    {
        return 1;
    }
    return 0;
}

int dataTypesAreBooleanAndFloat(DataType typeOne, DataType typeTwo)
{
    if (typeOne == DATA_TYPE_FLOAT && typeTwo == DATA_TYPE_BOOL) 
    {
        return 1;
    }
    if (typeOne == DATA_TYPE_BOOL && typeTwo == DATA_TYPE_FLOAT)
    {
        return 1;
    }
    return 0;
}

void throwCharConversionError(Node* nodeOne, Node* nodeTwo)
{
    Node* nonCharNode;
    if (nodeOne->dataType == DATA_TYPE_CHAR) 
    {
        nonCharNode = nodeTwo;
    } 
    else 
    {
        nonCharNode = nodeOne;
    }

    printf("ERRO! Tipo %s não é compatível com char na linha %d.", 
        getDataTypeName(nonCharNode->dataType), 
        nonCharNode->lexicalValue.lineNumber
    );

    if (nonCharNode->dataType == DATA_TYPE_BOOL) {
        freeGlobalVariables();
        exit(ERR_CHAR_TO_BOOL);
    } else if (nonCharNode->dataType == DATA_TYPE_FLOAT) {
        freeGlobalVariables();
        exit(ERR_CHAR_TO_FLOAT);
    } else if (nonCharNode->dataType == DATA_TYPE_INT) {
        freeGlobalVariables();
        exit(ERR_CHAR_TO_INT);
    }    
}

DataType typeInference(Node* nodeOne, Node* nodeTwo)
{
    if (nodeOne->dataType == nodeTwo->dataType) return nodeOne->dataType;

    if (dataTypesAreIntAndFloat(nodeOne->dataType, nodeTwo->dataType)) return DATA_TYPE_FLOAT;

    if (dataTypesAreBooleanAndInt(nodeOne->dataType, nodeTwo->dataType)) return DATA_TYPE_INT;

    if (dataTypesAreBooleanAndFloat(nodeOne->dataType, nodeTwo->dataType)) return DATA_TYPE_FLOAT;

    throwCharConversionError(nodeOne, nodeTwo);

    return DATA_TYPE_NON_DECLARED;
}

Node* createNodeFromBinaryOperator(LexicalValue lexicalValue, Node* leftChild, Node* rightChild) 
{
    Node* node = createNode(lexicalValue);
	node->dataType = typeInference(leftChild, rightChild);
    return node;
}

Node* createNodeFromUnaryOperator(LexicalValue lexicalValue, Node* child)
{
    Node* node = createNode(lexicalValue);
	node->dataType = child->dataType;
    return node;
}

void validateAttributionTypes(Node* leftChild, Node* rightChild)
{
    if (leftChild->dataType == DATA_TYPE_CHAR && rightChild->dataType != DATA_TYPE_CHAR)
    {
        printf("ERRO! Tentativa de atribuir o tipo %s para %s do tipo char na linha %d",
            getDataTypeName(rightChild->dataType),
            leftChild->lexicalValue.label,
            leftChild->lexicalValue.lineNumber
        );
        freeGlobalVariables();
        exit(ERR_X_TO_CHAR);
    }
    else if (rightChild->dataType == DATA_TYPE_CHAR && leftChild->dataType != DATA_TYPE_CHAR)
    {
        throwCharConversionError(rightChild, leftChild);
    }
}

Node* createNodeFromAttribution(LexicalValue lexicalValue, Node* leftChild, Node* rightChild)
{
    Node* node = createNode(lexicalValue);
	node->dataType = leftChild->dataType;

    validateAttributionTypes(leftChild, rightChild);

    return node;
}

Node* createNodeFromSymbol(LexicalValue lexicalValue, SymbolTableValue symbol)
{
    Node* node = createNode(lexicalValue);
    node->dataType = symbol.dataType;
    node->lexicalValue = copyLexicalValue(symbol.lexicalValue);
    return node;
}

void addChild(Node* parent, Node* child) 
{
    if (!child) return;

    if (!parent) {
        freeTree(child);
        return;
    }

    Node* lastChild = getLastChild(parent);
    if (lastChild)
    {
        lastChild->brother = child;
    } 
    else 
    {
        parent->child = child;
    }
    child->parent = parent;
}

Node* getRoot(Node* node)
{
    Node* currentParent = node;
    Node* possibleParent = node->parent;
    while(possibleParent) 
    {
        currentParent = possibleParent;
        possibleParent = possibleParent->parent;
    }
    return currentParent;
}

Node* getLastChild(Node* parent) 
{
    Node* currentLastChild = NULL;
    Node* possibleLastChild = parent->child;
    while (possibleLastChild)
    {
        currentLastChild = possibleLastChild;
        possibleLastChild = possibleLastChild->brother;
    }
    return currentLastChild;
}

void exportTree(Node* node)
{
    if (!node) return;

    printHeader(node);
    printBody(node);
}

void printHeader(Node* node)
{
    printf("%p [label=\"%s\"];\n", node, node->lexicalValue.label);
    if (node->child) 
    {
        printHeader(node->child);
    }
    if (node->brother)
    {
        printHeader(node->brother);
    }
}

void printBody(Node* node)
{
    if (node->parent) {
         printf("%p, %p \n", node->parent, node);
    }
    if (node->child) 
    {
        printBody(node->child);  
    }
    if (node->brother)
    {
        printBody(node->brother);
    }
}

void printTreeRecursively(Node* node, int level) 
{
    int i = 0;

    if (!node) return;

    for(i = 0; i < level - 1; i++) 
    {
        printf("    ");
    }

    if (level == 0)
    {
        printf("%s [type=%s]", node->lexicalValue.label, getDataTypeName(node->dataType));
    }
    else 
    {
        printf("●---");
        printf("%s [type=%s]", node->lexicalValue.label, getDataTypeName(node->dataType));
    }
    printf("\n");

    Node *nodo_f = node->child;
    while(nodo_f)
    {
        printTreeRecursively(nodo_f, level+1);
        nodo_f = nodo_f->brother;
    }
}

void printNonNullTree(Node* node) 
{
    Node* rootNode = getRoot(node);
    printTreeRecursively(rootNode, 1);
}

void printTree(Node* node) 
{
    if (node) 
    {
        printNonNullTree(node);
    }
}