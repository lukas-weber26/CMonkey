output: main.o tests.o lexer.o tokens.o
	cc main.o tests.o lexer.o tokens.o -o output

main.o: ast.c
	cc -c -g ast.c -o main.o

tests.o: tests.c
	cc -c -g tests.c -o tests.o

lexer.o: lexer.c
	cc -c -g lexer.c -o lexer.o

tokens.o: tokens.c
	cc -c -g tokens.c -o tokens.o

clean: 
	rm *.o output
