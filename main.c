/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include "generic_tree.h"
#include <stdio.h>
extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;

int main (int argc, char **argv)
{
  int ret = yyparse(); 
  // exporta(arvore);
  printTree(arvore);
  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  return ret;
}