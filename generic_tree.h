#ifndef GENERIC_TREE_HEADER
#define GENERIC_TREE_HEADER

#include <stdio.h>
#include "lexical_value.h"
#include "debug.h"

typedef struct Node {
    LexicalValue lexicalValue;
    struct Node* brother;
    struct Node* child;
    struct Node* parent;
} Node;

Node* createNode(LexicalValue lexicalValue);
Node* createNodeFromFunctionCall(LexicalValue lexicalValue);
void addChild(Node* parent, Node* child);
void printTree(Node* node);
void printNonNullTree(Node* node);
void printTreeRecursively(Node* node, int level);
Node* getRoot(Node* node);
Node* getLastChild(Node* parent);
void libera(Node* node);
void exporta(Node* node);
void getNodeLabel(Node* node);

#endif