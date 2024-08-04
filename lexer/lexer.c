#include "./tokens.c" 
#include <assert.h>
#include <ctype.h>
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
char * lexer_read_identifier(lexer * lexer);
char * lexer_read_number(lexer * lexer);
void lexer_read_char(lexer * lexer);
token * lexer_next_token(lexer * lexer);
lexer * lexer_create(char * input);
token * lexer_next_token(lexer * lexer);
void free_token(token * token);
void free_lexer(lexer * lexer);
void lexer_skip_whitespace(lexer * lexer);

//start of testing 
typedef struct test_next_token_struct {
	int token_type;
	char * expected_literal;
} test_next_token_struct;

void lexer_test_next_token() {
	char * input = "let five = 5;\n"
	"let ten = 10;\n"
	" \n"
	"let add = fn(x,y) {\n"
	"	x + y;\n"
	"};\n"
	"let result = add(five, ten);\n"
	"\n"
	"!-/*5;\n"
	"5 < 10 > 5; \n";

	//DANGER: THERE IS A CORE DIFFERENCE BETWEEN THIS AND THE BOOK:
	//In short, c strings do not contain EOF characters, hence no EOF character is detected.
	//EOF functionality can therefore not be tested effectively. Use an exernal file and maybe architect this 
	//so that the EOF token is not needed.

	#define test_length 36
	test_next_token_struct tests [test_length] = {
		{LET,"let"},
		{IDENT,"five"},
		{ASSIGN,"="},
		{INT,"5"},
		{SEMICOLON,";"},
		{LET,"let"},
		{IDENT,"ten"},
		{ASSIGN,"="},
		{INT,"10"},
		{SEMICOLON,";"},
		{LET,"let"},
		{IDENT,"add"},
		{ASSIGN,"="},
		{FUNCTION,"fn"},
		{LPAREN,"("},
		{IDENT,"x"},
		{COMMA,","},
		{IDENT,"y"},
		{RPAREN,")"},
		{LBRACE,"{"},
		{IDENT,"x"},
		{PLUS,"+"},
		{IDENT,"y"},
		{SEMICOLON,";"},
		{RBRACE,"}"},
		{SEMICOLON,";"},
		{LET,"let"},
		{IDENT,"result"},
		{ASSIGN,"="},
		{IDENT,"add"},
		{LPAREN,"("},
		{IDENT,"five"},
		{COMMA,","},
		{IDENT,"ten"},
		{RPAREN,")"},
		{SEMICOLON,";"},

		{SEMICOLON,";"},
		{SEMICOLON,";"},
		{SEMICOLON,";"},
		{SEMICOLON,";"},
		{SEMICOLON,";"},
	};

	lexer * test_lexer = lexer_create(input); //clean this up

	for (int i = 0; i < test_length; i++) {
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

char * lexer_read_identifier(lexer * lexer) {
	int initial_position = lexer->position;
	while (is_letter(lexer->current_char)) {
		lexer_read_char(lexer);
	}
	char * identifier = malloc(sizeof(char) * (1 +lexer->position- initial_position));
	strncpy(identifier, lexer->input + initial_position ,lexer->position - initial_position);
	identifier[lexer->position-initial_position] = '\0';
	assert(strlen(identifier));
	return identifier;
}

char * lexer_read_number(lexer * lexer) {
	int initial_position = lexer->position;
	while (is_digit(lexer->current_char)) {
		lexer_read_char(lexer);
	}
	char * identifier = malloc(sizeof(char) * (1 +lexer->position- initial_position));
	strncpy(identifier, lexer->input + initial_position ,lexer->position - initial_position);
	identifier[lexer->position-initial_position] = '\0';
	assert(strlen(identifier));
	return identifier;
}

void lexer_skip_whitespace(lexer * lexer) {
	while (lexer->current_char == ' ' || lexer->current_char == '\t' || lexer->current_char == '\n' || lexer->current_char == '\r') {
		lexer_read_char(lexer);
	}
}

token * lexer_next_token(lexer * lexer) {
	token * new_token = malloc(sizeof(token));
	lexer_skip_whitespace(lexer); //unclear if this should go before or after the next line 
	new_token->token_type = token_string_to_token(lexer->current_char);

	if (new_token->token_type == LITTERAL) { 
		new_token->token_string = lexer_read_identifier(lexer); 
		new_token->token_type = token_string_to_keyword(new_token->token_string); 
		return new_token;
	} else if (new_token->token_type == INT) {
		new_token->token_string = lexer_read_number(lexer); 
		return new_token;
	} else if (new_token -> token_type == ILLEGAL) {
		new_token->token_string = malloc(sizeof(char) * 2);
		new_token->token_string[0] = lexer->current_char;
		new_token->token_string[1] = '\0';
	} else {
		new_token->token_string = strdup(token_token_to_string(new_token->token_type));
	}

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
