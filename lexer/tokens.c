#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char * token_token_to_string(int token);
int token_string_to_token(char current_char);

enum token_types {ILLEGAL, ENDFILE, IDENT, INT, ASSIGN, PLUS, COMMA, SEMICOLON, LPAREN, RPAREN, LBRACE, RBRACE, FUNCTION, LET};

typedef struct token {
	int token_type; 
	char * token_string;
} token;

int token_string_to_token(char current_char) {
	switch (current_char) {
		case '=':	
			return ASSIGN;
			break;
		case ';':	
			return SEMICOLON;
			break;
		case '(':	
			return LPAREN;
			break;
		case ')':	
			return RPAREN;
			break;
		case ',':	
			return COMMA;
			break;
		case '+':	
			return PLUS;
			break;
		case '{':	
			return LBRACE;
			break;
		case '}':	
			return RBRACE;
			break;
		case '\0':	
			return ENDFILE;
			break;
		default:
			printf("Invalid character in tokenizer.");
			exit(0);
	}
}

char * token_token_to_string(int token) {
	switch (token) {
		case ILLEGAL:
			return "ILLEGAL";
			break;
		case ENDFILE:
			return "EOF";
			break;
		case IDENT:
			return "IDENT";
			break;
		case INT:
			return "INT";
			break;
		case ASSIGN:
			return "=";
			break;
		case PLUS:
			return "+";
			break;
		case COMMA:
			return ",";
			break;
		case SEMICOLON:
			return ";";
			break;
		case LPAREN:
			return "(";
			break;
		case RPAREN:
			return ")";
			break;
		case LBRACE:
			return "{";
			break;
		case RBRACE:
			return "}";
			break;
		case FUNCTION:
			return "FUNCTION";
			break;
		case LET:
			return "LET";
			break;
		default:
			printf("INVALID TOKEN.\n");
			exit(0);
	}
}

