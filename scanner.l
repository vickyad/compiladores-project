/* João Pedro Silveira e Silva e Victoria Duarte */
/* Definições */
%{
    #include "tokens.h"
    #define MEU_TOKEN 300
    int global = 0;
%}

%option noyywrap
/* não precisa do -lfl depois pra compilar */

%option yylineno
/* habilita contador de linhas */

espaco [ \t\n\r]
special_char [;:\(\){}|\-\+%\^!<>=\*\/]
multiline_comment \/\*[^"]*\*\/
inline_comment \/\/[^\n\r]+?(?:\*\)|[\n\r])
float_literal ((\+|\-)?[0-9]+)(.?)([0-9]*)(((e|E))((\+|\-)?[0-9]+))?

/* Expr. regulares e acoes */

%%
{espaco} {} /* Todo espaço ou \n vai ser ignorado */

"int"    { return TK_PR_INT; }
"float"  { return TK_PR_FLOAT; }
"bool"   { return TK_PR_BOOL; }
"char"   { return TK_PR_CHAR; }
"if"     { return TK_PR_IF; }
"then"   { return TK_PR_THEN; }
"else"   { return TK_PR_ELSE; }
"while"  { return TK_PR_WHILE; }
"input"  { return TK_PR_INPUT; }
"output" { return TK_PR_OUTPUT; }
"return" { return TK_PR_RETURN; }
"for"    { return TK_PR_FOR; }

"true"   { return TK_LIT_TRUE; }
"false"  { return TK_LIT_FALSE; }

"<="     { return TK_OC_LE; }
">="     { return TK_OC_GE; }
"=="     { return TK_OC_EQ; }
"!="     { return TK_OC_NE; }
"&&"     { return TK_OC_AND; }
"||"     { return TK_OC_OR; }


[a-zA-Z]+ { return TK_IDENTIFICADOR; }
(\+|\-)?[0-9]+ { return TK_LIT_INT; }
{float_literal} { return TK_LIT_FLOAT; }
'([a-zA-Z]|{special_char})?' { return TK_LIT_CHAR; }
{inline_comment} {}
{multiline_comment} {} 

{special_char} {return yytext[0];}

. {return TK_ERRO;} /* reconhece todos os outros tokens que não se enquadraram nas outras regras */
%%

/* Opcional, para código */

int get_line_number() {
    return yylineno;
}
