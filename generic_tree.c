#include "generic_tree.h"

Node* createNode(LexicalValue lexicalValue) 
{
    Node* node = malloc(sizeof(Node));

    node->lexicalValue = lexicalValue;
    node->brother = NULL;
    node->child = NULL;
    node->parent = NULL;

    return node;
}


Node* createNodeFromLabel(char* label, int lineNumber)
{
    Node* node = malloc(sizeof(Node));

    node->lexicalValue = createLexicalValue(label, SPECIAL_CHARACTER, IS_NOT_LITERAL, lineNumber);
    node->brother = NULL;
    node->child = NULL;
    node->parent = NULL;

    return node;
}

void addChild(Node* parent, Node* child) 
{
    if (!child) return;

    if (!parent) {
        freeNode(child);
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

void printTree(Node* node) 
{
    if (node) 
    {
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
    int i = 0;

    if (!node) return;

    for(i = 0; i < level - 1; i++) 
    {
        printf("    ");
    }

    if (level == 0)
    {
        printf("%s", node->lexicalValue.label);
    }
    else 
    {
        printf("●---");
        printf("%s", node->lexicalValue.label);
    }
    printf("\n");

    Node *nodo_f = node->child;
    while(nodo_f)
    {
        printTreeRecursively(nodo_f, level+1);
        nodo_f = nodo_f->brother;
    }
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

void libera(Node* node)
{
    freeNode(node);
}

void freeNode(Node* node)
{
    if (!node) return;
    
    freeLexicalValue(node->lexicalValue);

    Node* child = node->child;
    freeNode(child);

    Node* brother = node->brother;
    freeNode(brother);

    free(node);
}