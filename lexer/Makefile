repl:
	cc ./repl.c -g -o repl
	echo "Repl complete."

fast:
	cc ./repl.c -g -Ofast -o repl
	echo "Repl complete (fast)."

test:
	cc ./lexer.c -g -D TEST=True -o lexer
	./lexer

fast_test:
	cc ./lexer.c -g -Ofast -D TEST=True -o lexer
	./lexer

