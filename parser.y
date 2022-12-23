%{
#include <stdio.h>

int yylex(void);
void yyerror (char const *message);
int get_line_number();
%}

%define parse.error verbose

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_PR_FOR
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_IDENTIFICADOR
%token TK_ERRO

%start program

%%

program: ;
program: elements_list;

elements_list: elements_list function;
elements_list: elements_list declaration;
elements_list: function;
elements_list: declaration;

type: TK_PR_INT;
type: TK_PR_FLOAT;
type: TK_PR_BOOL;
type: TK_PR_CHAR;

function: type ';';

declaration: type var_list ';';
var_list: TK_IDENTIFICADOR;
var_list: TK_IDENTIFICADOR ',' var_list;

%%

void yyerror (const char *message) {
	printf("Erro sintatico [%s] na linha %d\n", message, get_line_number());
	return;
}
