#include "common_header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void ast_program_print(program * program) {
	for (int i = 0; i < program->node_count; i ++) {
		ast_node_print(program->nodes[i]);
	}
}

void ast_node_print(ast_node * ast_node) {
	if (ast_node->ast_node_type == EXPRESSION) {

	} else {
		//these prints will need to add support for the actual expressions!
		ast_statement_node * statement_node = ast_node->node_body;
		switch (statement_node->ast_statement_type) {
			case LET_STATEMENT:	
				printf("%s: %s\n", ast_node->token_litteral->token_string,((ast_let_statement *)(statement_node->statement_body)) -> name->identifier_token->token_string);
				break;
			case RETURN_STATEMENT:	
				printf("%s. \n", ast_node->token_litteral->token_string); //in the end these should probably also take care of their expressions... oh well
				break;
			case EXPRESSION_STATEMENT:
				printf("%s. \n", ast_node->token_litteral->token_string); //in the end these should probably also take care of their expressions... oh well
				break;
			default:
				break;
		}
	}

	return;
} 

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

ast_node * parser_create_expression_statement(token* initial_token, token * secondary_token) {
	ast_node * new_node = malloc(sizeof(ast_node));
	ast_statement_node * new_statement  = malloc(sizeof(ast_statement_node));
	ast_expression_statement* new_expression_statement = malloc(sizeof(ast_expression_statement));

	new_node->ast_node_type = STATEMENT;
	new_node->token_litteral= initial_token; 
	new_node->node_body = new_statement;
	new_statement->ast_statement_type = EXPRESSION_STATEMENT;
	new_statement->statement_body = new_statement; 
	new_expression_statement->value = NULL;
	
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

ast_node * parser_parse_expression_statement(parser * parser) {

	token * initial_token = copy_token(parser->current_token);
	parser_next_token(parser);

	while (!parser_current_token_is(parser, SEMICOLON)) {
		parser_next_token(parser);	
	}

	ast_node * new_node = parser_create_expression_statement(initial_token, NULL);
	return new_node;	
}

ast_node * parser_parse_statement(parser * parser) {
	switch (parser->current_token->token_type) {
		case LET:		
			return parser_parse_let_statement(parser);
		case RETURN:
			return parser_parse_return_statement(parser);
		default: 
			return parser_parse_expression_statement(parser);
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

