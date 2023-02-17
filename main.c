#include <stdio.h>
#include "free.h"
#include "generic_tree.h"
#include "symbol_table.h"

extern int yyparse(void);

Node* tree;
SymbolTableStack* symbolTableStack;

int main (int argc, char **argv)
{
  initGlobalSymbolStack();
  int ret = yyparse(); 
  // exportTree(tree);
  freeGlobalVariables();
  return 0;
}