/* c_lex.h - header file for c_lex.c */

/*  Copyright 1991 Mark Russell, University of Kent at Canterbury.
 *
 *  You can do what you like with this source code as long as
 *  you don't try to make money out of it and you include an
 *  unaltered copy of this message (including the copyright).
 */

/*
 *  This is the header file for a standalone C lexer (c_lex.c).
 *  Written : Jan 19 1998
 *  Author : Dibyendu Majumdar
 *  Email  : dibyendu@mazumdar.demon.co.uk  
 *  Website: www.mazumdar.demon.co.uk
 */

#ifndef c_lex_h
#define c_lex_h

typedef enum { FALSE, TRUE } bool;

typedef enum token_t token_t;
enum token_t {
	EOI,
	FOR,
	BREAK,
	CONTINUE,
	RETURN	,
	IF,
	ELSE,
	WHILE,
	DO,
	SWITCH,
	CASE,
	DEFAULT,
	GOTO,
	SIZEOF,
	AUTO,
	REGISTER,
	STATIC,
	EXTERN,
	TYPEDEF,
	INLINE,
	BOOL,
	COMPLEX,
	IMAGINARY,
	VOID,
	CHAR,
	SHORT,
	INT,
	LONG,
	FLOAT,
	DOUBLE,
	SIGNED,
	UNSIGNED,
	CONST,
	VOLATILE,
	RESTRICT,
	STRUCT,
	UNION,
	ENUM,
	AND,
	TILDE,
	NOT,
	LESSTHAN,
	GREATERTHAN,
	XOR,
	OR,
	PLUS,
	MINUS,
	SLASH,
	PERCENT,
	STAR,
	DOT,
	COLON,
	QUERY,
	SEMI,
	COMMA,
	LPAREN,
	RPAREN,
	LBRACE,
	RBRACE,
	LBRAC,
	RBRAC,
	EQUALS,
	STAR_EQUALS,
	SLASH_EQUALS,
	PERCENT_EQUALS,
	PLUS_EQUALS,
	MINUS_EQUALS,
	LSHIFT_EQUALS,
	RSHIFT_EQUALS,
	AND_EQUALS,
	XOR_EQUALS,
	OR_EQUALS,
	ANDAND,
	OROR,
	EQEQ,
	NOTEQ,
	GTEQ,
	LESSEQ,
	LSHIFT,
	RSHIFT,
	PLUSPLUS,
	MINUSMINUS,
	ARROW,
	ELLIPSIS,
	INTEGER_CONSTANT,
	CHARACTER_CONSTANT,
	FLOATING_CONSTANT,
	STRING_CONSTANT,
	IDENTIFIER,
	TYPEDEF_NAME,
	BADTOK
};

enum {
	MAX_IDENTIFIER_LEN = 1024
};

typedef struct {
	char id_name[MAX_IDENTIFIER_LEN];
} identifier_t;

typedef struct {
	char *co_val;
	size_t co_size;
} constant_t;

typedef union {
	identifier_t *identifier;
	constant_t *constant;
} lexeme_t;

/*  Environment for lexer.
 */
typedef struct lex_envst {
	const char *le_lptr;
	const char *le_filename;
	int le_lnum;
	bool le_had_error;
	const char *le_line;
	const char *(*le_getline)(char *arg);
	char *le_getline_arg;
	bool le_abort_parse;
	lexeme_t le_lexeme;
} lex_env_t;

extern lexeme_t *Lexeme;
extern lex_env_t *Lex_env;
#define BUF_SIZE 1024                // added
extern char token_str[];             // added

void *safe_calloc(size_t,size_t);
#define NEW(type)	((type *)safe_calloc(1, sizeof(type)))
#define NEW_ARRAY(type, size) ((type *)safe_calloc((size), sizeof(type)))

token_t lex_get_token (void);
token_t lex_prev_token (void);
bool lex_colon_follows (void);
void lex_error (const char *s);
const char *lex_tokname(token_t);
extern token_t name_type(const char *buf);
extern void *safe_calloc(size_t,size_t);
extern char *string_copy(const char *s, int len);

const char *tokname(token_t);


#endif
