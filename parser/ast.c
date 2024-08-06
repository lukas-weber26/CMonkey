#include "../lexer/lexer.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

enum ast_node_types {STATEMENT, EXPRESSION};
enum ast_expression_types {NA};
enum ast_statement_types {LET_STATEMENT, RETURN_STATEMENT};

typedef struct ast_node {
	int ast_node_type;
	token * token_litteral;
	void * node_body;
} ast_node;

typedef struct ast_expression_node {
	int ast_expression_type;
	void * expression_body;
} ast_expression_node;

typedef struct ast_statement_node{
	int ast_statement_type;
	void * statement_body;
} ast_statement_node;

typedef struct ast_idenitfier {
	token * identifier_token;
} ast_identifier;

typedef struct ast_let_statement {
	ast_identifier* name;	//identifier
	ast_node * value; //this makes sense
} ast_let_statement;

typedef struct ast_return_statement {		
	//I don't think this has an identifier
	ast_node * value; //this will be an expression
} ast_return_statement;

typedef struct program {
	ast_node ** nodes;	
	int node_count;
	int max_nodes;
} program;

typedef struct error {
	char * error_string;
	struct error * next_error;
} error;

typedef struct parser {
	lexer * lexer;
	token * current_token;
	token * next_token;
	error * parse_error;	
} parser;

ast_node * parser_create_let_statement(token* initial_token, token * secondary_token);
program * ast_new_program(int expected_length);
void ast_start_parse(program * program);
parser * parser_new_parser(char * input_string);
void parser_free_parser(parser * parser);
void parser_next_token(parser * parser);
program * ast_parse_program (parser * parser);
token * copy_token(token * token_to_copy); //this should probably go to the token file
void parser_peek_error(parser * parser, int token_type);
void parser_add_error(parser* parser, char * error_string);
ast_node * parser_create_return_statement(token* initial_token, token * secondary_token);

void parser_add_error(parser* parser, char * error_string) {
	error * new_error = malloc(sizeof(error));
	new_error -> error_string = strdup(error_string); 

	if (!(parser->parse_error)) {
		new_error->next_error = NULL;
		parser->parse_error = new_error;	
	} else {
		new_error->next_error = parser->parse_error;
		parser->parse_error = new_error;	
	}
}

token * copy_token(token * token_to_copy) {
	token * new_token = malloc(sizeof(token));
	new_token->token_string = strdup(token_to_copy->token_string);
	new_token->token_type = token_to_copy->token_type;
	return new_token;
}

void check_parse_errors(parser * parser) {
	if (parser->parse_error != NULL) {
		printf("Errors detected.\n");
		error * temp = parser->parse_error;
		while (temp) { 
			printf("%s",temp->error_string);
			temp = temp->next_error;
		}
		exit(0);
	}
}

void ast_parser_test_let_statements() {
	char * input = "let x = 5;\n"
	"let y = 10;\n"
	"let foobar = 838383;\n";
	
	parser * test_parser = parser_new_parser(input);
	program * test_program = ast_parse_program(test_parser); 
	check_parse_errors(test_parser);

	#define TEST_LENGTH 3 

	assert(test_program != NULL);
	assert(test_program->node_count == TEST_LENGTH);

	char * test_strings [TEST_LENGTH] = {"x","y","foobar"};

	for (int i = 0; i < TEST_LENGTH; i++) {
		ast_node * current_node = test_program->nodes[i];
		assert(strcmp(current_node->token_litteral->token_string,"let") == 0);
		assert(current_node->token_litteral->token_type == LET);
		assert(current_node->ast_node_type == STATEMENT);
		assert(current_node->node_body != NULL);
		
		ast_statement_node * current_statement = current_node->node_body;

		assert(current_statement->ast_statement_type == LET_STATEMENT);
		assert(current_statement->statement_body != NULL);

		ast_let_statement * current_let_statement = current_statement->statement_body;

		assert(strcmp(current_let_statement->name->identifier_token->token_string,test_strings[i]) == 0 );
		assert(current_let_statement->name->identifier_token->token_type == IDENT);

	}


}

void ast_parser_test_return_statements() {
	char * input = "return 5;\n"
	"return 10;\n"
	"return add(15);\n";
	
	parser * test_parser = parser_new_parser(input);
	program * test_program = ast_parse_program(test_parser); 
	check_parse_errors(test_parser);

	#define TEST_LENGTH 3 

	assert(test_program != NULL);
	assert(test_program->node_count == TEST_LENGTH);

	for (int i = 0; i < TEST_LENGTH; i++) {
		ast_node * current_node = test_program->nodes[i];
		assert(strcmp(current_node->token_litteral->token_string,"return") == 0);
		assert(current_node->token_litteral->token_type == RETURN);
		assert(current_node->ast_node_type == STATEMENT);
		assert(current_node->node_body != NULL);
		
		ast_statement_node * current_statement = current_node->node_body;

		assert(current_statement->ast_statement_type == RETURN_STATEMENT);
		assert(current_statement->statement_body != NULL);

		ast_return_statement * current_return_statement =current_statement->statement_body;

		assert(current_return_statement->value==NULL);

	}

}

program * ast_new_program(int expected_length) {
	program * new_program = malloc(sizeof(program));
	new_program ->node_count = 0;
	new_program ->max_nodes = expected_length;
	new_program->nodes = malloc(sizeof(ast_node)*expected_length);
	return new_program;
}

void parser_next_token(parser * parser) {
	free_token(parser->current_token);
	parser->current_token = parser->next_token;
	parser->next_token = lexer_next_token(parser->lexer); 
}

parser * parser_new_parser(char * input_string) {
	parser * new_parser = malloc(sizeof(parser));
	char * input = strdup(input_string);
	lexer * new_lexer = lexer_create(input);
	new_parser->lexer = new_lexer;
	new_parser->parse_error = NULL;

	new_parser -> current_token = lexer_next_token(new_parser->lexer); 
	new_parser -> next_token = lexer_next_token(new_parser->lexer);

	return new_parser;
}

void parser_free_parser(parser * parser) {
	free_lexer(parser->lexer);
	free(parser->current_token);
	free(parser->next_token);
	free(parser);
}


int parser_peek_token_is(parser* parser, int token_id) {
	if (parser->next_token->token_type == token_id) {
		return 1;
	}
	return 0;
}

int parser_current_token_is(parser* parser, int token_type) {
	if (parser->current_token->token_type == token_type) {
		return 1;
	}
	return 0;
}

void parser_peek_error(parser * parser, int token_type) {
	char error_buffer[200];
	sprintf(error_buffer,"ERROR: expected %s, got %s instead.\n", token_token_to_string(token_type), parser->next_token->token_string);
	parser_add_error(parser, error_buffer);
}

int parser_expect_peek(parser * parser, int token_type) {
	if (parser_peek_token_is(parser, token_type)) {
		parser_next_token(parser);
		return 1;
	} else {
		parser_peek_error(parser, token_type);
		return 0;
	}
}

ast_node * parser_create_let_statement(token* initial_token, token * secondary_token) {
	ast_node * new_node = malloc(sizeof(ast_node));
	ast_statement_node * new_statement  = malloc(sizeof(ast_statement_node));
	ast_let_statement* new_let_statement = malloc(sizeof(ast_let_statement));
	ast_identifier * new_identifier = malloc(sizeof(ast_identifier));

	new_node->ast_node_type = STATEMENT;
	new_node->token_litteral= initial_token; 
	new_node->node_body = new_statement;
	new_statement->ast_statement_type = LET_STATEMENT;
	new_statement->statement_body = new_let_statement; 
	new_let_statement->name = new_identifier;
	new_identifier->identifier_token = secondary_token; 
	new_let_statement->value = NULL;
	
	return new_node;
}

ast_node * parser_create_return_statement(token* initial_token, token * secondary_token) {
	ast_node * new_node = malloc(sizeof(ast_node));
	ast_statement_node * new_statement  = malloc(sizeof(ast_statement_node));
	ast_return_statement * new_return_statement = malloc(sizeof(ast_return_statement));

	new_node->ast_node_type = STATEMENT;
	new_node->token_litteral= initial_token; 
	new_node->node_body = new_statement;
	new_statement->ast_statement_type = RETURN_STATEMENT;
	new_statement->statement_body = new_return_statement; 
	new_return_statement->value = NULL;
	
	return new_node;
}

ast_node * parser_parse_let_statement(parser * parser) {

	token * initial_token = copy_token(parser->current_token);

	if (!parser_expect_peek(parser, IDENT)) {
		return NULL;
	}

	token * second_token = copy_token(parser->current_token);

	if (!parser_expect_peek(parser, ASSIGN)) {
		return NULL;
	}
	
	while (!parser_current_token_is(parser, SEMICOLON)) {
		parser_next_token(parser);	
	}

	ast_node * new_node = parser_create_let_statement(initial_token, second_token);
	return new_node;	
}

ast_node * parser_parse_return_statement(parser * parser) {

	token * initial_token = copy_token(parser->current_token);
	parser_next_token(parser);

	while (!parser_current_token_is(parser, SEMICOLON)) {
		parser_next_token(parser);	
	}

	ast_node * new_node = parser_create_return_statement(initial_token, NULL);
	return new_node;	
}

ast_node * parser_parse_statement(parser * parser) {
	switch (parser->current_token->token_type) {
		case LET:		
			return parser_parse_let_statement(parser);
		case RETURN:
			return parser_parse_return_statement(parser);
		default: 
			return NULL;
	}
}

program * ast_parse_program (parser * parser) {
	program * new_program = ast_new_program(4);

	//This condition is suss but the tests suggest that it works.
	while (parser->current_token->token_type != ENDFILE) {

		//get a statement from the current token. If valid, append it to the program.
		ast_node * new_node = parser_parse_statement(parser);
		//I could swear that this thing is currect
		new_program->nodes[new_program->node_count] = new_node;
	
		parser_next_token(parser);
		check_parse_errors(parser);
		new_program->node_count++;	
	

		//a little bit sketchy maybe..
		if (new_program->max_nodes == new_program->node_count) {
			new_program->max_nodes *= 2;
			new_program->nodes = realloc(new_program->nodes, new_program->max_nodes);
		}

	}

	//get rid of excess memory  
	new_program->nodes = realloc(new_program->nodes, new_program->max_nodes);
	return new_program;
}
	
int main() {
	ast_parser_test_let_statements();
	ast_parser_test_return_statements();
	printf("Tests passed!\n");
}

