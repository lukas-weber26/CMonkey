#include "common_header.h"

//perfomrance improvement idea: Store the lexer input length. strlen is going to be expensive for large files... Done.
//notice that the origional implementation has one branch less because it does not have an additional switch at the token level
//The obvious fix to this problem is to just bring the switch statement from the tokenizer over to this file..
//Program is valgrind aprooved

//DANGER: THERE IS A CORE DIFFERENCE BETWEEN THIS AND THE BOOK:
//In short, c strings do not contain EOF characters, hence no EOF character is detected.
//EOF functionality can therefore not be tested effectively. Use an exernal file and maybe architect this 
//so that the EOF token is not needed.

//start of testing 
#ifdef TEST
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
	"5 < 10 > 5; \n"
	"true false return if else;\n"
	"!= ==;\n";

	#define test_length 57
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
		{BANG,"!"},
		{MINUS,"-"},
		{SLASH,"/"},
		{ASTERISK,"*"},
		{INT,"5"},
		{SEMICOLON,";"},
		{INT,"5"},
		{LT,"<"},
		{INT,"10"},
		{GT,">"},
		{INT,"5"},
		{SEMICOLON,";"},
		{TRUE,"true"},
		{FALSE,"false"},
		{RETURN,"return"},
		{IF,"if"},
		{ELSE,"else"},
		{SEMICOLON,";"},
		{NOT_EQ,"!="},
		{EQ,"=="},
		{SEMICOLON,";"},
	};

	lexer * test_lexer = lexer_create(input); //clean this up

	for (int i = 0; i < test_length+1; i++) {
		token * test_token = lexer_next_token(test_lexer); 

		if (i != test_length) {
			assert(tests[i].token_type == test_token->token_type);
			assert(strcmp(tests[i].expected_literal, test_token->token_string) == 0);
		} else {
			printf("Token: %s\n", token_token_to_string(test_token->token_type));
		}

		free_token(test_token);
	}

	free_lexer(test_lexer);
	printf("Testing complete.\n");
}

int main() {
	lexer_test_next_token();	
}
#endif
//end of testing 


void lexer_read_char(lexer * lexer) {
	if (lexer->read_position >= lexer->input_length){
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
	new_lexer->input_length = strlen(new_lexer->input);
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
	} else if (new_token-> token_type == ASSIGN) {
		char next = lexer_peek_char(lexer);
		if (next == '=') {
			new_token->token_type = EQ;
			lexer_read_char(lexer);
		}
		new_token->token_string = strdup(token_token_to_string(new_token->token_type));
	} else if (new_token->token_type == BANG) {
		char next = lexer_peek_char(lexer);
		if (next == '=') {
			new_token->token_type = NOT_EQ;
			lexer_read_char(lexer); 
		}
		new_token->token_string = strdup(token_token_to_string(new_token->token_type));
	} else {
		new_token->token_string = strdup(token_token_to_string(new_token->token_type));
	}

	lexer_read_char(lexer); 
	return new_token;
}

char lexer_peek_char(lexer * lexer) {
	if (lexer->position >= lexer->input_length) {
		return 0;	
	} else {
		return lexer->input[lexer->read_position];
	}
}


void free_token(token * token) {
	free(token->token_string);
	free(token);
}

void free_lexer(lexer * lexer) {
	free(lexer->input);
	free(lexer);
}
