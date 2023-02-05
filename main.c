/*
Função principal para realização da análise sintática.

Este arquivo será posterioremente substituído, não acrescente nada.
*/
#include "generic_tree.h"
#include "debug.h"
#include <stdio.h>
extern int yyparse(void);
extern int yylex_destroy(void);

void* arvore = NULL;
// void exporta (void *arvore);
// void libera (void *arvore);

int main (int argc, char **argv)
{
  int ret = yyparse(); 
  printTree(arvore);
  // exporta (arvore);
  // libera(arvore);
  // arvore = NULL;
  yylex_destroy();
  return ret;
}