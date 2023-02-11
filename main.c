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

int main (int argc, char **argv)
{
  // Testing symbol table
  SymbolTable* symbolTable = createTable();

  printf("Table size: %d \n", symbolTable->size);
  printf("Table capacity: %d \n", symbolTable->capacity);

  LexicalValue lexicalValue1 = createLexicalValue("1", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  LexicalValue lexicalValue2 = createLexicalValue("2", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  LexicalValue lexicalValue3 = createLexicalValue("3", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  LexicalValue lexicalValue4 = createLexicalValue("4", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  LexicalValue lexicalValue5 = createLexicalValue("5", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  LexicalValue lexicalValue6 = createLexicalValue("6", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  LexicalValue lexicalValue7 = createLexicalValue("7", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);
  LexicalValue lexicalValue8 = createLexicalValue("8", TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, 10);

  SymbolTableValue value1 = createTableValue(SYMBOL_TYPE_LITERAL, lexicalValue1);
  SymbolTableValue value2 = createTableValue(SYMBOL_TYPE_LITERAL, lexicalValue2);
  SymbolTableValue value3 = createTableValue(SYMBOL_TYPE_LITERAL, lexicalValue3);
  SymbolTableValue value4 = createTableValue(SYMBOL_TYPE_LITERAL, lexicalValue4);
  SymbolTableValue value5 = createTableValue(SYMBOL_TYPE_LITERAL, lexicalValue5);
  SymbolTableValue value6 = createTableValue(SYMBOL_TYPE_LITERAL, lexicalValue6);
  SymbolTableValue value7 = createTableValue(SYMBOL_TYPE_LITERAL, lexicalValue7);
  SymbolTableValue value8 = createTableValue(SYMBOL_TYPE_LITERAL, lexicalValue8);

  addSymbol(symbolTable, value1);
  addSymbol(symbolTable, value2);
  addSymbol(symbolTable, value3);
  addSymbol(symbolTable, value4);
  addSymbol(symbolTable, value5);
  addSymbol(symbolTable, value6);
  addSymbol(symbolTable, value7);
  addSymbol(symbolTable, value8);

  SymbolTableValue retrievedValue = getValue(symbolTable, lexicalValue1.label);
  printDebug(retrievedValue.lexicalValue.label);

  retrievedValue = getValue(symbolTable, lexicalValue2.label);
  printDebug(retrievedValue.lexicalValue.label);

  retrievedValue = getValue(symbolTable, lexicalValue3.label);
  printDebug(retrievedValue.lexicalValue.label);

  retrievedValue = getValue(symbolTable, lexicalValue4.label);
  printDebug(retrievedValue.lexicalValue.label);

  retrievedValue = getValue(symbolTable, lexicalValue5.label);
  printDebug(retrievedValue.lexicalValue.label);

  retrievedValue = getValue(symbolTable, lexicalValue6.label);
  printDebug(retrievedValue.lexicalValue.label);

  retrievedValue = getValue(symbolTable, lexicalValue7.label);
  printDebug(retrievedValue.lexicalValue.label);

  retrievedValue = getValue(symbolTable, lexicalValue8.label);
  printDebug(retrievedValue.lexicalValue.label);

  printf("Table size: %d \n", symbolTable->size);
  printf("Table capacity: %d \n", symbolTable->capacity);

  return 1;
  // int ret = yyparse(); 
  // exporta (arvore);
  // libera(arvore);
  // arvore = NULL;
  // yylex_destroy();
  // return ret;
}