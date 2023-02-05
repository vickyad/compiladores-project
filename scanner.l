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

"int"    { yylval.LexicalValue = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_INT; }
"float"  { yylval.LexicalValue = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_FLOAT; }
"bool"   { yylval.LexicalValue = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_BOOL; }
"char"   { yylval.LexicalValue = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_CHAR; }
"if"     { yylval.LexicalValue = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_IF; }
"then"   { yylval.LexicalValue = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_THEN; }
"else"   { yylval.LexicalValue = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_ELSE; }
"while"  { yylval.LexicalValue = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_WHILE; }
"input"  { yylval.LexicalValue = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_INPUT; }
"output" { yylval.LexicalValue = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_OUTPUT; }
"return" { yylval.LexicalValue = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_RETURN; }
"for"    { yylval.LexicalValue = createLexicalValue(yytext, RESERVED_WORD, IS_NOT_LITERAL, yylineno); return TK_PR_FOR; }

"true"   { yylval.LexicalValue = createLexicalValue(yytext, LITERAL, BOOL, yylineno); return TK_LIT_TRUE; }
"false"  { yylval.LexicalValue = createLexicalValue(yytext, LITERAL, BOOL, yylineno); return TK_LIT_FALSE; }

"<="     { yylval.LexicalValue = createLexicalValue(yytext, COMPOUND_OPERATOR, IS_NOT_LITERAL, yylineno); return TK_OC_LE; }
">="     { yylval.LexicalValue = createLexicalValue(yytext, COMPOUND_OPERATOR, IS_NOT_LITERAL, yylineno); return TK_OC_GE; }
"=="     { yylval.LexicalValue = createLexicalValue(yytext, COMPOUND_OPERATOR, IS_NOT_LITERAL, yylineno); return TK_OC_EQ; }
"!="     { yylval.LexicalValue = createLexicalValue(yytext, COMPOUND_OPERATOR, IS_NOT_LITERAL, yylineno); return TK_OC_NE; }
"&&"     { yylval.LexicalValue = createLexicalValue(yytext, COMPOUND_OPERATOR, IS_NOT_LITERAL, yylineno); return TK_OC_AND; }
"||"     { yylval.LexicalValue = createLexicalValue(yytext, COMPOUND_OPERATOR, IS_NOT_LITERAL, yylineno); return TK_OC_OR; }


[a-zA-Z]+ { yylval.LexicalValue = createLexicalValue(yytext, IDENTIFIER, IS_NOT_LITERAL, yylineno); return TK_IDENTIFICADOR; }
[0-9]+ { yylval.LexicalValue = createLexicalValue(yytext, LITERAL, INT, yylineno); return TK_LIT_INT; }
{float_literal} { yylval.LexicalValue = createLexicalValue(yytext, LITERAL, FLOAT, yylineno); return TK_LIT_FLOAT; }
'.?' { yylval.LexicalValue = createLexicalValue(yytext, LITERAL, CHAR, yylineno); return TK_LIT_CHAR; }
{inline_comment} {}
{multiline_comment} {} 

{special_char} { yylval.LexicalValue = createLexicalValue(yytext, SPECIAL_CHARACTER, IS_NOT_LITERAL, yylineno); return yytext[0]; }

. {return TK_ERRO;} /* reconhece todos os outros tokens que não se enquadraram nas outras regras */
%%

/* Opcional, para código */

int get_line_number() {
    return yylineno;
}
