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

/* Expr. regulares e acoes */

%%
{espaco} {} /* Todo espaço ou \n vai ser ignorado */

"int" { return TK_PR_INT; }
"float" { return TK_PR_FLOAT; }
"bool" { return TK_PR_BOOL; }
"char" { return TK_PR_CHAR; }
"if" { return TK_PR_IF; }
"then"

[<=] { return TK_OC_LE; }
[>=] { return TK_OC_GE; }
[==] { return TK_OC_EQ; }
[!=] { return TK_OC_NE; }
[&&] { return TK_OC_AND; }
[||] { return TK_OC_OR; }

[\/*([^"]*)*\/] {}

[a-z]+ {return TK_IDENTIFICADOR;}
[;:\(\){}|\-\+%\^!<>=*/] {return yytext[0];}

. {return TK_ERRO;} /* reconhece todos os outros tokens que não se enquadraram nas outras regras */
%%

/* Opcional, para código */

int get_line_number() {
    return yylineno;
}