main: main.c
	gcc main.c -c # gera main.o

scanner: scanner.l
	flex scanner.l
	gcc lex.yy.c -c #gera lex.yy.o

all: main scanner
	gcc lex.yy.o main.o -o etapa1