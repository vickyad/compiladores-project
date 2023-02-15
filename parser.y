%{
#include <stdio.h>
#include "types.h"
#include "data_type.h"
#include "generic_tree.h"
#include "lexical_value.h"
#include "symbol_table.h"
#include "print.h"

DataType declaredType = DATA_TYPE_NON_DECLARED;

int yylex(void);
void yyerror (char const *message);
int get_line_number();
extern void *arvore;
extern void *symbolTableStack;
%}

%code requires {
    #include "generic_tree.h"
    #include "lexical_value.h"
    #include "symbol_table.h"
}

%union {
   LexicalValue LexicalValue;
   struct Node* Node;
   Dimension Dimension;
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
%token<LexicalValue> ',' ';' ':' '(' ')' '{' '}' '|' '-' '+' '%' '^' '!' '<' '>' '=' '*' '/' '[' ']' 
%token TK_ERRO

%type<Node> program
%type<Node> elements_list
%type<Node> type
%type<Node> literal
%type<Node> global_declaration
%type<Node> var_list
%type<Node> array
%type<Dimension> dimension
%type<Node> function
%type<Node> header
%type<Node> body
%type<Node> arguments
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
%type<Node> start_flow_control_block
%type<Node> flow_control_else
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
program:  { 
    $$ = NULL; 
    arvore = NULL; 
};

program: elements_list { 
    $$ = $1; 
    arvore = $$;
};

elements_list: function elements_list { 
    $$ = $1;
    addChild($$, $2);
};

elements_list: global_declaration elements_list {
    $$ = $2;
};

elements_list: function { 
    $$ = $1;
};

elements_list: global_declaration { 
    $$ = NULL;
};


// =======================
// =        Tipos        =
// =======================
type: TK_PR_INT { 
    $$ = NULL; 
    freeLexicalValue($1);
    declaredType = DATA_TYPE_INT;
};

type: TK_PR_FLOAT { 
    $$ = NULL; 
    freeLexicalValue($1);
    declaredType = DATA_TYPE_FLOAT;
};

type: TK_PR_BOOL { 
    $$ = NULL; 
    freeLexicalValue($1);
    declaredType = DATA_TYPE_BOOL;
};

type: TK_PR_CHAR { 
    $$ = NULL; 
    freeLexicalValue($1);
    declaredType = DATA_TYPE_CHAR;
};


// =======================
// =      Literais       =
// =======================
literal: TK_LIT_INT { 
    $$ = createNodeWithType($1, DATA_TYPE_INT);
};

literal: TK_LIT_FLOAT { 
    $$ = createNodeWithType($1, DATA_TYPE_FLOAT);
};

literal: TK_LIT_FALSE { 
    $$ = createNodeWithType($1, DATA_TYPE_BOOL);
};

literal: TK_LIT_TRUE { 
    $$ = createNodeWithType($1, DATA_TYPE_BOOL);
};

literal: TK_LIT_CHAR { 
    $$ = createNodeWithType($1, DATA_TYPE_CHAR);
};


// =======================
// =  Variaveis globais  =
// =======================
global_declaration: type var_list ';' { 
    $$ = NULL;
    freeLexicalValue($3);
};

var_list: TK_IDENTIFICADOR { 
    $$ = NULL; 

    SymbolTableValue symbol = createSymbolTableValueWithType(SYMBOL_TYPE_VARIABLE, $1, declaredType);
    addValueToSymbolTableStack(symbolTableStack, symbol);
};

var_list: array { 
    $$ = NULL; 
};

var_list: TK_IDENTIFICADOR ',' var_list {
    $$ = NULL;
    freeLexicalValue($2);

    SymbolTableValue symbol = createSymbolTableValueWithType(SYMBOL_TYPE_VARIABLE, $1, declaredType);
    addValueToSymbolTableStack(symbolTableStack, symbol);
};

var_list: array ',' var_list { 
    $$ = NULL;
    freeLexicalValue($2);
} ;

// Arranjos
array: TK_IDENTIFICADOR '[' dimension ']' {
    $$ = NULL;
    freeLexicalValue($2);
    freeLexicalValue($4);

    SymbolTableValue symbol = createSymbolTableValueWithTypeAndSize(SYMBOL_TYPE_ARRAY, $1, declaredType, $3);
    addValueToSymbolTableStack(symbolTableStack, symbol);
}; 

dimension: TK_LIT_INT {
    $$ = getDimension($1);
    freeLexicalValue($1);
};

dimension: TK_LIT_INT '^' dimension {
    $$ = getDimensionMultipling($1, $3);
    freeLexicalValue($1);
    freeLexicalValue($2);
};


// =======================
// =       Funcoes       =
// =======================
function: header arguments body {
    $$ = $1;
    addChild($$, $3);
};

header: type TK_IDENTIFICADOR {
    $$ = createNodeWithType($2, declaredType);

    // First create function symbol on external context
    SymbolTableValue symbol = createSymbolTableValueWithType(SYMBOL_TYPE_FUNCTION, $2, declaredType);
    addValueToSymbolTableStack(symbolTableStack, symbol);

    // Then create a new internal context
    symbolTableStack = createNewTableOnSymbolTableStack(symbolTableStack);
};

body: command_block { 
    $$ = $1;
};

// Lista de argumentos
arguments: '(' ')' { 
    $$ = NULL;
    freeLexicalValue($1);
    freeLexicalValue($2);
};

arguments: '(' arg_list ')' { 
    $$ = NULL;
    freeLexicalValue($1);
    freeLexicalValue($3);
};

arg_list: type TK_IDENTIFICADOR {
    $$ = NULL;

    SymbolTableValue symbol = createSymbolTableValueWithType(SYMBOL_TYPE_VARIABLE, $2, declaredType);
    addValueToSymbolTableStack(symbolTableStack, symbol);
};

arg_list: type TK_IDENTIFICADOR ',' arg_list {
    $$ = NULL;
    freeLexicalValue($3);

    SymbolTableValue symbol = createSymbolTableValueWithType(SYMBOL_TYPE_VARIABLE, $2, declaredType);
    addValueToSymbolTableStack(symbolTableStack, symbol);
};


// =======================
// =  Bloco de comandos  =
// =======================
command_block: '{' '}' { 
    $$ = NULL;
    freeLexicalValue($1);
    freeLexicalValue($2);
    symbolTableStack = destroyFirstTableFromSymbolTableStack(symbolTableStack);
};

command_block: '{' simple_command_list '}' { 
    $$ = $2;
    freeLexicalValue($1);
    freeLexicalValue($3);
    symbolTableStack = destroyFirstTableFromSymbolTableStack(symbolTableStack);
};

simple_command_list: simple_command { 
    $$ = $1;
};

simple_command_list: simple_command simple_command_list {
    if ($1)
    {
        $$ = $1;
        addChild($$, $2);
    }
    else
    {        
        $$ = $2;
    }
};


// =======================
// =  Comandos simples   =
// =======================
simple_command: var_declaration ';' { 
    $$ = $1;
    freeLexicalValue($2);
};

simple_command: attribution ';' { 
    $$ = $1;
    freeLexicalValue($2);
};

simple_command: function_call ';' { 
    $$ = $1;
    freeLexicalValue($2);
};

simple_command: return_command ';' {
    $$ = $1;
    freeLexicalValue($2);
};

simple_command: flow_control_commands ';' { 
    $$ = $1;
    freeLexicalValue($2);
};

simple_command: command_block ';' { 
    $$ = $1;
    freeLexicalValue($2);
};

// Declaracao de variavel
var_declaration: type var_decl_list { 
    $$ = $2;
};

var_decl_list: TK_IDENTIFICADOR { 
    $$ = NULL;

    SymbolTableValue symbol = createSymbolTableValueWithType(SYMBOL_TYPE_VARIABLE, $1, declaredType);
    addValueToSymbolTableStack(symbolTableStack, symbol);
};

var_decl_list: TK_IDENTIFICADOR TK_OC_LE literal {
    $$ = createNodeWithType($2, declaredType);
    addChild($$, createNodeWithType($1, declaredType));
    addChild($$, $3);

    SymbolTableValue symbol = createSymbolTableValueWithType(SYMBOL_TYPE_VARIABLE, $1, declaredType);
    addValueToSymbolTableStack(symbolTableStack, symbol);
};

var_decl_list: TK_IDENTIFICADOR ',' var_decl_list { 
    $$ = $3;
    freeLexicalValue($2);

    SymbolTableValue symbol = createSymbolTableValueWithType(SYMBOL_TYPE_VARIABLE, $1, declaredType);
    addValueToSymbolTableStack(symbolTableStack, symbol);
};

var_decl_list: TK_IDENTIFICADOR TK_OC_LE literal ',' var_decl_list { 
    $$ = createNodeWithType($2, declaredType);
    addChild($$, createNodeWithType($1, declaredType));
    addChild($$, $3);
    addChild($$, $5);
    freeLexicalValue($4);

    SymbolTableValue symbol = createSymbolTableValueWithType(SYMBOL_TYPE_VARIABLE, $1, declaredType);
    addValueToSymbolTableStack(symbolTableStack, symbol);
};

// Atribuicao
attribution: TK_IDENTIFICADOR '=' expression { 
    SymbolTableValue symbol = getByLexicalValueOnSymbolTableStack(symbolTableStack, $1);

    Node* variable = createNodeFromSymbol($1, symbol);

    $$ = createNodeFromChild($2, variable); 
    addChild($$, variable);
    addChild($$, $3);
};

attribution: TK_IDENTIFICADOR '[' attr_array ']' '=' expression {
    SymbolTableValue symbol = getByLexicalValueOnSymbolTableStack(symbolTableStack, $1);

    Node* variable = createNodeFromSymbol($1, symbol);
    
    Node* array = createNodeFromChild($2, variable);
    addChild(array, variable); 
    addChild(array, $3);

    $$ = createNodeFromChild($5, array);
    addChild($$, array);
    addChild($$, $6);

    freeLexicalValue($4);
};

attr_array: attr_array '^' expression {  
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);
};

attr_array: expression { 
    $$ = $1;
};

// Chamada de funcao
function_call: TK_IDENTIFICADOR '(' ')' { 
    SymbolTableValue symbol = getByLexicalValueOnSymbolTableStack(symbolTableStack, $1);

    $$ = createNodeForFunctionCallFromSymbol($1, symbol);
    freeLexicalValue($2);
    freeLexicalValue($3);
};

function_call: TK_IDENTIFICADOR '(' arg_fn_list ')' { 
    SymbolTableValue symbol = getByLexicalValueOnSymbolTableStack(symbolTableStack, $1);

    $$ = createNodeForFunctionCallFromSymbol($1, symbol);
    addChild($$, $3);
    freeLexicalValue($2);
    freeLexicalValue($4);
};

arg_fn_list: expression { 
    $$ = $1;
};

arg_fn_list: expression ',' arg_fn_list { 
    $$ = $1;
    addChild($$, $3);
    freeLexicalValue($2);
};

// Comando de retorno
return_command: TK_PR_RETURN expression { 
    $$ = createNodeFromChild($1, $2);
    addChild($$, $2);
};

// Comando de controle de fluxo
flow_control_commands: TK_PR_IF '(' expression start_flow_control_block TK_PR_THEN command_block { 
    $$ = createNodeFromChild($1, $3);
    addChild($$, $3);
    addChild($$, $6);
    freeLexicalValue($2);
    freeLexicalValue($5);
};

flow_control_commands: TK_PR_IF '(' expression start_flow_control_block TK_PR_THEN command_block flow_control_else command_block { 
    $$ = createNodeFromChild($1, $3);
    addChild($$, $3);
    addChild($$, $6);
    addChild($$, $8);
    freeLexicalValue($2);
    freeLexicalValue($5);
};

flow_control_commands: TK_PR_WHILE '(' expression start_flow_control_block command_block { 
    $$ = createNodeFromChild($1, $3);
    addChild($$, $3);
    addChild($$, $5);
    freeLexicalValue($2);
};

start_flow_control_block: ')' {
    $$ = NULL;
    freeLexicalValue($1);
    symbolTableStack = createNewTableOnSymbolTableStack(symbolTableStack);
};

flow_control_else: TK_PR_ELSE {
    $$ = NULL;
    freeLexicalValue($1);
    symbolTableStack = createNewTableOnSymbolTableStack(symbolTableStack);
}
 
// =======================
// =     Expressoes      =
// =======================
expression: expression_grade_eight { 
    $$ = $1;
};

expression_grade_eight: expression_grade_eight TK_OC_OR expression_grade_seven { 
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);
};

expression_grade_eight: expression_grade_seven { 
    $$ = $1;
};

expression_grade_seven: expression_grade_seven TK_OC_AND expression_grade_six { 
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);
};

expression_grade_seven: expression_grade_six { 
    $$ = $1;
};

expression_grade_six: expression_grade_six TK_OC_EQ expression_grade_five { 
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);
};

expression_grade_six: expression_grade_six TK_OC_NE expression_grade_five { 
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);    
};

expression_grade_six: expression_grade_five { 
    $$ = $1;
};

expression_grade_five: expression_grade_five '>' expression_grade_four { 
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);
};

expression_grade_five: expression_grade_five '<' expression_grade_four { 
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);
};

expression_grade_five: expression_grade_five TK_OC_LE expression_grade_four { 
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);
};

expression_grade_five: expression_grade_five TK_OC_GE expression_grade_four { 
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);
};

expression_grade_five: expression_grade_four { 
    $$ = $1;
};

expression_grade_four: expression_grade_four '+' expression_grade_three { 
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);
};

expression_grade_four: expression_grade_four '-' expression_grade_three { 
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);
};

expression_grade_four: expression_grade_three {
    $$ = $1;
};

expression_grade_three: expression_grade_three '*' expression_grade_two {
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);
};

expression_grade_three: expression_grade_three '/' expression_grade_two { 
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);
};

expression_grade_three: expression_grade_three '%' expression_grade_two { 
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);
};

expression_grade_three: expression_grade_two {
    $$ = $1;
};

expression_grade_two: '!' expression_grade_one { 
    $$ = createNodeFromChild($1, $2); 
    addChild($$, $2);
};

expression_grade_two: '-' expression_grade_one { 
    $$ = createNodeFromChild($1, $2); 
    addChild($$, $2);
};

expression_grade_two: expression_grade_one {
    $$ = $1;
};

expression_grade_one: TK_IDENTIFICADOR { 
    SymbolTableValue symbol = getByLexicalValueOnSymbolTableStack(symbolTableStack, $1);

    $$ = createNodeFromSymbol($1, symbol);
};

expression_grade_one: TK_IDENTIFICADOR '[' expression_list ']' {
    SymbolTableValue symbol = getByLexicalValueOnSymbolTableStack(symbolTableStack, $1);

    Node* variable = createNodeFromSymbol($1, symbol);

    $$ = createNodeFromChild($2, variable);
    addChild($$, variable);
    addChild($$, $3);
    freeLexicalValue($4);
};

expression_grade_one: literal { 
    $$ = $1;
};

expression_grade_one: function_call {
    $$ = $1;
};

expression_grade_one: '(' expression ')' { 
    $$ = $2;
    freeLexicalValue($1);
    freeLexicalValue($3);
};

expression_list: expression_list '^' expression { 
    $$ = createNodeFromTwoChildren($2, $1, $3);
    addChild($$, $1);
    addChild($$, $3);        
};

expression_list: expression { 
    $$ = $1;
};
%%

void yyerror (const char *message) {
	printf("Erro sintatico [%s] na linha %d\n", message, get_line_number());
	return;
}
