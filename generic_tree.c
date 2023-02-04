#include "generic_tree.h"

Node* createNode(LexicalValue lexicalValue) 
{
    Node* node = malloc(sizeof(Node));

    node->lexicalValue = lexicalValue;
    node->nextBrother = NULL;
    node->firstChild = NULL;
    node->parent = NULL;

    return node;
}


Node* createNodeFromSpecialCharacter(char* specialCharacter, int lineNumber)
{
    Node* node = malloc(sizeof(Node));

    node->lexicalValue = createLexicalValue(specialCharacter, SPECIAL_CHARACTER, IS_NOT_LITERAL, lineNumber);
    node->nextBrother = NULL;
    node->firstChild = NULL;
    node->parent = NULL;

    return node;
}

void addChild(Node* parent, Node* child) 
{
    Node* lastChild = getLastChild(parent);
    if (lastChild != NULL) 
    {
        lastChild->nextBrother = child;
    } 
    else 
    {
        parent->firstChild = child;
    }
    child->parent = parent;
}

void printTree(Node* node) 
{
    if (node != NULL) {
        printNonNullTree(node);
    }
}

void printNonNullTree(Node* node) 
{
    Node* rootNode = getRoot(node);
    printTreeRecursively(rootNode, 1);
}

void printTreeRecursively(Node* node, int level) 
{
    Node *child = node->firstChild;
    while(child != NULL)
    {
        printTreeRecursively(child, level + 1);
        child = child->nextBrother;
    }

    printf("Nível %d %s", level, node->lexicalValue.label);
    printf("\n");
}

Node* getRoot(Node* node) 
{
    Node* currentParent = NULL;
    Node* possibleParent = node->parent;
    while(possibleParent != NULL) {
        currentParent = possibleParent;
        possibleParent = possibleParent->parent;
    }
    return currentParent;
}

Node* getLastChild(Node* parent) {
    Node* currentLastChild = NULL;
    Node* possibleLastChild = parent->firstChild;
    while (possibleLastChild != NULL) 
    {
        currentLastChild = possibleLastChild;
        possibleLastChild = possibleLastChild->firstChild;
    }
    return currentLastChild;
}