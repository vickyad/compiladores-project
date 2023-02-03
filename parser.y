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
   LexicalValue lexicalValue;
   struct Node* node;
}

%define parse.error verbose

%token<LexicalValue>  TK_PR_INT
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

program:  { $$ = NULL; };
program: elements_list { $$ = $1; };

elements_list: elements_list function {  };
elements_list: elements_list global_declaration { };
elements_list: function { $$ = createNode(FUNCTION, "function"); };
elements_list: global_declaration { $$ = createNode(GLOBAL_DECLARATION, "global_declaration"); };

// =======================
// =        Tipos        =
// =======================
type: TK_PR_INT { $$ = create_leaf(TYPE, $1); };
type: TK_PR_FLOAT { $$ = create_leaf(TYPE, $1); };
type: TK_PR_BOOL { $$ = create_leaf(TYPE, $1); };
type: TK_PR_CHAR { $$ = create_leaf(TYPE, $1); };


// =======================
// =      Literais       =
// =======================
literal: TK_LIT_INT { $$ = create_leaf(LITERAL, $1); };
literal: TK_LIT_FLOAT { $$ = create_leaf(LITERAL, $1); };
literal: TK_LIT_FALSE { $$ = create_leaf(LITERAL, $1); };
literal: TK_LIT_TRUE { $$ = create_leaf(LITERAL, $1); };
literal: TK_LIT_CHAR { $$ = create_leaf(LITERAL, $1); };


// =======================
// =  Variaveis globais  =
// =======================
global_declaration: type var_list ';' {};

var_list: TK_IDENTIFICADOR { $$ = create_leaf(IDENTIFICADOR, $1); };
var_list: array { $$ = $1; };
var_list: TK_IDENTIFICADOR ',' var_list { $$ = create_leaf(IDENTIFICADOR, $1); add_child($1, $2); $$ = $1; };
var_list: array ',' var_list { add_child($1, $2); $$ = $1; } ;

// Arranjos
array: TK_IDENTIFICADOR '[' dimension ']' {}; 
dimension: TK_LIT_INT { $$ = create_leaf(LITERAL, $1); };
dimension: TK_LIT_INT '^' dimension {};


// =======================
// =       Funcoes       =
// =======================
function: header body {};
header: type TK_IDENTIFICADOR argumments {};
body: command_block { $$ = $1; };

// Lista de argumentos
argumments: '(' ')' { $$ = NULL; };
argumments: '(' arg_list ')' { $$ = $2; };
arg_list: type TK_IDENTIFICADOR {};
arg_list: type TK_IDENTIFICADOR ',' arg_list {};


// =======================
// =  Bloco de comandos  =
// =======================
command_block: '{' '}' { $$ = NULL; };
command_block: '{' simple_command_list '}' { $$ = $2; };
simple_command_list: simple_command { $$ = $1; };
simple_command_list: simple_command simple_command_list {};


// =======================
// =  Comandos simples   =
// =======================
simple_command: var_declaration ';' { $$ = $1; };
simple_command: attribution ';' { $$ = $1; };
simple_command: function_call ';' { $$ = $1; };
simple_command: return_command ';' { $$ = $1; };
simple_command: flow_control_commands ';' { $$ = $1; };
simple_command: command_block ';' { $$ = $1; };

// Declaracao de variavel
var_declaration: type var_decl_list {};
var_decl_list: TK_IDENTIFICADOR { $$ = create_leaf(IDENTIFICADOR, $1); };
var_decl_list: TK_IDENTIFICADOR TK_OC_LE literal { $$ = create_node(VAR_DECL, "var_decl"); create_leaf(IDENTIFICATOR, $1); add_child($$, $3); };
var_decl_list: TK_IDENTIFICADOR ',' var_decl_list { $$ = create_leaf(IDENTIFICADOR, $1);  add_child($$, $3); };
var_decl_list: TK_IDENTIFICADOR TK_OC_LE literal ',' var_decl_list {};

// Atribuicao
attribution: TK_IDENTIFICADOR '=' expression { $$ = create_node(ATTRIBUTION, "attr"); add_child($$, $1); add_child($$, $3) };
attribution: TK_IDENTIFICADOR '[' attr_array ']' '=' expression {};
attr_array: expression { $$ = $1; };
attr_array: expression '^' attr_array { $$ = $1; add_child($$, $1); };

// Chamada de funcao
function_call: TK_IDENTIFICADOR '(' ')' { $$ = create_node(FUNCTION_CALL, $1); add_child($$, NULL); };
function_call: TK_IDENTIFICADOR '(' arg_fn_list ')' { $$ = create_node(FUNCTION_CALL, $1); add_child($$, $3); };
arg_fn_list: expression { $$ = $1; };
arg_fn_list: expression ',' arg_fn_list { $$ = $1; add_child($$, $3); };

// Comando de retorno
return_command: TK_PR_RETURN expression { $$ = create_node(RETURN, "return"); add_child($$, $2); };

// Comando de controle de fluxo
flow_control_commands: TK_PR_IF '(' expression ')' TK_PR_THEN command_block { $$ = create_node(IF, "if"); add_child($$, $3); add_child($$, $5); add_child($$, NULL); };
flow_control_commands: TK_PR_IF '(' expression ')' TK_PR_THEN command_block TK_PR_ELSE command_block { $$ = create_node(IF, "if"); add_child($$, $3); add_child($$, $5); add_child($$, $7); };
flow_control_commands: TK_PR_WHILE '(' expression ')' command_block { $$ = create_node(WHILE, "while"); add_child($$, $3); add_child($$, $5); };


// =======================
// =     Expressoes      =
// =======================
expression: expression_grade_eight { $$ = $1; };

expression_grade_eight: expression_grade_eight TK_OC_OR expression_grade_seven { $$ = create_node(OR, "||"); add_child($$, $1); add_child($$, $3); };
expression_grade_eight: expression_grade_seven { $$ = $1; };

expression_grade_seven: expression_grade_seven TK_OC_AND expression_grade_six { $$ = create_node(AND, "&&"); add_child($$, $1); add_child($$, $3); };
expression_grade_seven: expression_grade_six { $$ = $1; };

expression_grade_six: expression_grade_six TK_OC_EQ expression_grade_five { $$ = create_node(EQ, "=="); add_child($$, $1); add_child($$, $3); };
expression_grade_six: expression_grade_six TK_OC_NE expression_grade_five { $$ = create_node(NE, "!="); add_child($$, $1); add_child($$, $3); };
expression_grade_six: expression_grade_five { $$ = $1; };

expression_grade_five: expression_grade_five '>' expression_grade_four { $$ = create_node(GT, ">"); add_child($$, $1); add_child($$, $3); };
expression_grade_five: expression_grade_five '<' expression_grade_four { $$ = create_node(LT, "<"); add_child($$, $1); add_child($$, $3); };
expression_grade_five: expression_grade_five TK_OC_LE expression_grade_four { $$ = create_node(LE, "<="); add_child($$, $1); add_child($$, $3); };
expression_grade_five: expression_grade_five TK_OC_GE expression_grade_four { $$ = create_node(GE, ">="); add_child($$, $1); add_child($$, $3); };
expression_grade_five: expression_grade_four { $$ = $1; };

expression_grade_four: expression_grade_four '+' expression_grade_three { $$ = create_node(ADD, "+"); add_child($$, $1); add_child($$, $3); };
expression_grade_four: expression_grade_four '-' expression_grade_three { $$ = create_node(MINUS, "-"); add_child($$, $1); add_child($$, $3); };
expression_grade_four: expression_grade_three { $$ = $1; };

expression_grade_three: expression_grade_three '*' expression_grade_two { $$ = create_node(MULTIPLY, "*"); add_child($$, $1); add_child($$, $3); };
expression_grade_three: expression_grade_three '/' expression_grade_two { $$ = create_node(DIVISION, "/"); add_child($$, $1); add_child($$, $3); };
expression_grade_three: expression_grade_three '%' expression_grade_two { $$ = create_node(REMAINDER, "%"); add_child($$, $1); add_child($$, $3); };
expression_grade_three: expression_grade_two { $$ = $1; } ;

expression_grade_two: '!' expression_grade_one { $$ = create_node(UNARY_NEGATION, "-"); add_child($$, $2); };
expression_grade_two: '-' expression_grade_one { $$ = create_node(UNARY_MINUS, "-"); add_child($$, $2); };
expression_grade_two: expression_grade_one { $$ = $1; };

expression_grade_one: TK_IDENTIFICADOR { $$ = create_leaf(IDENTIFICADOR, $1); };
expression_grade_one: TK_IDENTIFICADOR '[' expression_list ']' { $$ = create_node(SQUARE_BRACKETS, "[]"); add_child($$, $1), add_child($$, $3); };
expression_grade_one: literal { $$ = $1; };
expression_grade_one: function_call { $$ = $1; };
expression_grade_one: '(' expression ')' { $$ = create_node(BRACKETS, "()"); add_child($$, $2); };

expression_list: expression_list '^' expression { $$ = $1; add_child($$, $3); };
expression_list: expression { $$ = $1; };
%%

void yyerror (const char *message) {
	printf("Erro sintatico [%s] na linha %d\n", message, get_line_number());
	return;
}
