/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>
#include "symbol_table.h"
#include "generic_tree.h"
extern int yyparse(void);
extern int yylex_destroy(void);

Node* arvore = NULL;
SymbolTableStack* symbolTableStack = NULL;

void initSymbolStack() {
  symbolTableStack = createSymbolTableStack();
  SymbolTable* globalTable = createSymbolTable();
  symbolTableStack = addTableToSymbolTableStack(symbolTableStack, globalTable);
}

void freeGlobalTree() {
  libera(arvore);
  arvore = NULL;
}

void freeGlobalSymbolTableStack() {
  destroySymbolTableStack(symbolTableStack);
  symbolTableStack = NULL;
}

int main (int argc, char **argv)
{
  initSymbolStack();
  int ret = yyparse(); 
  // exporta(arvore);
  printTree(arvore);
  freeGlobalTree();
  freeGlobalSymbolTableStack();
  yylex_destroy();
  return ret;
}