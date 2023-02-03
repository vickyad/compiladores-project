COMPILER = gcc

main: main.c
	bison -d parser.y
	flex scanner.l
	$(COMPILER) main.c lexical_value.c generic_tree.c parser.tab.c lex.yy.c -c # gera main.o
	$(COMPILER) main.o lexical_value.o generic_tree.o parser.tab.o lex.yy.o -o etapa2

debug: main.c
	bison -d parser.y --report-file parser.output -Wcounterexamples
	flex scanner.l
	$(COMPILER) main.c lexical_value.c generic_tree.c parser.tab.c lex.yy.c -c # gera main.o
	$(COMPILER) main.o lexical_value.o generic_tree.o parser.tab.o lex.yy.o -o etapa2

scanner: scanner.l
	flex scanner.l
	$(COMPILER) lex.yy.c -c #gera lex.yy.o

all: main scanner
	$(COMPILER) lex.yy.o main.o -o etapa2