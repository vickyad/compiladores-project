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


Node* createNodeFromFunctionCall(LexicalValue lexicalValue)
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

void addChild(Node* parent, Node* child) 
{
    if (!child) return;

    if (!parent) {
        libera(child);
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
    if (!node) return;
    
    freeLexicalValue(node->lexicalValue);

    Node* child = node->child;
    libera(child);

    Node* brother = node->brother;
    libera(brother);

    free(node);
}

void exporta(Node* node)
{
    if (!node) return;

    printHeader(node);
    printBody(node);
}

void printHeader(Node* node)
{
    printf("%p [label=\"%s\"]\n", node, node->lexicalValue.label);
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

void getNodeLabel(Node* node)
{

}