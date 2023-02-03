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

"int"    { yylval.lexicalValue = createLexicalValue(yytext, RESERVED_WORD, NULL, yylineno); return TK_PR_INT; }
"float"  { yylval.lexicalValue = createLexicalValue(yytext, RESERVED_WORD, NULL, yylineno); return TK_PR_FLOAT; }
"bool"   { yylval.lexicalValue = createLexicalValue(yytext, RESERVED_WORD, NULL, yylineno); return TK_PR_BOOL; }
"char"   { yylval.lexicalValue = createLexicalValue(yytext, RESERVED_WORD, NULL, yylineno); return TK_PR_CHAR; }
"if"     { yylval.lexicalValue = createLexicalValue(yytext, RESERVED_WORD, NULL, yylineno); return TK_PR_IF; }
"then"   { yylval.lexicalValue = createLexicalValue(yytext, RESERVED_WORD, NULL, yylineno); return TK_PR_THEN; }
"else"   { yylval.lexicalValue = createLexicalValue(yytext, RESERVED_WORD, NULL, yylineno); return TK_PR_ELSE; }
"while"  { yylval.lexicalValue = createLexicalValue(yytext, RESERVED_WORD, NULL, yylineno); return TK_PR_WHILE; }
"input"  { yylval.lexicalValue = createLexicalValue(yytext, RESERVED_WORD, NULL, yylineno); return TK_PR_INPUT; }
"output" { yylval.lexicalValue = createLexicalValue(yytext, RESERVED_WORD, NULL, yylineno); return TK_PR_OUTPUT; }
"return" { yylval.lexicalValue = createLexicalValue(yytext, RESERVED_WORD, NULL, yylineno); return TK_PR_RETURN; }
"for"    { yylval.lexicalValue = createLexicalValue(yytext, RESERVED_WORD, NULL, yylineno); return TK_PR_FOR; }

"true"   { yylval.lexicalValue = createLexicalValue(yytext, LITERAL, BOOL, yylineno); return TK_LIT_TRUE; }
"false"  { yylval.lexicalValue = createLexicalValue(yytext, LITERAL, BOOL, yylineno); return TK_LIT_FALSE; }

"<="     { yylval.lexicalValue = createLexicalValue(yytext, COMPOUND_OPERATOR, NULL, yylineno); return TK_OC_LE; }
">="     { yylval.lexicalValue = createLexicalValue(yytext, COMPOUND_OPERATOR, NULL, yylineno); return TK_OC_GE; }
"=="     { yylval.lexicalValue = createLexicalValue(yytext, COMPOUND_OPERATOR, NULL, yylineno); return TK_OC_EQ; }
"!="     { yylval.lexicalValue = createLexicalValue(yytext, COMPOUND_OPERATOR, NULL, yylineno); return TK_OC_NE; }
"&&"     { yylval.lexicalValue = createLexicalValue(yytext, COMPOUND_OPERATOR, NULL, yylineno); return TK_OC_AND; }
"||"     { yylval.lexicalValue = createLexicalValue(yytext, COMPOUND_OPERATOR, NULL, yylineno); return TK_OC_OR; }


[a-zA-Z]+ { yylval.lexicalValue = createLexicalValue(yytext, IDENTIFIER, NULL, yylineno); return TK_IDENTIFICADOR; }
[0-9]+ { yylval.lexicalValue = createLexicalValue(yytext, LITERAL, INT, yylineno); return TK_LIT_INT; }
{float_literal} { yylval.lexicalValue = createLexicalValue(yytext, LITERAL, FLOAT, yylineno); return TK_LIT_FLOAT; }
'.?' { yylval.lexicalValue = createLexicalValue(yytext, LITERAL, CHAR, yylineno); return TK_LIT_CHAR; }
{inline_comment} {}
{multiline_comment} {} 

{special_char} {yylval.lexicalValue = createLexicalValue(yytext, SPECIAL_CHARACTER, NULL, yylineno); return yytext[0];}

. {return TK_ERRO;} /* reconhece todos os outros tokens que não se enquadraram nas outras regras */
%%

/* Opcional, para código */

int get_line_number() {
    return yylineno;
}
