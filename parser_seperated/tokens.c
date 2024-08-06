#include "common_header.h"

int is_letter(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

int is_digit(char c) {
	return (c >= '0' && c <= '9');
}

int token_string_to_keyword(char * keyword) {
	if (strcmp(keyword, "fn") == 0) {
		return FUNCTION;
	}	
	
	if (strcmp(keyword, "let") == 0) {
		return LET;
	}	
	
	if (strcmp(keyword, "true") == 0) {
		return TRUE;
	}	
	
	if (strcmp(keyword, "false") == 0) {
		return FALSE;
	}	
	
	if (strcmp(keyword, "if") == 0) {
		return IF;
	}	
	
	if (strcmp(keyword, "else") == 0) {
		return ELSE;
	}	
	
	if (strcmp(keyword, "return") == 0) {
		return RETURN;
	}	

	return IDENT;	
}

char * token_keyword_to_string(int keyword) {
	switch (keyword) {
		case LET:
			return "LET";
			break;
		case FUNCTION:
			return "fn";
			break;
		default:
			printf("Illegal keyword.\n");
			exit(0);
			break;
	}
}

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
		case '<':	
			return LT;
			break;
		case '>':	
			return GT;
			break;
		case '!':	
			return BANG;
			break;
		case '*':	
			return ASTERISK;
			break;
		case '-':	
			return MINUS;
			break;
		case '/':	
			return SLASH;
			break;
		case '\0':	
			return ENDFILE;
			break;
		default:
			if (is_letter(current_char)) {
				return LITTERAL;
			} else if (is_digit(current_char)) {
				return INT;
			} else {
				return ILLEGAL;
			}
			break;
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
		case LITTERAL:
			return "LITTERAL";
			break;
		case LT:	
			return "<";
			break;
		case GT:	
			return ">";
			break;
		case BANG:
			return "!";
			break;
		case ASTERISK:
			return "*";
			break;
		case MINUS:
			return "-";
			break;
		case SLASH:
			return "/";
			break;
		case EQ:
			return "==";
			break;
		case NOT_EQ:
			return "!=";
			break;
		default:
			printf("INVALID TOKEN.\n");
			exit(0);
	}
}


