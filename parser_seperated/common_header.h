#include "../lexer/lexer.c"

enum ast_node_types {STATEMENT, EXPRESSION};
enum ast_expression_types {NA};
enum ast_statement_types {LET_STATEMENT, RETURN_STATEMENT, EXPRESSION_STATEMENT};

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

typedef struct ast_expression_statement {		
	//I don't think this has an identifier
	ast_node * value; //this will be an expression
} ast_expression_statement;

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

//core functions

//parser
ast_node * parser_create_let_statement(token* initial_token, token * secondary_token);
ast_node * parser_create_return_statement(token* initial_token, token * secondary_token);
ast_node * parser_create_expression_statement(token* initial_token, token * secondary_token);
ast_node * parser_parse_expression_statement(parser * parser);

void parser_free_parser(parser * parser);
void parser_next_token(parser * parser);
void parser_peek_error(parser * parser, int token_type);
void parser_add_error(parser* parser, char * error_string);
void check_parse_errors(parser * parser);

parser * parser_new_parser(char * input_string);

//ast node
program * ast_new_program(int expected_length);
program * ast_parse_program (parser * parser);

void ast_start_parse(program * program);
void ast_node_print(ast_node * ast_node);
void ast_program_print(program * program);

//token
token * copy_token(token * token_to_copy); //this should probably go to the token file

//test functions 
void ast_parser_test_return_statements();
void ast_parser_test_let_statements();
