main: main.c
	bison -d parser.y
	flex scanner.l
	gcc main.c parser.tab.c lex.yy.c -c # gera main.o
	gcc main.o parser.tab.o lex.yy.o -o etapa2

debug: main.c
	bison -d parser.y --report-file parser.output -Wcounterexamples
	flex scanner.l
	gcc main.c parser.tab.c lex.yy.c -c # gera main.o
	gcc main.o parser.tab.o lex.yy.o -o etapa2

scanner: scanner.l
	flex scanner.l
	gcc lex.yy.c -c #gera lex.yy.o

all: main scanner
	gcc lex.yy.o main.o -o etapa2