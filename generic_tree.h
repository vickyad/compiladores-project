#ifndef GENERIC_TREE_HEADER
#define GENERIC_TREE_HEADER

#include <stdio.h>
#include "lexical_value.h"

typedef struct Node {
    LexicalValue lexicalValue;
    struct Node* nextBrother;
    struct Node* firstChild;
    struct Node* parent;
} Node;

Node* createNode(LexicalValue LexicalValue);
Node* createNodeFromSpecialCharacter(char* specialCharacter, int lineNumber);
void addChild(Node* parent, Node* child);
void printTree(Node* node);
void printNonNullTree(Node* node);
void printTreeRecursively(Node* node, int level);
Node* getRoot(Node* node);
Node* getLastChild(Node* parent);

#endif