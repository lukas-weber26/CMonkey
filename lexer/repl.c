#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.c"

int main() {
	
	printf(">> ");
	
	int nread;
	char * line = NULL;
	unsigned long len;

	while((nread = getline(&line, &len, stdin)) != -1) {
		lexer * line_lexer = lexer_create(line);
		token * test_token;

		while ((test_token = lexer_next_token(line_lexer))->token_type != ENDFILE) {
			printf("%s\n",test_token->token_string);
			free_token(test_token);
		}

		free_lexer(line_lexer);
		printf(">> ");
	}

	printf("Exiting Monkey repl. Bye!\n");
	return 0;

}
