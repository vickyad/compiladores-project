#ifndef GENERIC_TREE_HEADER
#define GENERIC_TREE_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexical_value.h"
#include "types.h"
#include "print.h"
#include "errors.h"
#include "data_type.h"

Node* createNode(LexicalValue lexicalValue);
Node* createNodeFromSymbol(LexicalValue lexicalValue, SymbolTableValue symbol);
Node* createNodeFromTwoChildren(LexicalValue lexicalValue, Node* leftChild, Node* rightChild);
Node* createNodeFromChild(LexicalValue lexicalValue, Node* leftChild);
Node* createNodeWithType(LexicalValue lexicalValue, DataType dataType);
Node* createNodeForFunctionCall(LexicalValue lexicalValue);
Node* createNodeForFunctionCallFromSymbol(LexicalValue lexicalValue, SymbolTableValue symbol);
void addChild(Node* parent, Node* child);
Node* getRoot(Node* node);
Node* getLastChild(Node* parent);
void libera(Node* node);
void exporta(Node* node);
void printHeader(Node* node);
void printBody(Node* node);
void printTree(Node* node);

#endif
