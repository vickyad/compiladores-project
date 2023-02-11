#ifndef GENERIC_TREE_HEADER
#define GENERIC_TREE_HEADER

#include <stdio.h>
#include "lexical_value.h"
#include "print.h"

typedef struct Node {
    LexicalValue lexicalValue;
    struct Node* brother;
    struct Node* child;
    struct Node* parent;
} Node;

Node* createNode(LexicalValue lexicalValue);
Node* createNodeFromFunctionCall(LexicalValue lexicalValue);
void addChild(Node* parent, Node* child);
Node* getRoot(Node* node);
Node* getLastChild(Node* parent);
void libera(Node* node);
void exporta(Node* node);
void printHeader(Node* node);
void printBody(Node* node);

#endif