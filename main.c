/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>
#include "symbol_table.h"
extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);

SymbolTable* getSymbolTableWithItems()
{
  SymbolTable* symbolTable = createSymbolTable();

  LexicalValue lexicalValue1 = createLexicalValue("1", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  LexicalValue lexicalValue2 = createLexicalValue("2", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  LexicalValue lexicalValue3 = createLexicalValue("3", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  LexicalValue lexicalValue4 = createLexicalValue("4", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  LexicalValue lexicalValue5 = createLexicalValue("5", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  LexicalValue lexicalValue6 = createLexicalValue("6", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  LexicalValue lexicalValue7 = createLexicalValue("7", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  LexicalValue lexicalValue8 = createLexicalValue("8", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);

  SymbolTableValue value1 = createSymbolTableValue(SYMBOL_TYPE_LITERAL, lexicalValue1);
  SymbolTableValue value2 = createSymbolTableValue(SYMBOL_TYPE_LITERAL, lexicalValue2);
  SymbolTableValue value3 = createSymbolTableValue(SYMBOL_TYPE_LITERAL, lexicalValue3);
  SymbolTableValue value4 = createSymbolTableValue(SYMBOL_TYPE_LITERAL, lexicalValue4);
  SymbolTableValue value5 = createSymbolTableValue(SYMBOL_TYPE_LITERAL, lexicalValue5);
  SymbolTableValue value6 = createSymbolTableValue(SYMBOL_TYPE_LITERAL, lexicalValue6);
  SymbolTableValue value7 = createSymbolTableValue(SYMBOL_TYPE_LITERAL, lexicalValue7);
  SymbolTableValue value8 = createSymbolTableValue(SYMBOL_TYPE_LITERAL, lexicalValue8);

  addValueToSymbolTable(symbolTable, value1);
  addValueToSymbolTable(symbolTable, value2);
  addValueToSymbolTable(symbolTable, value3);
  addValueToSymbolTable(symbolTable, value4);
  addValueToSymbolTable(symbolTable, value5);
  addValueToSymbolTable(symbolTable, value6);
  addValueToSymbolTable(symbolTable, value7);
  addValueToSymbolTable(symbolTable, value8);

  return symbolTable;
}

int main (int argc, char **argv)
{
  SymbolTableStack* stack = createSymbolTableStack();
  
  SymbolTable* tableOne = createSymbolTable();
  LexicalValue lexicalValueOne = createLexicalValue("1", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  SymbolTableValue valueOne = createSymbolTableValue(SYMBOL_TYPE_LITERAL, lexicalValueOne);
  addValueToSymbolTable(tableOne, valueOne);
  stack = addTableToSymbolTableStack(stack, tableOne);

  SymbolTable* tableTwo = createSymbolTable();
  LexicalValue lexicalValueTwo = createLexicalValue("2", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 20);
  SymbolTableValue valueTwo = createSymbolTableValue(SYMBOL_TYPE_LITERAL, lexicalValueTwo);
  addValueToSymbolTable(tableTwo, valueTwo);
  stack = addTableToSymbolTableStack(stack, tableTwo);

  SymbolTable* tableThree = createSymbolTable();
  LexicalValue lexicalValueThree = createLexicalValue("3", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 33);
  SymbolTableValue valueThree = createSymbolTableValue(SYMBOL_TYPE_LITERAL, lexicalValueThree);
  addValueToSymbolTable(tableThree, valueThree);
  stack = addTableToSymbolTableStack(stack, tableThree);

  SymbolTable* tableFour = createSymbolTable();
  LexicalValue lexicalValueFour = createLexicalValue("3", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 15);
  SymbolTableValue valueFour = createSymbolTableValue(SYMBOL_TYPE_LITERAL, lexicalValueFour);
  addValueToSymbolTable(tableFour, valueFour);
  stack = addTableToSymbolTableStack(stack, tableFour);

  SymbolTableValue value = getByKeyOnSymbolTableStack(stack, "3");
  printf("%d", value.lexicalValue.lineNumber);

  value = getByKeyOnSymbolTableStack(stack, "2");
  printf("%d", value.lexicalValue.lineNumber);

  value = getByKeyOnSymbolTableStack(stack, "1");
  printf("%d", value.lexicalValue.lineNumber);

  stack = destroyFirstTableFromSymbolTableStack(stack);

  value = getByKeyOnSymbolTableStack(stack, "3");
  printf("%d", value.lexicalValue.lineNumber);

  stack = destroyFirstTableFromSymbolTableStack(stack);

  value = getByKeyOnSymbolTableStack(stack, "3");
  printf("%d", value.symbolType == SYMBOL_TYPE_NON_EXISTENT);

  return 1;
  // int ret = yyparse(); 
  // exporta (arvore);
  // libera(arvore);
  // arvore = NULL;
  // yylex_destroy();
  // return ret;
}