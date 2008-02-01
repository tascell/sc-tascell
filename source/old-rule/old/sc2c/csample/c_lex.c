/* c_lex.c - a standalone C lexical analyser */

/*  Copyright 1991 Mark Russell, University of Kent at Canterbury.
 *
 *  You can do what you like with this source code as long as
 *  you don't try to make money out of it and you include an
 *  unaltered copy of this message (including the copyright).
 */

/*
 *  This standalone version created on Jan 19 1998.
 *  Author : Dibyendu Majumdar
 *  Email  : dibyendu@mazumdar.demon.co.uk  
 *  Website: www.mazumdar.demon.co.uk
 */

/*
 * 21 Jan 2001 Added support for inline, restrict, _Bool, _Complex, and _Imaginary.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "c_lex.h"
static bool Want_debugging_output;

/* static const char *tokname (token_t token); */
static const char *parse_hash_directive (const char *line, lex_env_t *le);
static const char *skip_whitespace (lex_env_t *le, const char *line);
static int get_float_constant (lex_env_t *le, const char *line,
					const char **p_end, constant_t *co);
static const char *getline (lex_env_t *le);
static int get_string (lex_env_t *le, const char *line, constant_t *co);

static struct {
	const char *name;
	token_t token;
	bool need_lexinfo;
} Keytab[] = {
	{"_Bool",	BOOL,		FALSE},
	{"_Complex",	COMPLEX,	FALSE},
	{"_Imaginary",	IMAGINARY,	FALSE},
	{"auto",	AUTO,		FALSE},
	{"break",	BREAK,		TRUE},
	{"case",	CASE,		FALSE},
	{"char",	CHAR,		FALSE},
	{"const",	CONST,		FALSE},
	{"continue",	CONTINUE,	TRUE},
	{"default",	DEFAULT,	FALSE},
	{"do",		DO,		FALSE},
	{"double",	DOUBLE,		FALSE},
	{"else",	ELSE,		FALSE},
	{"enum",	ENUM,		FALSE},
	{"extern",	EXTERN,		FALSE},
	{"float",	FLOAT,		FALSE},
	{"for",		FOR,		TRUE},
	{"goto",	GOTO,		FALSE},
	{"if",		IF,		FALSE},
	{"inline",	INLINE,		FALSE},
	{"int",		INT,		FALSE},
	{"long",	LONG,		FALSE},
	{"register",	REGISTER,	FALSE},
	{"restrict",	RESTRICT,	FALSE},
	{"return",	RETURN,		TRUE},
	{"short",	SHORT,		FALSE},
	{"signed",	SIGNED,		FALSE},
	{"sizeof",	SIZEOF,		FALSE},
	{"static",	STATIC,		FALSE},
	{"struct",	STRUCT,		FALSE},
	{"switch",	SWITCH,		FALSE},
	{"typedef",	TYPEDEF,	FALSE},
	{"union",	UNION,		FALSE},
	{"unsigned",	UNSIGNED,	FALSE},
	{"void",	VOID,		FALSE},
	{"volatile",	VOLATILE,	FALSE},
	{"while",	WHILE,		FALSE}
};
#define NKEYS (sizeof Keytab / sizeof *Keytab)

lex_env_t *Lex_env;
lexeme_t *Lexeme;

constant_t Constant;
identifier_t Identifier;

char *string_copy(const char *string, int len);

void
lex_error(s)
const char *s;
{
  fprintf(stderr, "Error: %s", s);
}

static const char *
parse_hash_directive(line, le)
const char *line;
lex_env_t *le;
{
	int lnum, nitems;
	char name[256];

	for (; isspace(*line) && *line != '\0'; ++line)
		;
	if (*line == '\0')
		return line;

	if (strncmp(line, "pragma", 6) == 0 && isspace(line[6])) {
		for (line += 7; *line != '\0' && isspace(*line); ++line)
			;
		fprintf(stderr, "#pragma `%.*s' ignored",
							strlen(line) - 1, line);
		return line + strlen(line);
	}
	if (strncmp(line, "line", 4) == 0) {
		line += 4;
	}

	nitems = sscanf(line, "%d \"%[^\"]\"", &lnum, name);
	if (nitems < 1) {
		fprintf(stderr, "Bad # directive \"%s\"", line);
		return "";
	}
	if (nitems == 2) {
		char *buf;
		int len;

		len = strlen(name);
		buf = NEW_ARRAY(char, len + 1);
		(void) memcpy(buf, name, len + 1);
		le->le_filename = buf;
	}

	/*  Subtract 1 because we number internally from 0,
	 *  and 1 because we are just about to bump the
	 *  line number.
	 */
	le->le_lnum = lnum - 2;

	return line + strlen(line);
}



const char *
ci_translate_escape(s, p_res)
const char *s;
int *p_res;
{
	static const char hexdigits[] = "0123456789abcdefABCDEF";
	const char *pos, *save_s;
	int ch;

	switch (*s) {
	case 'n':
		ch = '\n';
		break;
	case 't':
		ch = '\t';
		break;
	case 'v':
		ch = '\v';
		break;
	case 'b':
		ch = '\b';
		break;
	case 'r':
		ch = '\r';
		break;
	case 'f':
		ch = '\f';
		break;
	case 'a':
		ch = '\007';
		break;
	case '0': case '1': case '2': case '3':
	case '4': case '5': case '6': case '7':
		ch = 0;
		for (save_s = s; isdigit(*s) && *s < '8' && s - save_s < 3; ++s)
			ch = ch * 8 + *s - '0';
		--s;
		break;
	case 'x':
		ch = 0;
		for (; *s != '\0' && (pos = strchr(hexdigits, *s)) != NULL; ++s) {
			if (pos >= hexdigits + 16)
				pos -= 6;
			ch = ch * 16 + pos - hexdigits;
		}
		break;
	default:
		ch = *s;
		break;
	}
	/* Dibyendu : 11/1/99
	 * Fixed problem of sign extension - '\377' is now -1 and not 255
	 */
	*p_res = (int)(char)ch;
	/* *p_res = ch; */

	return s;
}

/*  Based on K&P's hoc follow() function.
 */
#define follow(s, ch, ifyes, ifno) ((*(s) == (ch)) ? (++(s), (ifyes)) : (ifno))

static const char *
getline(le)
lex_env_t *le;
{
	if (le->le_abort_parse)
		return NULL;

	++le->le_lnum;
	return le->le_line = (*le->le_getline)(le->le_getline_arg);
}

/*  Skip white space and comments.
 */
static const char *
skip_whitespace(le, line)
lex_env_t *le;
const char *line;
{
	bool read_another_line;
	bool incomment;

	incomment = FALSE;
	read_another_line = FALSE;

	if (line == NULL) {
		if ((line = getline(le)) == NULL)
			return line;
	}

	for (;;) {
		for(;;) {
			while (*line != '\0' && isspace(*line))
				++line;
			if (*line != '\0')
				break;

			if ((line = getline(le)) == NULL)
				break;
			read_another_line = TRUE;
			if (*line == '#')
				line = parse_hash_directive(line + 1, le);
		}
		if (incomment) {
			if (line == NULL) {
				fprintf(stderr, 
						"Hit EOF while in a comment");
				break;
			}
			else if (*line == '*' && line[1] == '/') {
				line += 2;
				incomment = FALSE;
			}
			else
				++line;
		}
		else {
			if (line != NULL && *line == '/' && line[1] == '*') {
				line += 2;
				incomment = TRUE;
			}
			else
				break;
		}
	}

	if (Want_debugging_output && read_another_line) {
#if 0
		putchar('\n');
		printf("\n\"%s\", %d: %s", le->le_filename, le->le_lnum, line);
#endif
		printf("\"%s\", %d: %s\n", le->le_filename, le->le_lnum, line);
	}
	return line;
}

static bool
is_aggr_type_specifier(token)
token_t token;
{
	if (token == STRUCT 
	||  token == UNION
	||  token == ENUM)
		return TRUE;
	return FALSE;
}
    
static bool
is_basic_type_specifier(token)
token_t token;
{
	if (token == INT
	||  token == UNSIGNED
	||  token == SIGNED
	||  token == LONG
	||  token == SHORT
	||  token == VOID
	||  token == CHAR
	||  token == FLOAT
	||  token == DOUBLE) 
		return TRUE;
	return FALSE;
}

static bool
is_storage_class_or_qualifier(token)
token_t token;
{
	if (token == STATIC
	||  token == EXTERN
	||  token == TYPEDEF
	||  token == AUTO
	||  token == REGISTER
	||  token == CONST
	||  token == VOLATILE)
		return TRUE;
	return FALSE;
}

static bool
is_decl_specifier(token)
token_t token;
{
	return is_storage_class_or_qualifier(token)
	||     is_basic_type_specifier(token)
	||     is_aggr_type_specifier(token)
	||     token == TYPEDEF_NAME;
}

static token_t Prev_token = 0;	/* remember last token */
static bool Colon_follows = FALSE;

token_t
lex_prev_token(void)
{
	return Prev_token;
}

bool
lex_colon_follows(void)
{
	return Colon_follows;
}

token_t
lex_get_token()
{
	static int pos = -1;
	lex_env_t *le;
	token_t token;
	const char *line;
	const char *line_head; // added

	le = Lex_env;

	if (pos == -1) {
		Want_debugging_output = getenv("LEX_DEBUG") != NULL;
		pos = 0;
	}
	if (le == NULL) {
		if (Want_debugging_output)
			puts("\n");
		return 0;
	}

	if ((line = skip_whitespace(le, le->le_lptr)) == NULL) {
		le->le_lptr = line;
		return 0;	/* EOF */
	}
	line_head = line;

	switch (*line++) {
	case '_': case '$':

	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
	case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
	case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
	case 'v': case 'w': case 'x': case 'y': case 'z': 

	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
	case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
	case 'V': case 'W': case 'X': case 'Y': case 'Z':
		{
			const char *s;
			int len, i;

			--line;
			for (s = line; isalnum(*s) || *s == '_' || *s == '$'; ++s)
				;
			len = s - line;

			for (i = 0; i < NKEYS; ++i)
				if (memcmp(Keytab[i].name, line, len) == 0 &&
							Keytab[i].name[len] == '\0')
					break;
			if (i < NKEYS) {
				token = Keytab[i].token;
				line += len;
				break;
			}
					
			if (len+1 > sizeof Identifier.id_name)
				len = sizeof Identifier.id_name-1;
			
			strncpy(Identifier.id_name, line, len+1);
			Identifier.id_name[len] = '\0';
			Lexeme->identifier = &Identifier;

			line = skip_whitespace(le, s);

			/* The parser provides the function name_type() which is 
			 * called here to determine whether a name is a potential 
			 * TYPEDEF name. 
			 */
			token = name_type(Identifier.id_name);
			Colon_follows = *line == ':';
		}
		break;
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		{
			char *end;
			long val;
			
			val = strtol(line - 1, &end, 0);
			if (end == line - 1) {
				le->le_lptr = line;
				fprintf(stderr,
					"Badly formed integer constant \"%s\"",
					line - 1);
				token = BADTOK;
			}
			else if (*end == 'e' || *end == 'E' || *end == '.') {
				token = get_float_constant(le, line-1, &line,
								&Constant);
				Lexeme->constant = &Constant;
			}
			else {
				while (*end == 'L' || *end == 'l' || *end == 'u' || *end == 'U')
					++end;
				Constant.co_val = string_copy(line-1, end-(line-1));
				Constant.co_size = end-(line-1);
				line = end;

				Lexeme->constant = &Constant;
				token = INTEGER_CONSTANT;
			}
		}
		break;
	case '!':
		token = follow(line, '=', NOTEQ, NOT);
		break;
	case '=':
		token = follow(line, '=', EQEQ, EQUALS);
		break;
	case '%':
		token = follow(line, '=', PERCENT_EQUALS, PERCENT);
		break;
	case '/':
		token = follow(line, '=', SLASH_EQUALS, SLASH);
		break;
	case '^':
		token = follow(line, '=', XOR_EQUALS, XOR);
		break;
	case '*':
		token = follow(line, '=', STAR_EQUALS, STAR);
		break;
	case '[':
		token = LBRAC;
		break;
	case ']':
		token = RBRAC;
		break;
	case '{':
		token = LBRACE;
		break;
	case '}':
		token = RBRACE;
		break;
	case '(':
		token = LPAREN;
		break;
	case ')':
		token = RPAREN;
		break;
	case ',':
		token = COMMA;
		break;
	case ';':
		token = SEMI;
		break;
	case '?':
		token = QUERY;
		break;
	case ':':
		token = COLON;
		break;
	case '\'': {
		/*  BUG: no escapes etc.
		 */
		int val;
		const char *startp = line-1;
		const char *endp = 0;

		if (*line == '\\')
			line = ci_translate_escape(line + 1, &val);
		else
			val = *line;
		++line;

		if (*line != '\'') {
			le->le_lptr = line;
			fprintf(stderr, "Unterminated char constant");
			token = BADTOK;
		}
		else {
			endp = ++line;
			Constant.co_val = string_copy(startp, endp-startp);
			Constant.co_size = endp-startp;
			Lexeme->constant = &Constant;
			token = CHARACTER_CONSTANT;
		}
		break;
	}
	case '"': {
		token = get_string(le, line, &Constant);
		Lexeme->constant = &Constant;
		line = le->le_lptr;
		break;
	}
	case '.':
		if (*line == '.' && line[1] == '.') {
			line += 2;
			token = ELLIPSIS;
		}
		else if (isdigit(*line)) {
			token = get_float_constant(le, line-1, &line, &Constant);
			Lexeme->constant = &Constant;
		}
		else
			token = DOT;
		break;
	case '~':
		token = TILDE;
		break;
	case '+':
		if (*line == '+')
			token = PLUSPLUS;
		else if (*line == '=')
			token = PLUS_EQUALS;
		else {
			token = PLUS;
			--line;
		}
		++line;
		break;
	case '-':
		if (*line == '>')
			token = ARROW;
		else if (*line == '-')
			token = MINUSMINUS;
		else if (*line == '=')
			token = MINUS_EQUALS;
		else {
			token = MINUS;
			--line;
		}
		++line;
		break;
	case '|':
		if (*line == '|')
			token = OROR;
		else if (*line == '=')
			token = OR_EQUALS;
		else {
			--line;
			token = OR;
		}
		++line;
		break;
	case '&':
		if (*line == '&')
			token = ANDAND;
		else if (*line == '=')
			token = AND_EQUALS;
		else {
			--line;
			token = AND;
		}
		++line;
		break;
	case '>':
		if (*line == '>') {
			++line;
			token = follow(line, '=', RSHIFT_EQUALS, RSHIFT);
		}
		else if (*line == '=') {
			++line;
			token = GTEQ;
		}
		else
			token = GREATERTHAN;
		break;
	case '<':
		if (*line == '<') {
			++line;
			token = follow(line, '=', LSHIFT_EQUALS, LSHIFT);
		}
		else if (*line == '=') {
			++line;
			token = LESSEQ;
		}
		else
			token = LESSTHAN;
		break;
			
	default:
		le->le_lptr = line; /* because we are about to call diagf */
		fprintf(stderr,
			"Illegal character '%c' (0x%02x)", line[-1], line[-1]);
		token = BADTOK;
		break;
	}
	le->le_lptr = line;

#if 0
	if (Want_debugging_output) {
		const char *name;

		if (pos > 70) {
			putchar('\n');
			pos = 0;
		}
		name = tokname(token);
		printf("%s ", name);
		pos += strlen(name) + 1;
		fflush(stdout);
	}
#endif

	Prev_token = token;
	/* <-- added */
	{
	  const char *src = line_head;
	  char *dst = token_str;
	  for( ; isspace(*src) ; src++ );
	  for( ; src<line ; src++, dst++ )
	    *dst = *src;
	  *dst = '\0';
	}
	/* added --> */
	return token;
}

static int
get_string(le, line, co)
lex_env_t *le;
const char *line;
constant_t *co;
{
	static const char badalloc[] =
				"Unable to allocate memory for string constant";
	static char *buf;
	static int bufsize = 0;
	int opos;
	bool ok;

	if (bufsize == 0) {
		bufsize = 50;
		if ((buf = malloc(bufsize + 1)) == NULL) {
			fprintf(stderr, "%s", badalloc);
			return BADTOK;
		}
	}

	opos = 0;
	ok = FALSE;		/* set to TRUE on success */

	for (; *line != '\0'; ++line) {
		int ch;

		if (*line == '"') {
			const char *new_line;

			new_line = skip_whitespace(le, line + 1);
			if (new_line == NULL || *new_line != '"') {
				ok = TRUE;
				le->le_lptr = new_line;
				break;
			}

			line = new_line;
			continue;
		}

		if (*line != '\\')
			ch = *line;
		else if (*++line == '\n') {
			line = getline(le);
			ch = (line != NULL) ? *line : '\0';
		}
		else
			line = ci_translate_escape(line, &ch);

		if (line == NULL || *line == '\n' || *line == '\0') {
			le->le_lptr = line;
			fprintf(stderr,
						"Unterminated string constant");
			break;
		}

		if (opos == bufsize) {
			bufsize *= 2;
			if ((buf = realloc(buf, bufsize + 1)) == NULL) {
				le->le_lptr = line;
				fprintf(stderr,
							"%s", badalloc);
				break;
			}
		}
		buf[opos++] = ch;
	}
	buf[opos++] = '\0';

	if (!ok)
		return BADTOK;

	co->co_val = buf;
	co->co_size = opos;
	return STRING_CONSTANT;
}

static int
get_float_constant(le, line, p_end, co)
lex_env_t *le;
const char *line, **p_end;
constant_t *co;
{
	double val;
	char *end;

	val = strtod(line, &end);

	if (end == line) {
		le->le_lptr = line;
		fprintf(stderr, "Badly formed floating constant \"%s\"", line);
		return BADTOK;
	}

	co->co_val = string_copy(line, end-line);
	co->co_size = end-line;

	*p_end = end;

	return FLOATING_CONSTANT;
}

/* static */
const char *
tokname(token_t token)
{
	static struct {
		const char *name;
		token_t token;
	} tab[] = {
		"IF",                     IF,
		"ELSE",                   ELSE,
		"WHILE",                  WHILE,
		"FOR",                    FOR,
		"DO",                     DO,
		"GOTO",                   GOTO,
		"BREAK",                  BREAK,
		"CONTINUE",               CONTINUE,
		"RETURN",                 RETURN,
		"SWITCH",                 SWITCH,
		"CASE",                   CASE,
		"DEFAULT",                DEFAULT,
		"SIZEOF",                 SIZEOF,
		"AUTO",                   AUTO,
		"REGISTER",               REGISTER,
		"STATIC",                 STATIC,
		"EXTERN",                 EXTERN,
		"TYPEDEF",                TYPEDEF,
		"INLINE",                 INLINE,
		"BOOL",                   BOOL,
		"COMPLEX",                COMPLEX,
		"IMAGINARY",              IMAGINARY,
		"VOID",                   VOID,
		"CHAR",                   CHAR,
		"SHORT",                  SHORT,
		"INT",                    INT,
		"LONG",                   LONG,
		"FLOAT",                  FLOAT,
		"DOUBLE",                 DOUBLE,
		"SIGNED",                 SIGNED,
		"UNSIGNED",               UNSIGNED,
		"CONST",                  CONST,
		"VOLATILE",               VOLATILE,
		"RESTRICT",               RESTRICT,
		"STRUCT",                 STRUCT,
		"UNION",                  UNION,
		"ENUM",                   ENUM,
		"AND",                    AND,
		"TILDE",                  TILDE,
		"NOT",                    NOT,
		"LESSTHAN",               LESSTHAN,
		"GREATERTHAN",            GREATERTHAN,
		"XOR",                    XOR,
		"OR",                     OR,
		"PLUS",                   PLUS,
		"MINUS",                  MINUS,
		"SLASH",                  SLASH,
		"PERCENT",                PERCENT,
		"STAR",                   STAR,
		"DOT",                    DOT,
		"COLON",                  COLON,
		"QUERY",                  QUERY,
		"SEMI",                   SEMI,
		"COMMA",                  COMMA,
		"LPAREN",                 LPAREN,
		"RPAREN",                 RPAREN,
		"LBRACE",                 LBRACE,
		"RBRACE",                 RBRACE,
		"LBRAC",                  LBRAC,
		"RBRAC",                  RBRAC,
		"EQUALS",                 EQUALS,
		"STAR_EQUALS",            STAR_EQUALS,
		"SLASH_EQUALS",           SLASH_EQUALS,
		"PERCENT_EQUALS",         PERCENT_EQUALS,
		"PLUS_EQUALS",            PLUS_EQUALS,
		"MINUS_EQUALS",           MINUS_EQUALS,
		"LSHIFT_EQUALS",          LSHIFT_EQUALS,
		"RSHIFT_EQUALS",          RSHIFT_EQUALS,
		"AND_EQUALS",             AND_EQUALS,
		"XOR_EQUALS",             XOR_EQUALS,
		"OR_EQUALS",              OR_EQUALS,
		"ANDAND",                 ANDAND,
		"OROR",                   OROR,
		"EQEQ",                   EQEQ,
		"NOTEQ",                  NOTEQ,
		"GTEQ",                   GTEQ,
		"LESSEQ",                 LESSEQ,
		"LSHIFT",                 LSHIFT,
		"RSHIFT",                 RSHIFT,
		"PLUSPLUS",               PLUSPLUS,
		"MINUSMINUS",             MINUSMINUS,
		"ARROW",                  ARROW,
		"ELLIPSIS",               ELLIPSIS,
		"STRING_CONSTANT",        STRING_CONSTANT,
		"INTEGER_CONSTANT",       INTEGER_CONSTANT,
		"CHARACTER_CONSTANT",     CHARACTER_CONSTANT,
		"FLOATING_CONSTANT",      FLOATING_CONSTANT,
		"IDENTIFIER",             IDENTIFIER,
		"TYPEDEF_NAME",           TYPEDEF_NAME,
		"BADTOK",                 BADTOK,
		"EOF",                    0,
	};
	static char buf[100];
	int i;

	for (i = 0; i < sizeof tab / sizeof *tab; ++i)
		if (tab[i].token == token)
			return tab[i].name;

	(void) sprintf(buf, "<unknown token %d>", token);
	return buf;
}


void *safe_calloc(size_t n, size_t s)
{
	void *p = calloc(n,s);
	if (!p) {
		fprintf(stderr, "Error: out of memory\n");
		exit(1);
	}
	return p;
}

#include "alloc.h"
static allocator *String_allocator = 0;

char *string_copy(const char *string, int len)
{
	char *p;
	if (String_allocator == 0) {
		String_allocator = new_allocator(0, 100);
	}
	p = allocate(String_allocator, len+1);
	strncpy(p, string, len);
	p[len] = 0;
	return p;
}
	
	

	
	
