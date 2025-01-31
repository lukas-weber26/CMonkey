#include "common_header.h"

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

	printf("Test program print:\n");
	ast_program_print(test_program);

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
