#ifndef GENERIC_TREE_HEADER
#define GENERIC_TREE_HEADER

#include <stdio.h>
#include "lexical_value.h"
#include "print.h"

typedef enum NodeType {
    NODE_TYPE_INT,
    NODE_TYPE_FLOAT,
    NODE_TYPE_CHAR,
    NODE_TYPE_BOOL
} LiteralType;

typedef struct Node {
    LexicalValue lexicalValue;
	NodeType nodeType;
    struct Node* brother;
    struct Node* child;
    struct Node* parent;
} Node;

Node* createNode(LexicalValue lexicalValue);
Node* createNodeWithType(LexicalValue lexicalValue, NodeType nodeType);
Node* createNodeFromFunctionCall(LexicalValue lexicalValue);
void addChild(Node* parent, Node* child);
Node* getRoot(Node* node);
Node* getLastChild(Node* parent);
void libera(Node* node);
void exporta(Node* node);
void printHeader(Node* node);
void printBody(Node* node);

#endif
