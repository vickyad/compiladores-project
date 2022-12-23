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

// =======================
// =        Tipos        =
// =======================
type: TK_PR_INT;
type: TK_PR_FLOAT;
type: TK_PR_BOOL;
type: TK_PR_CHAR;


// =======================
// =  Variaveis globais  =
// =======================
declaration: type var_list ';';

var_list: TK_IDENTIFICADOR;
var_list: array;
var_list: TK_IDENTIFICADOR ',' var_list;
var_list: array ',' var_list;

// Arranjos
array: TK_IDENTIFICADOR ':' dimension;
dimension: TK_LIT_INT;
dimension: TK_LIT_INT '^' dimension;


// =======================
// =       Funcoes       =
// =======================
function: header body;
header: type TK_IDENTIFICADOR argumments;
body: command_block;

// Lista de argumentos
argumments: '(' ')';
argumments: '(' arg_list ')';
arg_list: type TK_IDENTIFICADOR;
arg_list: type TK_IDENTIFICADOR ',' arg_list;


// =======================
// =  Bloco de comandos  =
// =======================
command_block: '{' '}';
command_block: '{' simple_command_list '}';
simple_command_list: simple_command;
simple_command_list: simple_command simple_command_list;

// =======================
// =  Comandos simples   =
// =======================
simple_command: var_declaration ';';
simple_command: attribution ';';
simple_command: function_call ';';
simple_command: return_command ';';
simple_command: flow_control_commands ';';
simple_command: command_block;

var_declaration: type var_decl_list;
var_decl_list: TK_IDENTIFICADOR;
var_decl_list: TK_IDENTIFICADOR TK_OC_LE literal;
var_decl_list: TK_IDENTIFICADOR ',' var_decl_list;
var_decl_list: TK_IDENTIFICADOR TK_OC_LE literal ',' var_decl_list;
literal: TK_LIT_INT;
literal: TK_LIT_FLOAT;
literal: TK_LIT_FALSE;
literal: TK_LIT_TRUE;
literal: TK_LIT_CHAR;

attribution: '-';
function_call: '%';
return_command: '!';
flow_control_commands: '^';


// =======================
// =     Expressoes      =
// =======================
%%

void yyerror (const char *message) {
	printf("Erro sintatico [%s] na linha %d\n", message, get_line_number());
	return;
}
