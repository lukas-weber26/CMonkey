#include "./tokens.c" 
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct lexer {
	char * input; 
	int position; 
	int read_position;
	char current_char;
} lexer ;

token * lexer_next_token(lexer * lexer);
void lexer_read_char(lexer * lexer);
token * lexer_next_token(lexer * lexer);
lexer * lexer_create(char * input);
token * lexer_next_token(lexer * lexer);
void free_token(token * token);
void free_lexer(lexer * lexer);

//start of testing 
typedef struct test_next_token_struct {
	int token_type;
	char * expected_literal;
} test_next_token_struct;

void lexer_test_next_token() {
	char * input = "=+(){},;";
	#define test_length 8
	test_next_token_struct tests [test_length+1] = {
		{ASSIGN,"="},
		{PLUS,"+"},
		{LPAREN,"("},
		{RPAREN,")"},
		{LBRACE,"{"},
		{RBRACE,"}"},
		{COMMA,","},
		{SEMICOLON,";"},
		{ENDFILE,"EOF"},
	};

	assert(strlen(input) == test_length);	
	lexer * test_lexer = lexer_create(input); //clean this up

	for (int i = 0; i < test_length+1; i++) {
		token * test_token = lexer_next_token(test_lexer); 
		assert(tests[i].token_type == test_token->token_type);
		assert(strcmp(tests[i].expected_literal, test_token->token_string) == 0);
		free_token(test_token);
	}

	free_lexer(test_lexer);
}
//end of testing 

int main() {
	lexer_test_next_token();	
}

void lexer_read_char(lexer * lexer) {
	if (lexer->read_position >= strlen(lexer->input)){
		lexer->current_char = '\0';
	} else {
		lexer->current_char = lexer->input[lexer->read_position];
	}
	lexer->position  = lexer->read_position;
	lexer->read_position ++;
}

lexer * lexer_create(char * input) {
	assert(strlen(input));
	lexer * new_lexer = malloc(sizeof(lexer));
	new_lexer->input = strdup(input);
	assert(strlen(new_lexer->input));
	new_lexer -> read_position = 0;
	lexer_read_char(new_lexer);
	return new_lexer;
}

token * lexer_next_token(lexer * lexer) {
	token * new_token = malloc(sizeof(token));
	new_token->token_type = token_string_to_token(lexer->current_char);
	new_token->token_string = strdup(token_token_to_string(new_token->token_type));
	lexer_read_char(lexer); 
	return new_token;
}

void free_token(token * token) {
	free(token->token_string);
	free(token);
}

void free_lexer(lexer * lexer) {
	free(lexer->input);
	free(lexer);
}
