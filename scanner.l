/* João Pedro Silveira e Silva e Victoria Duarte */
/* Definições */
%{
    #include <stdio.h>
    #include "generic_tree.h"
    #include "lexical_value.h"
	#include "parser.tab.h"
    #define YYSTYPE LexicalValue
%}

%option noyywrap
/* não precisa do -lfl depois pra compilar */

%option yylineno
/* habilita contador de linhas */

espaco [ \t\n\r]
special_char [,;:\(\){}|\-\+%\^!<>=\*\/\[\]]
multiline_comment \/\*(\/)*([^/]|[^*/](\/)+)*\*\/
inline_comment \/\/[^\n\r]+?(?:\*\)|[\n\r])
float_literal ([0-9]+)[.]([0-9]+)(((e|E))((\+|\-)?[0-9]+))?

/* Expr. regulares e acoes */

%%
{espaco} {} /* Todo espaço ou \n vai ser ignorado */

"int"    { yylval.valor_lexico = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_INT; }
"float"  { yylval.valor_lexico = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_FLOAT; }
"bool"   { yylval.valor_lexico = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_BOOL; }
"char"   { yylval.valor_lexico = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_CHAR; }
"if"     { yylval.valor_lexico = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_IF; }
"then"   { yylval.valor_lexico = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_THEN; }
"else"   { yylval.valor_lexico = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_ELSE; }
"while"  { yylval.valor_lexico = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_WHILE; }
"input"  { yylval.valor_lexico = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_INPUT; }
"output" { yylval.valor_lexico = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_OUTPUT; }
"return" { yylval.valor_lexico = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_RETURN; }
"for"    { yylval.valor_lexico = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_FOR; }

"true"   { yylval.valor_lexico = createLexicalValue(yytext, LITERAL, BOOL, yylineno); return TK_LIT_TRUE; }
"false"  { yylval.valor_lexico = createLexicalValue(yytext, LITERAL, BOOL, yylineno); return TK_LIT_FALSE; }

"<="     { yylval.valor_lexico = createLexicalValue(yytext, COMPOUND_OPERATOR, IS_NOT_LITERAL, yylineno); return TK_OC_LE; }
">="     { yylval.valor_lexico = createLexicalValue(yytext, COMPOUND_OPERATOR, IS_NOT_LITERAL, yylineno); return TK_OC_GE; }
"=="     { yylval.valor_lexico = createLexicalValue(yytext, COMPOUND_OPERATOR, IS_NOT_LITERAL, yylineno); return TK_OC_EQ; }
"!="     { yylval.valor_lexico = createLexicalValue(yytext, COMPOUND_OPERATOR, IS_NOT_LITERAL, yylineno); return TK_OC_NE; }
"&&"     { yylval.valor_lexico = createLexicalValue(yytext, COMPOUND_OPERATOR, IS_NOT_LITERAL, yylineno); return TK_OC_AND; }
"||"     { yylval.valor_lexico = createLexicalValue(yytext, COMPOUND_OPERATOR, IS_NOT_LITERAL, yylineno); return TK_OC_OR; }


[a-zA-Z]+ { yylval.valor_lexico = createLexicalValue(yytext, IDENTIFIER, IS_NOT_LITERAL, yylineno); return TK_IDENTIFICADOR; }
[0-9]+ { yylval.valor_lexico = createLexicalValue(yytext, LITERAL, INT, yylineno); return TK_LIT_INT; }
{float_literal} { yylval.valor_lexico = createLexicalValue(yytext, LITERAL, FLOAT, yylineno); return TK_LIT_FLOAT; }
'.?' { yylval.valor_lexico = createLexicalValue(yytext, LITERAL, CHAR, yylineno); return TK_LIT_CHAR; }
{inline_comment} {}
{multiline_comment} {} 

{special_char} {yylval.valor_lexico = createLexicalValue(yytext, SPECIAL_CHARACTER, IS_NOT_LITERAL, yylineno); return yytext[0];}

. {return TK_ERRO;} /* reconhece todos os outros tokens que não se enquadraram nas outras regras */
%%

/* Opcional, para código */

int get_line_number() {
    return yylineno;
}
