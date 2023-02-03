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

    node->lexicalValue = createLexicalValue(specialCharacter, SPECIAL_CHARACTER, NULL, lineNumber);
    node->nextBrother = NULL;
    node->firstChild = NULL;
    node->parent = NULL;

    return node;
}

Node* addChild(Node* parent, Node* child) 
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
    Node* node = getRoot(node);
    printTreeRecursively(node, 1);
}

void printTreeRecursively(Node* node, int level) 
{
    for(int x = 0; x < level; x++) 
    {
        printf("    ");
    }
    printf("%s", getLexicalValueLabel(node->lexicalValue));
    printf("\n");

    Node *child = node->firstChild;
    while(child != NULL)
    {
        printTreeRecursively(child, level + 1);
        child = child->nextBrother;
    }
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