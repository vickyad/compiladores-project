#ifndef GENERIC_TREE_HEADER
#define GENERIC_TREE_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "free.h"
#include "errors.h"
#include "data_type.h"
#include "lexical_value.h"
#include "types.h"
#include "print.h"

Node* createNode(LexicalValue lexicalValue);
Node* createNodeFromSymbol(LexicalValue lexicalValue, SymbolTableValue symbol);
Node* createNodeFromBinaryOperator(LexicalValue lexicalValue, Node* leftChild, Node* rightChild);
Node* createNodeFromUnaryOperator(LexicalValue lexicalValue, Node* child);
Node* createNodeFromAttribution(LexicalValue lexicalValue, Node* leftChild, Node* rightChild);
Node* createNodeWithType(LexicalValue lexicalValue, DataType dataType);
Node* createNodeForFunctionCall(LexicalValue lexicalValue);
Node* createNodeForFunctionCallFromSymbol(LexicalValue lexicalValue, SymbolTableValue symbol);
void addChild(Node* parent, Node* child);
Node* getRoot(Node* node);
Node* getLastChild(Node* parent);
void exportTree(Node* node);
void printHeader(Node* node);
void printBody(Node* node);
void printTree(Node* node);

#endif
