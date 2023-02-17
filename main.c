#include <stdio.h>
#include "free.h"
#include "generic_tree.h"
#include "symbol_table.h"

extern int yyparse(void);
extern int yylex_destroy(void);

Node* tree;
SymbolTableStack* symbolTableStack;

int main (int argc, char **argv)
{
  initGlobalSymbolStack();
  int ret = yyparse(); 
  // exportTree(tree);
  printTree(tree);
  freeGlobalVariables();
  yylex_destroy();
  return 0;
}