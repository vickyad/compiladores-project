#ifndef GENERIC_TREE_HEADER
#define GENERIC_TREE_HEADER

#include <stdio.h>
#include "lexical_value.h"

typedef struct Node {
    LexicalValue lexicalValue;
    Node* nextBrother;
    Node* firstChild;
    Node* parent;
} Node;

Node* createNode(LexicalValue LexicalValue);
Node* createNodeFromSpecialCharacter(char* specialCharacter);
Node* addChild(Node* parent, Node* child);
void printTree(Node* node);

#endif