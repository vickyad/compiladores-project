%{
#include <stdio.h>

int yylex(void);
void yyerror (char const *message);
int get_line_number();
%}

%code requires {
    #include "generic_tree.h"
    #include "lexical_value.h"
}

%union {
   LexicalValue valor_lexico;
   struct Node* node;
}

%define parse.error verbose

%token<LexicalValue> TK_PR_INT
%token<LexicalValue> TK_PR_FLOAT
%token<LexicalValue> TK_PR_BOOL
%token<LexicalValue> TK_PR_CHAR
%token<LexicalValue> TK_PR_IF
%token<LexicalValue> TK_PR_THEN
%token<LexicalValue> TK_PR_ELSE
%token<LexicalValue> TK_PR_WHILE
%token<LexicalValue> TK_PR_INPUT
%token<LexicalValue> TK_PR_OUTPUT
%token<LexicalValue> TK_PR_RETURN
%token<LexicalValue> TK_PR_FOR
%token<LexicalValue> TK_OC_LE
%token<LexicalValue> TK_OC_GE
%token<LexicalValue> TK_OC_EQ
%token<LexicalValue> TK_OC_NE
%token<LexicalValue> TK_OC_AND
%token<LexicalValue> TK_OC_OR
%token<LexicalValue> TK_LIT_INT
%token<LexicalValue> TK_LIT_FLOAT
%token<LexicalValue> TK_LIT_FALSE
%token<LexicalValue> TK_LIT_TRUE
%token<LexicalValue> TK_LIT_CHAR
%token<LexicalValue> TK_IDENTIFICADOR
%token TK_ERRO

%type<Node> program
%type<Node> elements_list
%type<Node> type
%type<Node> literal
%type<Node> global_declaration
%type<Node> var_list
%type<Node> array
%type<Node> dimension
%type<Node> function
%type<Node> header
%type<Node> body
%type<Node> argumments
%type<Node> arg_list
%type<Node> command_block
%type<Node> simple_command_list
%type<Node> simple_command
%type<Node> var_declaration
%type<Node> var_decl_list
%type<Node> attribution
%type<Node> attr_array
%type<Node> function_call
%type<Node> arg_fn_list
%type<Node> return_command
%type<Node> flow_control_commands
%type<Node> expression
%type<Node> expression_grade_eight
%type<Node> expression_grade_seven
%type<Node> expression_grade_six
%type<Node> expression_grade_five
%type<Node> expression_grade_four
%type<Node> expression_grade_three
%type<Node> expression_grade_two
%type<Node> expression_grade_one
%type<Node> expression_list

%start program

%%

program:  {  };
program: elements_list { };

elements_list: elements_list function {  };
elements_list: elements_list global_declaration { };
elements_list: function {  };
elements_list: global_declaration {  };

// =======================
// =        Tipos        =
// =======================
type: TK_PR_INT {  };
type: TK_PR_FLOAT {  };
type: TK_PR_BOOL {  };
type: TK_PR_CHAR {  };


// =======================
// =      Literais       =
// =======================
literal: TK_LIT_INT {  };
literal: TK_LIT_FLOAT {  };
literal: TK_LIT_FALSE {  };
literal: TK_LIT_TRUE {  };
literal: TK_LIT_CHAR {  };


// =======================
// =  Variaveis globais  =
// =======================
global_declaration: type var_list ';' { };

var_list: TK_IDENTIFICADOR {  };
var_list: array { };
var_list: TK_IDENTIFICADOR ',' var_list { };
var_list: array ',' var_list {  } ;

// Arranjos
array: TK_IDENTIFICADOR '[' dimension ']' {}; 
dimension: TK_LIT_INT {  };
dimension: TK_LIT_INT '^' dimension {};


// =======================
// =       Funcoes       =
// =======================
function: header body {};
header: type TK_IDENTIFICADOR argumments {};
body: command_block {  };

// Lista de argumentos
argumments: '(' ')' {  };
argumments: '(' arg_list ')' {  };
arg_list: type TK_IDENTIFICADOR {};
arg_list: type TK_IDENTIFICADOR ',' arg_list {};


// =======================
// =  Bloco de comandos  =
// =======================
command_block: '{' '}' {  };
command_block: '{' simple_command_list '}' {  };
simple_command_list: simple_command {  };
simple_command_list: simple_command simple_command_list {};


// =======================
// =  Comandos simples   =
// =======================
simple_command: var_declaration ';' {  };
simple_command: attribution ';' {  };
simple_command: function_call ';' {  };
simple_command: return_command ';' {  };
simple_command: flow_control_commands ';' {  };
simple_command: command_block ';' {  };

// Declaracao de variavel
var_declaration: type var_decl_list {};
var_decl_list: TK_IDENTIFICADOR {  };
var_decl_list: TK_IDENTIFICADOR TK_OC_LE literal { };
var_decl_list: TK_IDENTIFICADOR ',' var_decl_list {  };
var_decl_list: TK_IDENTIFICADOR TK_OC_LE literal ',' var_decl_list {};

// Atribuicao
attribution: TK_IDENTIFICADOR '=' expression {  };
attribution: TK_IDENTIFICADOR '[' attr_array ']' '=' expression {};
attr_array: expression {  };
attr_array: expression '^' attr_array { };

// Chamada de funcao
function_call: TK_IDENTIFICADOR '(' ')' {  };
function_call: TK_IDENTIFICADOR '(' arg_fn_list ')' {  };
arg_fn_list: expression {  };
arg_fn_list: expression ',' arg_fn_list {  };

// Comando de retorno
return_command: TK_PR_RETURN expression {  };

// Comando de controle de fluxo
flow_control_commands: TK_PR_IF '(' expression ')' TK_PR_THEN command_block { };
flow_control_commands: TK_PR_IF '(' expression ')' TK_PR_THEN command_block TK_PR_ELSE command_block {  };
flow_control_commands: TK_PR_WHILE '(' expression ')' command_block {  };


// =======================
// =     Expressoes      =
// =======================
expression: expression_grade_eight {  };

expression_grade_eight: expression_grade_eight TK_OC_OR expression_grade_seven {  };
expression_grade_eight: expression_grade_seven { };

expression_grade_seven: expression_grade_seven TK_OC_AND expression_grade_six {  };
expression_grade_seven: expression_grade_six { };

expression_grade_six: expression_grade_six TK_OC_EQ expression_grade_five {  };
expression_grade_six: expression_grade_six TK_OC_NE expression_grade_five {  };
expression_grade_six: expression_grade_five { };

expression_grade_five: expression_grade_five '>' expression_grade_four {  };
expression_grade_five: expression_grade_five '<' expression_grade_four {  };
expression_grade_five: expression_grade_five TK_OC_LE expression_grade_four {  };
expression_grade_five: expression_grade_five TK_OC_GE expression_grade_four {  };
expression_grade_five: expression_grade_four { };

expression_grade_four: expression_grade_four '+' expression_grade_three {  };
expression_grade_four: expression_grade_four '-' expression_grade_three {  };
expression_grade_four: expression_grade_three { };

expression_grade_three: expression_grade_three '*' expression_grade_two { };
expression_grade_three: expression_grade_three '/' expression_grade_two {  };
expression_grade_three: expression_grade_three '%' expression_grade_two {  };
expression_grade_three: expression_grade_two { } ;

expression_grade_two: '!' expression_grade_one { };
expression_grade_two: '-' expression_grade_one {  };
expression_grade_two: expression_grade_one { };

expression_grade_one: TK_IDENTIFICADOR {  };
expression_grade_one: TK_IDENTIFICADOR '[' expression_list ']' {  };
expression_grade_one: literal { };
expression_grade_one: function_call { };
expression_grade_one: '(' expression ')' {  };

expression_list: expression_list '^' expression {  };
expression_list: expression { };
%%

void yyerror (const char *message) {
	printf("Erro sintatico [%s] na linha %d\n", message, get_line_number());
	return;
}
