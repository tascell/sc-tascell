
/* c_parser.c - a C 99 parser */

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*  Copyright 2000, 2001 Dibyendu Majumdar.
 *  Author : Dibyendu Majumdar
 *  Email  : dibyendu@mazumdar.demon.co.uk  
 *  Website: www.mazumdar.demon.co.uk
 *
 */

/*
 * 12 Jan 2000 - Started
 * 13 Jan 2000 - Completed 1st version
 * 14-15 Jan 2000 - Fixed many bugs, added TRACE()
 * 16 Jan 2001 - Streamlined TokMap, and token test macros
 * 16 Jan 2001 - Documented FIRST sets, ambiguities
 * 17 Jan 2001 - Started using CVS
 * 17 Jan 2001 - Imported typedef handling from UPS sources
 * 18 Jan 2001 - First version that can parse typedefs
 *               BUG: a typedef name can be hidden by declaring a function,
 *                    variable, or enumerator with the same name. Currently
 *                    cannot handle this.
 * 19 Jan 2001 - Started work on C99 grammer.
 * 19 Jan 2001 - Added support for designators in initializers
 * 19 Jan 2001 - Modified external declaration so that these can no longer
 *               begin without a declaration specifier.
 * 19 Jan 2001 - Selection (if, switch) and iteration (for, do, while)
 *               statements now enclosed in blocks, sub statements also 
 *               enclosed in blocks.
 * 19 Jan 2001 - First expression of for can now be a declaration.
 * 19 Jan 2001 - Compound statements can now have declartions and statements
 *               interspersed.
 * 20 Jan 2001 - Tentative code for parsing compound literals.
 * 20 Jan 2001 - Structured trace messages to show parser in action.
 * 16 Nov 2001 - Tidied up for release. As you can see I have not worked on this since Jan, 2001.
 *               It is unlikely I will be able to spend time on this in the immediate future. Hence,
 *               I am releasing it "as is".
 */

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "c_lex.h"
#include "list.h"

#include "c_parser.h"
#include <err.h>

/***
* Various FIRST SETS
* ------------------
*   declarator
*        IDENTIFIER * ( 
*   declaration_specifier
*        AUTO REGISTER STATIC EXTERN TYPEDEF VOID CHAR
*        SHORT INT LONG FLOAT DOUBLE SIGNED UNSIGNED STRUCT UNION
*        ENUM TYPEDEF_NAME CONST VOLATILE
*   declaration
*        declaration_specifier
*   external_declaration
*        declaration declarator
*   expression
*        ( ++ -- & * + - ~ ! SIZEOF IDENTIFIER FLOATING_CONSTANT
*        INTEGER_CONSTANT CHARACTER_CONSTANT STRING_CONSTANT
*   statement
*        expression ; { IF SWITCH WHILE DO FOR GOTO CONTINUE BREAK
*        RETURN CASE DEFAULT IDENTIFIER
*/

/***
* Ambiguity
*    1) A declaration and a function definition look alike.
*    2) An expression and a statement look alike.
*    3) A type cast and an expression look alike.
*    4) A labeled statment and an expression statement look alike.
*    5) An assignment and a conditional expression look alike.
*/

#define is_assign_operator(tok) \
	(TokMap[(tok)] & TOK_ASSGNOP)
#define is_type_name(tok) \
	(TokMap[(tok)] & TOK_DECL_SPEC || ((tok) == IDENTIFIER && Cursym != 0 && Cursym->object_type == OBJ_TYPEDEF_NAME) )
#define is_declaration(tok) \
	is_type_name(tok)
#ifdef C89
#define is_external_declaration(tok) \
	(is_declaration(tok) || (tok) == STAR || (tok) == LPAREN || (tok) == IDENTIFIER)
#else
#define is_external_declaration(tok) \
	is_declaration(tok)
#endif
#define is_expression(tok) \
	(TokMap[tok] & TOK_EXPR)
#define is_statement(tok) \
	(TokMap[tok] & TOK_STMT)
#define is_function_body(tok) \
	(tok == LBRACE || (is_declaration(tok) && (tok) != TYPEDEF))

#if 1
#define TRACEIN(s) do { if (DebugLevel == 2) printf("%*s%s {\n", TraceLevel, "", s); fflush(stdout); TraceLevel++; } while(0);
#define TRACEOUT(s) do { --TraceLevel; if (DebugLevel == 2) printf("%*s} (%s)\n", TraceLevel, "", s); fflush(stdout); } while(0);
#else
#define TRACEIN(s)
#define TRACEOUT(s)
#endif

enum
{
  TOK_UNKNOWN = 0,
  TOK_EXPR = 1,
  TOK_CONSTANT = 2,
  TOK_TYPE_SPECIFIER_QUALIFIER = 4,
  TOK_TYPE_SPECIFIER = 8,
  TOK_TYPE_QUALIFIER = 16,
  TOK_TAG = 32,
  TOK_STRUCT = 64,
  TOK_STORAGE_CLASS = 256,
  TOK_STMT = 1024,
  TOK_DECL_SPEC = 2048,
  TOK_ASSGNOP = 4096,
};

enum
{
  LEVEL_GLOBAL = 0,
  LEVEL_FUNCTION = 1,
  LEVEL_STATEMENT = 2
};

enum
{
  OBJ_TYPEDEF_NAME = 1,
  OBJ_FUNCTION_DECL = 2,
  OBJ_FUNCTION_DEFN = 4,
  OBJ_ENUMERATOR = 8,
  OBJ_VARIABLE = 16,
  OBJ_PARAMETER = 32,
  OBJ_IDENTIFIER = 2 + 4 + 8 + 16 + 32
};

typedef struct symbol_t
{
  link_t link;
  const char *name;
  int storage_class;
  int object_type;
} symbol_t;

typedef struct
{
  link_t link;
  int level;			/* nesting level */
  list_t symbols;		/* list of symbols */
} symtab_t;

static unsigned long TokMap[BADTOK];

static int DebugLevel = 0;
static int TraceLevel = 0;

static int Level = 0;
static int Saw_ident = 0;
static int Is_func = 0;
static token_t tok;
static int Parsing_struct = 0;
static int Parsing_oldstyle_parmdecl = 0;
static int Storage_class[100];
static int stack_ptr = -1;

static list_t identifiers;	/* head of the identifiers list */
static list_t labels;
static list_t types;
static symtab_t *Cursymtab;	/* current symbol table */
static symtab_t *Curlabels;
static symtab_t *Curtypes;
static symbol_t *Cursym = 0;

static void init_tokmap (void);
static const char *object_name (int object_type);
static symtab_t *new_symbol_table (list_t * owner);
static void init_symbol_table (void);
static symbol_t *find_symbol (symtab_t * tab, const char *name, int all_scope);
static void enter_scope (void);
static void exit_scope (void);
static void
install_symbol (const char *name, int storage_class, int object_type);
static void match (token_t expected_tok);
static bool check_not_typedef (void);

static t_constant_expression *constant_expression (void);
static t_expression *expression (void);
static t_primary_expression *primary_expression (void);
static t_postfix_operator *postfix_operator (void);
static t_postfix_operators *postfix_operators (void);
static t_sizeof_expression *sizeof_expression (void);
static t_unary_expression *unary_expression (void);
static t_multiplicative_expression *multiplicative_expression (void);
static t_additive_expression *additive_expression (void);
static t_shift_expression *shift_expression (void);
static t_relational_expression *relational_expression (void);
static t_equality_expression *equality_expression (void);
static t_and_expression *and_expression (void);
static t_exclusive_or_expression *exclusive_or_expression (void);
static t_inclusive_or_expression *inclusive_or_expression (void);
static t_logical_and_expression *logical_and_expression (void);
static t_logical_or_expression *logical_or_expression (void);
static t_conditional_expression *conditional_expression (void);
static t_assignment_expression *assignment_expression (void);
static t_labeled_statement *labeled_statement (void);
static t_case_statement *case_statement (void);
static t_default_statement *default_statement (void);
static t_if_statement *if_statement (void);
static t_switch_statement *switch_statement (void);
static t_while_statement *while_statement (void);
static t_do_while_statement *do_while_statement (void);
static t_for_statement *for_statement (void);
static t_break_statement *break_statement (void);
static t_continue_statement *continue_statement (void);
static t_goto_statement *goto_statement (void);
static t_return_statement *return_statement (void);
static t_empty_statement *empty_statement (void);
static t_expression_statement *expression_statement (void);
static t_statement *statement (void);
static t_compound_statement *compound_statement (void);
static t_enumerator *enumerator (void);
static t_enum_specifier *enum_specifier (void);
static t_member *member (void);
static t_members *members (void);
static t_struct_or_union_specifier *struct_or_union_specifier (void);
static t_type_name *type_name (void);
static t_declaration_specifiers *declaration_specifiers (int no_storage_class);
static t_pointer *pointer (void);
static t_direct_declarator *direct_declarator (int abstract);
static t_parameter_list *parameter_list (int *new_style);
static t_suffix_declarator *suffix_declarator (void);
static t_declarator *declarator (int abstract);
static t_designator *designator (void);
static t_initializer *initializer (int recurse);
static t_function_definition *function_definition (void);
static t_init_declarator *init_declarator (int check_if_function);
static t_declaration *declaration (void);
static t_translation_unit *translation_unit (void);

static const char *mygetline (char *arg);

char token_str[BUF_SIZE];     // added

static void
init_tokmap (void)
{
  TokMap[FLOATING_CONSTANT] = TOK_CONSTANT | TOK_EXPR | TOK_STMT;
  TokMap[INTEGER_CONSTANT] = TOK_CONSTANT | TOK_EXPR | TOK_STMT;
  TokMap[STRING_CONSTANT] = TOK_CONSTANT | TOK_EXPR | TOK_STMT;
  TokMap[CHARACTER_CONSTANT] = TOK_CONSTANT | TOK_EXPR | TOK_STMT;
  TokMap[IDENTIFIER] = TOK_EXPR | TOK_STMT;
  TokMap[SIZEOF] = TOK_EXPR | TOK_STMT;
  TokMap[AND] = TOK_EXPR | TOK_STMT;
  TokMap[PLUSPLUS] = TOK_EXPR | TOK_STMT;
  TokMap[MINUSMINUS] = TOK_EXPR | TOK_STMT;
  TokMap[STAR] = TOK_EXPR | TOK_STMT;
  TokMap[PLUS] = TOK_EXPR | TOK_STMT;
  TokMap[MINUS] = TOK_EXPR | TOK_STMT;
  TokMap[TILDE] = TOK_EXPR | TOK_STMT;
  TokMap[LPAREN] = TOK_EXPR | TOK_STMT;
  TokMap[NOT] = TOK_EXPR | TOK_STMT;
  TokMap[TYPEDEF_NAME] =
      TOK_TYPE_SPECIFIER_QUALIFIER | TOK_TYPE_SPECIFIER | TOK_DECL_SPEC;
  TokMap[CHAR] =
      TOK_TYPE_SPECIFIER_QUALIFIER | TOK_TYPE_SPECIFIER | TOK_DECL_SPEC;
  TokMap[FLOAT] =
      TOK_TYPE_SPECIFIER_QUALIFIER | TOK_TYPE_SPECIFIER | TOK_DECL_SPEC;
  TokMap[DOUBLE] =
      TOK_TYPE_SPECIFIER_QUALIFIER | TOK_TYPE_SPECIFIER | TOK_DECL_SPEC;
  TokMap[SHORT] =
      TOK_TYPE_SPECIFIER_QUALIFIER | TOK_TYPE_SPECIFIER | TOK_DECL_SPEC;
  TokMap[INT] =
      TOK_TYPE_SPECIFIER_QUALIFIER | TOK_TYPE_SPECIFIER | TOK_DECL_SPEC;
  TokMap[UNSIGNED] =
      TOK_TYPE_SPECIFIER_QUALIFIER | TOK_TYPE_SPECIFIER | TOK_DECL_SPEC;
  TokMap[SIGNED] =
      TOK_TYPE_SPECIFIER_QUALIFIER | TOK_TYPE_SPECIFIER | TOK_DECL_SPEC;
  TokMap[VOID] =
      TOK_TYPE_SPECIFIER_QUALIFIER | TOK_TYPE_SPECIFIER | TOK_DECL_SPEC;
  TokMap[STRUCT] =
      TOK_TYPE_SPECIFIER_QUALIFIER | TOK_STRUCT | TOK_TAG | TOK_DECL_SPEC;
  TokMap[UNION] =
      TOK_TYPE_SPECIFIER_QUALIFIER | TOK_STRUCT | TOK_TAG | TOK_DECL_SPEC;
  TokMap[ENUM] = TOK_TYPE_SPECIFIER_QUALIFIER | TOK_TAG | TOK_DECL_SPEC;
  TokMap[LONG] =
      TOK_TYPE_SPECIFIER_QUALIFIER | TOK_TYPE_SPECIFIER | TOK_DECL_SPEC;
  TokMap[CONST] =
      TOK_TYPE_SPECIFIER_QUALIFIER | TOK_TYPE_QUALIFIER | TOK_DECL_SPEC;
  TokMap[VOLATILE] =
      TOK_TYPE_SPECIFIER_QUALIFIER | TOK_TYPE_QUALIFIER | TOK_DECL_SPEC;
  TokMap[STATIC] = TOK_STORAGE_CLASS | TOK_DECL_SPEC;
  TokMap[EXTERN] = TOK_STORAGE_CLASS | TOK_DECL_SPEC;
  TokMap[AUTO] = TOK_STORAGE_CLASS | TOK_DECL_SPEC;
  TokMap[REGISTER] = TOK_STORAGE_CLASS | TOK_DECL_SPEC;
  TokMap[TYPEDEF] = TOK_STORAGE_CLASS | TOK_DECL_SPEC;
  TokMap[IF] = TOK_STMT;
  TokMap[BREAK] = TOK_STMT;
  TokMap[CASE] = TOK_STMT;
  TokMap[CONTINUE] = TOK_STMT;
  TokMap[DEFAULT] = TOK_STMT;
  TokMap[DO] = TOK_STMT;
  TokMap[ELSE] = TOK_STMT;
  TokMap[FOR] = TOK_STMT;
  TokMap[GOTO] = TOK_STMT;
  TokMap[RETURN] = TOK_STMT;
  TokMap[SWITCH] = TOK_STMT;
  TokMap[WHILE] = TOK_STMT;
  TokMap[LBRACE] = TOK_STMT;
  TokMap[SEMI] = TOK_STMT;
  TokMap[EQUALS] = TOK_ASSGNOP;
  TokMap[PLUS_EQUALS] = TOK_ASSGNOP;
  TokMap[MINUS_EQUALS] = TOK_ASSGNOP;
  TokMap[STAR_EQUALS] = TOK_ASSGNOP;
  TokMap[SLASH_EQUALS] = TOK_ASSGNOP;
  TokMap[PERCENT_EQUALS] = TOK_ASSGNOP;
  TokMap[LSHIFT_EQUALS] = TOK_ASSGNOP;
  TokMap[RSHIFT_EQUALS] = TOK_ASSGNOP;
  TokMap[AND_EQUALS] = TOK_ASSGNOP;
  TokMap[XOR_EQUALS] = TOK_ASSGNOP;
  TokMap[OR_EQUALS] = TOK_ASSGNOP;

}

static const char *
object_name (int object_type)
{
  const char *name;

  switch (object_type)
    {
    case OBJ_TYPEDEF_NAME:
      name = "typedef";
      break;
    case OBJ_FUNCTION_DECL:
      name = "function_decl";
      break;
    case OBJ_FUNCTION_DEFN:
      name = "function_defn";
      break;
    case OBJ_ENUMERATOR:
      name = "enumerator";
      break;
    case OBJ_VARIABLE:
      name = "variable";
      break;
    case OBJ_PARAMETER:
      name = "parameter";
      break;
    case OBJ_IDENTIFIER:
      name = "identifier";
      break;
    default:
      name = "invalid";
      break;
    }
  return name;
}

static symtab_t *
new_symbol_table (list_t * owner)
{
  symtab_t *tab;

  tab = calloc (1, sizeof (symtab_t));
  list_init (&tab->symbols);
  list_append (owner, tab);
  return tab;
}

static void
init_symbol_table (void)
{
  list_init (&identifiers);
  Cursymtab = new_symbol_table (&identifiers);
  list_init (&labels);
  list_init (&types);
  Curtypes = new_symbol_table (&types);
}

static symbol_t *
find_symbol (symtab_t * tab, const char *name, int all_scope)
{
  symbol_t *sym;

  assert (tab != 0);
  for (sym = (symbol_t *) list_first (&tab->symbols);
       sym != 0; sym = (symbol_t *) list_next (&tab->symbols, sym))
    {
      if (strcmp (sym->name, name) == 0)
	{
	  return sym;
	}
    }
  if (!all_scope)
    return 0;
  tab = (symtab_t *) list_prev (&identifiers, tab);
  if (tab == 0)
    return 0;
  return find_symbol (tab, name, all_scope);
}

static void
enter_scope (void)
{
  /* symtab_t *tab; */

  Level++;
  if (DebugLevel == 3)
    {
      printf ("%*sEntering scope %d\n", TraceLevel, "", Level);
    }
  if (Level == LEVEL_STATEMENT)
    {
      /* Although we increment the level when we parse 
       * function parameters, and again when we enter the
       * function body, ANSI C requires these to be in the same
       * scope.
       */
      return;
    }
  /*
     tab = calloc(1, sizeof(symtab_t));
     list_init(&tab->symbols);
     list_append(&identifiers, tab);
     Cursymtab = tab;
   */
  Cursymtab = new_symbol_table (&identifiers);
}

static void
exit_scope (void)
{
  symtab_t *tab;

  if (DebugLevel == 3)
    {
      printf ("%*sExiting scope %d\n", TraceLevel, "", Level);
    }
  Level--;
  /* Although we increment the Level when we parse 
   * function parameters, and again when we enter the
   * function body, ANSI C requires these to be in the same
   * scope.
   */
  if (Level == LEVEL_FUNCTION)
    return;
  tab = Cursymtab;
  Cursymtab = (symtab_t *) list_prev (&identifiers, Cursymtab);
  assert (Cursymtab != 0);
  list_remove (&identifiers, tab);
}

static void
install_symbol (const char *name, int storage_class, int object_type)
{
  symbol_t *sym;

  sym = find_symbol (Cursymtab, name, 0);
  if (sym != 0)
    {
      fprintf (stderr, "Error: redeclaration of symbol %s as %s\n", name,
	       object_name (object_type));
      fprintf (stderr, "Error: previously declared as %s\n",
	       object_name (sym->object_type));
      /* fprintf(stderr, "Level = %d\n", Level); */
      /* exit(1); */
    }
  if (DebugLevel == 3)
    {
      printf ("%*sInstalling %s name %s\n", TraceLevel, "",
	      object_name (object_type), name);
      if (sym)
	{
	  printf ("%*s\tOverriding %s name %s\n", TraceLevel, "",
		  object_name (sym->object_type), sym->name);
	}
    }
  sym = safe_calloc (1, sizeof (symbol_t));
  sym->name = string_copy (name, strlen (name));
  sym->storage_class = storage_class;
  sym->object_type = object_type;
  list_append (&Cursymtab->symbols, sym);
}

static void
match (token_t expected_tok)
{
  if (tok != expected_tok)
    {
      printf ("Parse failed: Expected %s, ", tokname (expected_tok));
      printf ("got %s\n", tokname (tok));
      /* printf("Level = %d\n", Level); */
      exit (1);
    }
  else
    {
      if (DebugLevel)
	{
	  const char *cp = 0;

	  if (TokMap[tok] & TOK_CONSTANT)
	    {
	      cp = Lexeme->constant->co_val;
	    }
	  else if (tok == IDENTIFIER)
	    {
	      cp = Lexeme->identifier->id_name;
	    }
	  if (cp == 0)
	    printf ("%*s[%s]\n", TraceLevel, "", tokname (tok));
	  else
	    printf ("%*s[%s(%s)]\n", TraceLevel, "", tokname (tok), cp);
	}
      tok = lex_get_token ();
    }
}

static bool
check_not_typedef (void)
{
  return (Cursym == 0 || Cursym->object_type != OBJ_TYPEDEF_NAME);
}

/***************************************************************************/

/* C-identifier -> SC-identifier */
void cid2scid( const char *cid, char *scid )
{
  int lower_mode = 1;   /* 0:upper, 1:lower */
  for( ; *cid != EOS ; cid++ ){
    // alphabet( lower )
    if( islower( *cid ) ){
      if( !lower_mode ){
        *(scid++) = '@';
        lower_mode = 1;
      }
      *(scid++) = *cid;
    }
    // upper
    else if( isupper( *cid ) ){
      if( lower_mode ){
        *(scid++) = '@';
        lower_mode = 0;
      }
      *(scid++) = *cid;
    }
    // otherwise
    else{
      *(scid++) = *cid;
    }
  }
  *scid = EOS;
}

void string_c2sc( const char *cstr, char *scstr )
{
  const char *c = cstr;

  if( *(c++) != '"' ){
    fprintf( stderr, "%s ", cstr );
    perror( "is not a constant literal!!" );
  }

  *(scstr++) = '"';

  for( ; *c != '"' ; c++ ){
    switch( *c )
      {
      case '~':
	*(scstr++)='~';  *(scstr++)='~';
	break;
      case '\\':
	switch( *(++c) ){
	case 'n':
	  *(scstr++)='~';   *(scstr++)='%';   break;
	case 'r':
	  *(scstr++)='~';   *(scstr++)='&';   break;
	case 'f':
	  *(scstr++)='~';   *(scstr++)='|';   break;
	case '\\':
	  *(scstr++)='\\';  *(scstr++)='\\';  break;
	case '\"': 
	  *(scstr++)='\\';  *(scstr++)='\"';  break;
	case '\'':
	  *(scstr++)=*c;                      break;
	default:
	  warn( "SC does not handle escape character \\%c", *c );
	  *(scstr++)='\\';  *(scstr++)='\\';
	  *(scstr++)=*c;                      break;
	}
	break;
      default:
	*(scstr++)=*c;
	break;
      }
  }
  *(scstr++) = '"';
  *scstr = EOS;
  return;
}

/***************************************************************************/

static void prt_constant_expression( t_constant_expression *const_exp )
{
  //  prt_conditional_expression( const_exp->cond_exp );
}

static void free_constant_expression( t_constant_expression *const_exp )
{
  if( const_exp ){
    //    free_conditional_expression( const_exp->cond_exp );
    free( const_exp );
  }
}

static t_constant_expression*
constant_expression (void)
{
  t_constant_expression *ret;
  ret = malloc( sizeof( t_constant_expression));
  // constant-expression:
  //   conditional-expression
  TRACEIN ("constant_expression");
  ret->cond_exp = conditional_expression ();
  /* fold constant */
  TRACEOUT ("constant_expression");
  return ret;
}

/***************************************************************************/

static void prt_expression( t_expression *exp )
{
  if( exp->exp ){
    fputs( " (exps ", sc_out );
    prt_expression( exp->exp );
    //    prt_assignment_expression( exp->ass_exp );
    fputs( ") ", sc_out );
  } else {
    //    prt_assignment_expression( exp->ass_exp );
  }
}
    
static void free_expression( t_expression *exp )
{
  if( exp ){
    free_expression( exp->exp );
    //    free_assignment_expression( exp->ass_exp );
    free( exp );
  }
}

static t_expression*
expression (void)
{
  t_expression *ret, *new_ret;
  // expression:
  //   assignment-expression
  //   expression , assignment-expression
  TRACEIN ("expression");

  if (!is_expression (tok))
    {
      TRACEOUT ("expression");
      return NULL;
    }

  ret = malloc( sizeof( t_expression));
  ret->exp = NULL;
  ret->ass_exp = assignment_expression ();

  while (tok == COMMA)
    {
      match (COMMA);
      new_ret = malloc( sizeof( t_expression));
      new_ret->exp = ret;
      new_ret->ass_exp = assignment_expression ();
      ret = new_ret;
    }
  TRACEOUT ("expression");
  return ret;
}

/***************************************************************************/

static void prt_primary_expression( t_primary_expression *pr_exp )
{
  static char str[BUF_SIZE];
  
  switch( pr_exp->spec )
    {
    case PR_IDENTIFIER:
      cid2scid( pr_exp->str, str );
      fprintf( sc_out, " %s ", str );
      break;
      
    case PR_CONSTANT:
      fprintf( sc_out, " %s ", pr_exp->str );
      break;
      
    case PR_STRING_LITERAL:
      string_c2sc( pr_exp->str, str );
      fprintf( sc_out, " %s ", str );
      break;

    default:
      perror( "unexpected value of pr_exp->spe" );
    }
}

static void free_primary_expression( t_primary_expression *pr_exp )
{
  free( pr_exp->str );
  free( pr_exp );
}

static t_primary_expression*
primary_expression (void)
{
  t_primary_expression *ret;
  ret = malloc( sizeof( t_primary_expression));
  ret->spec = PR_UNDEFINED;
  ret->str = NULL;
  // primary-expression:
  //   identifier
  //   constant
  //   string-literal
  //   ( expression )    --> in unary_expression
  TRACEIN ("primary_expression");
  if (tok == IDENTIFIER)
    {
      check_not_typedef ();
      ret->spec = PR_IDENTIFIER;
      ret->str = malloc( strlen( token_str)+ 1);
      strcpy( ret->str, token_str );
      match (IDENTIFIER);
    }
  else if (TokMap[tok] & TOK_CONSTANT)
    {
      if( token_str[0] != '"' )
	ret->spec = PR_CONSTANT;
      else 
	ret->spec = PR_STRING_LITERAL;
      ret->str = malloc( strlen( token_str)+ 1);
      strcpy( ret->str, token_str );
      match (tok);
    }
  if( ret->spec == PR_UNDEFINED ){
    free( ret );  
    ret = NULL; 
  }
  /* parenthesized expression handled in unary_expression() */
  TRACEOUT ("primary_expression");
  return ret;
}

/***************************************************************************/

// 保留
static void prt_postfix_operator( t_postfix_operator *pf )
{
  switch( pf->spec )
    {
    case PF_UNDEFINED:
    case PF_AREF:
    
    case PF_CALL:
      
    case PF_ARROW:
    case PF_DOT:
      
    case PF_INC:
    case PF_DEC:;
    }
}

static void free_postfix_operator( t_postfix_operator *pf )
{
  if( pf ){
    free_expression( pf->exp );
    free( pf->identifier );
    free( pf );
  }
}

static t_postfix_operator *
postfix_operator (void)
{
  t_postfix_operator *ret;
  ret = malloc( sizeof( t_postfix_operator));
  ret->spec = PF_UNDEFINED;
  ret->exp = NULL;  ret->identifier = NULL;
  
  TRACEIN ("postfix_operator");
  if (tok == LBRAC)
    {
      ret->spec = PF_AREF;
      match (LBRAC);
      ret->exp = expression ();
      match (RBRAC);
    }
  else if (tok == LPAREN)
    {
      ret->spec = PF_CALL;
      match (LPAREN);
      //if (tok != RPAREN)
      //{
      //  assignment_expression ();
      //  while (tok == COMMA)
      //    {
      //      match (COMMA);
      //      assignment_expression ();
      //    }
      //}
      ret->exp = expression ();      
      match (RPAREN);
    }
  else if (tok == DOT || tok == ARROW)
    {
      ret->spec = (tok==DOT)?(PF_DOT):(PF_ARROW);
      match (tok);
      ret->identifier = malloc( strlen( token_str)+1);
      strcpy( ret->identifier, token_str);
      match (IDENTIFIER);
    }
  else if (tok == PLUSPLUS || tok == MINUSMINUS)
    {
      ret->spec = (tok==PLUSPLUS)?(PF_INC):(PF_DEC);
      match (tok);
    }
  TRACEOUT ("postfix_operator");
  return ret;
}

/***************************************************************************/

static void prt_postfix_operators( t_postfix_operators *pfs )
{
  prt_postfix_operator( pfs->pf );
  if( pfs->next )
    prt_postfix_operators( pfs->next );  
}

static void free_postfix_operators( t_postfix_operators *pfs )
{
  if( pfs ){
    free_postfix_operator( pfs->pf );
    free_postfix_operators( pfs->next );
    free( pfs );
  }
}

static t_postfix_operators *
postfix_operators (void)
{
  t_postfix_operators *ret=NULL;
  t_postfix_operators **cur=&ret;

  TRACEIN ("postfix_operators");
  while (tok == LBRAC || tok == LPAREN || tok == DOT ||
	 tok == ARROW || tok == PLUSPLUS || tok == MINUSMINUS)
    {
      *cur = malloc( sizeof( t_postfix_operators));
      (*cur)->pf = postfix_operator ();
      (*cur)->next = NULL;
      cur = &((*cur)->next);
    }
  TRACEOUT ("postfix_operators");
  return ret;
}

/***************************************************************************/

static void prt_sizeof_expression( t_sizeof_expression *sz_exp )
{
  // 保留
}

static void free_sizeof_expression( t_sizeof_expression *sz_exp )
{
  if( sz_exp ){
    free_expression( sz_exp->exp );
    free_postfix_operators( sz_exp->pfs );
    free_unary_expression( sz_exp->un_exp );
    //    free_type_name( sz_exp->tn );
    free( sz_exp );
  }
}

static t_sizeof_expression *
sizeof_expression (void)
{
  t_sizeof_expression *ret;
  ret = malloc( sizeof( t_sizeof_expression));
  ret->spec = SZ_UNDEFINED;
  ret->exp = NULL;     ret->pfs = NULL;
  ret->un_exp = NULL;  ret->tn = NULL;
  // sizeof_expression:
  //   sizeof unary-expression
  //   sizeof ( type-name )
  TRACEIN ("sizeof_expression");

  match (SIZEOF);
  if (tok == LPAREN)
    {
      int found_typename = 0;

      match (LPAREN);
      if (is_type_name (tok))
	{
	  ret->spec = SZ_TYPE;
	  ret->tn = type_name ();
	  found_typename = 1;
	}
      else
	{
	  ret->spec = SZ_PR_EXP;
	  ret->exp = expression ();
	}
      match (RPAREN);
      
#if 1				/* as per comp.std.c */
      // compound literal ( SCは未対応 )
      /*
      if (found_typename && tok == LBRACE)
	{
	  initializer (0);
	  postfix_operators ();
	}
      */
#endif
      //else 
      if (!found_typename)
	{
	  ret->pfs = postfix_operators ();
	}
    }
  else
    {
      ret->spec = SZ_UN_EXP;
      ret->un_exp = unary_expression ();
    }
  TRACEOUT ("sizeof_expression");
  return ret;
}

/***************************************************************************/

static void prt_unary_expression( t_unary_expression *un_exp )
{
  // 保留
}

static void free_unary_expression( t_unary_expression *un_exp )
{
  if( un_exp ){
    free_sizeof_expression( un_exp->sz_exp );
    //    free_type_name( un_exp->tn );            
    free_unary_expression( un_exp->un_exp ); 
    free_expression( un_exp->exp );          
    free_postfix_operators( un_exp->pfs );    
    free_primary_expression( un_exp->pr_exp );
    free( un_exp );
  }
}

static t_unary_expression *
unary_expression (void)
{
  t_unary_expression *ret;
  ret = malloc( sizeof( t_unary_expression));
  ret->spec = UE_UNDEFINED;
  ret->uop = UOP_UNDEFINED;
  ret->sz_exp = NULL;  ret->tn = NULL;  ret->un_exp = NULL;  ret->exp = NULL; 
  ret->pfs = NULL;  ret->pr_exp = NULL;
  // unary-expression:
  //   postfix-expression
  //   ++ unary-expression
  //   -- unary-expression
  //   unary-operator cast-expression
  //   sizeof-expression
  // postfix-expression:
  //   primary-expression
  //   primary-expression postfix-operators
  // cast-expression:
  //   unary-expression
  //   ( type-name ) cast-expression
  TRACEIN ("unary_expression");
  if (tok == SIZEOF)
    {
      ret->spec = UE_SIZEOF;
      ret->sz_exp = sizeof_expression ();
    }
  else if (tok == LPAREN)
    {
      int found_typename = 0;

      match (LPAREN);
      if (is_type_name (tok))
	{
	  ret->spec = UE_CAST;
	  ret->tn = type_name ();
	  found_typename = 1;
	}
      else
	{
	  ret->spec = UE_PAREN;
	  ret->exp = expression ();
	}
      match (RPAREN);
      // compound literal ( SCは未対応 )
      /*
      if (found_typename && tok == LBRACE)
	{
	  initializer (0);
	  postfix_operators ();
	}
	else
      */
      if (!found_typename)
	{
	  ret->pfs = postfix_operators ();
	}
      else
	{
	  ret->un_exp = unary_expression ();
	}
    }
  else if (tok == PLUSPLUS || tok == MINUSMINUS || tok == AND
	   || tok == STAR || tok == PLUS || tok == MINUS
	   || tok == TILDE || tok == NOT)
    {
      ret->spec = UE_UN_OP;
      switch( tok ){
      case PLUSPLUS:    ret->uop = UOP_PLUSPLUS;    break;
      case MINUSMINUS:  ret->uop = UOP_MINUSMINUS;  break;
      case AND:         ret->uop = UOP_AND;         break;
      case STAR:        ret->uop = UOP_STAR;        break;
      case PLUS:        ret->uop = UOP_PLUS;        break;
      case MINUS:       ret->uop = UOP_MINUS;       break;
      case TILDE:       ret->uop = UOP_TILDE;       break;
      case NOT:         ret->uop = UOP_NOT;         break;
      default: 
	perror( "unexpcted type of tok" );
	break;
      }
      match (tok);
      ret->un_exp = unary_expression ();
    }
  else
    {
      ret->spec = UE_PRIM;
      ret->pr_exp = primary_expression ();
      ret->pfs = postfix_operators ();
    }
  TRACEOUT ("unary_expression");
  return ret;
}

/***************************************************************************/

static t_multiplicative_expression *
multiplicative_expression (void)
{
  t_multiplicative_expression *ret;
  t_multiplicative_expression *new_ret;  
  ret = malloc( sizeof( t_multiplicative_expression));
  ret->ml_exp = NULL;
  ret->mop = MOP_UNDEFINED;
  // multiplicative-expression:
  //   cast-expression
  //   multiplicative-expression * cast-expression
  //   multiplicative-expression / cast-expression
  //   multiplicative-expression % cast-expression
  TRACEIN ("multiplicative_expression");
  ret->un_exp = unary_expression ();
  while (tok == STAR || tok == SLASH || tok == PERCENT)
    {
      new_ret = malloc( sizeof( t_multiplicative_expression));
      switch( tok ){
      case STAR:    new_ret->mop=MOP_STAR;    break;
      case SLASH:   new_ret->mop=MOP_SLASH;   break;
      case PERCENT: new_ret->mop=MOP_PERCENT; break;
      default: 
	perror( "unexpcted type of tok" );
	break;
      }
      match (tok);
      new_ret->ml_exp = ret;
      new_ret->un_exp = unary_expression ();
      ret = new_ret;
    }
  TRACEOUT ("multiplicative_expression");
  return ret;
}

/***************************************************************************/

static t_additive_expression *
additive_expression (void)
{
  t_additive_expression *ret;
  t_additive_expression *new_ret;  
  ret = malloc( sizeof( t_additive_expression));
  ret->ad_exp = NULL;
  ret->aop = AOP_UNDEFINED;
  // additive-expression:
  //   multiplicative-expression
  //   additive-expression + multiplicative-expression
  //   additive-expression - multiplicative-expression
  TRACEIN ("additive_expression");
  ret->ml_exp = multiplicative_expression ();
  while (tok == PLUS || tok == MINUS)
    {
      new_ret = malloc( sizeof( t_additive_expression));
      switch( tok ){
      case PLUS:    new_ret->aop=AOP_PLUS;    break;
      case MINUS:   new_ret->aop=AOP_MINUS;   break;
      default: 
	perror( "unexpcted type of tok" );
	break;
      }
      match (tok);
      new_ret->ad_exp = ret;
      new_ret->ml_exp = multiplicative_expression ();
      ret = new_ret;
    }
  TRACEOUT ("additive_expression");
  return ret;
}

/***************************************************************************/

static t_shift_expression *
shift_expression (void)
{
  t_shift_expression *ret;
  t_shift_expression *new_ret;  
  ret = malloc( sizeof( t_shift_expression));
  ret->sh_exp = NULL;
  ret->sop = SOP_UNDEFINED;
  // shift-expression:
  //   additive-expression
  //   shift-expression << additive-expression
  //   shift-expression >> additive-expression
  TRACEIN ("shift_expression");
  ret->ad_exp = additive_expression ();
  while (tok == LSHIFT || tok == RSHIFT)
    {
      new_ret = malloc( sizeof( t_shift_expression));
      switch( tok ){
      case LSHIFT:    new_ret->sop=SOP_LSHIFT;    break;
      case RSHIFT:    new_ret->sop=SOP_RSHIFT;    break;
      default: 
	perror( "unexpcted type of tok" );
	break;
      }
      match (tok);
      new_ret->sh_exp = ret;
      new_ret->ad_exp = additive_expression ();
      ret = new_ret;
    }
  TRACEOUT ("shift_expression");
  return ret;
}

/***************************************************************************/

static t_relational_expression *
relational_expression (void)
{
  t_relational_expression *ret;
  t_relational_expression *new_ret;  
  ret = malloc( sizeof( t_relational_expression));
  ret->rl_exp = NULL;
  ret->rop = ROP_UNDEFINED;
  // relational-expression:
  //   shift-expression
  //   relational-expression <  shift-expression
  //   relational-expression >  shift-expression
  //   relational-expression <= shift-expression
  //   relational-expression <= shift-expression
  TRACEIN ("relational_expression");
  ret->sh_exp = shift_expression ();
  while (tok == GREATERTHAN || tok == LESSTHAN || tok == GTEQ || tok == LESSEQ)
    {
      new_ret = malloc( sizeof( t_relational_expression));
      switch( tok ){
      case GREATERTHAN: new_ret->rop=ROP_GREATERTHAN; break;
      case LESSTHAN:    new_ret->rop=ROP_LESSTHAN;    break;
      case GTEQ:        new_ret->rop=ROP_GTEQ;        break;
      case LESSEQ:      new_ret->rop=ROP_LESSEQ;      break;
      default: 
	perror( "unexpcted type of tok" );
	break;
      }
      match (tok);
      new_ret->rl_exp = ret;
      new_ret->sh_exp = shift_expression ();
      ret = new_ret;
    }
  TRACEOUT ("relational_expression");
  return ret;
}

/***************************************************************************/

static t_equality_expression *
equality_expression (void)
{
  t_equality_expression *ret;
  t_equality_expression *new_ret;  
  ret = malloc( sizeof( t_equality_expression));
  ret->eq_exp = NULL;
  ret->eop = EOP_UNDEFINED;
  // equality-expression:
  //   relational-expression
  //   equality-expression == relational-expression
  //   equality-expression != relational-expression
  TRACEIN ("equality_expression");
  ret->rl_exp = relational_expression ();
  while (tok == EQEQ || tok == NOTEQ)
    {
      new_ret = malloc( sizeof( t_equality_expression));
      switch( tok ){
      case EQEQ:        new_ret->eop=EOP_EQEQ;     break;
      case NOTEQ:       new_ret->eop=EOP_NOTEQ;    break;
      default: 
	perror( "unexpcted type of tok" );
	break;
      }
      match (tok);
      new_ret->eq_exp = ret;
      new_ret->rl_exp = relational_expression ();
      ret = new_ret;
    }
  TRACEOUT ("equality_expression");
  return ret;
}

/***************************************************************************/

static t_and_expression *
and_expression (void)
{
  t_and_expression *ret;
  t_and_expression *new_ret;  
  ret = malloc( sizeof( t_and_expression));
  ret->an_exp = NULL;
  // AND-expression:
  //   equality-expression
  //   AND-expression & equlity-expression
  TRACEIN ("and_expression");
  ret->eq_exp = equality_expression ();
  while (tok == AND)
    {
      new_ret = malloc( sizeof( t_and_expression));
      match (AND);
      new_ret->an_exp = ret;
      new_ret->eq_exp = equality_expression ();
      ret = new_ret;
    }
  TRACEOUT ("and_expression");
  return ret;
}

/***************************************************************************/

static t_exclusive_or_expression *
exclusive_or_expression (void)
{
  t_exclusive_or_expression *ret;
  t_exclusive_or_expression *new_ret;  
  ret = malloc( sizeof( t_exclusive_or_expression));
  ret->xo_exp = NULL;
  // exclusive-OR-expression:
  //   AND-expression
  //   exclusive-OR-expression ^ AND-expression
  TRACEIN ("exclusive_or_expression");
  ret->an_exp = and_expression ();
  while (tok == XOR)
    {
      new_ret = malloc( sizeof( t_exclusive_or_expression));
      match (XOR);
      new_ret->xo_exp = ret;
      new_ret->an_exp = and_expression ();
      ret = new_ret;
    }
  TRACEOUT ("exclusive_or_expression");
  return ret;
}

/***************************************************************************/

static t_inclusive_or_expression *
inclusive_or_expression (void)
{
  t_inclusive_or_expression *ret;
  t_inclusive_or_expression *new_ret;  
  ret = malloc( sizeof( t_inclusive_or_expression));
  ret->or_exp = NULL;
  // inclusive-OR-expression:
  //   exclusive-OR-expression
  //   inclusive-OR-expression | exclusive-OR-expression
  TRACEIN ("inclusive_or_expression");
  ret->xo_exp = exclusive_or_expression ();
  while (tok == OR)
    {
      new_ret = malloc( sizeof( t_inclusive_or_expression));
      match (OR);
      new_ret->or_exp = ret;
      new_ret->xo_exp = exclusive_or_expression ();
      ret = new_ret;
    }
  TRACEOUT ("inclusive_or_expression");
  return ret;
}

/***************************************************************************/

static t_logical_and_expression *
logical_and_expression (void)
{
  t_logical_and_expression *ret;
  t_logical_and_expression *new_ret;  
  ret = malloc( sizeof( t_logical_and_expression));
  ret->la_exp = NULL;
  // logical-AND-expression:
  //   inclusive-OR-expression
  //   logical-AND-expression && inclusive-OR-expression
  TRACEIN ("logical_and_expression");
  ret->or_exp = inclusive_or_expression ();
  while (tok == ANDAND)
    {
      new_ret = malloc( sizeof( t_logical_and_expression));
      match (ANDAND);
      new_ret->la_exp = ret;
      new_ret->or_exp = inclusive_or_expression ();
      ret = new_ret;
    }
  TRACEOUT ("logical_and_expression");
  return ret;
}

/***************************************************************************/

static t_logical_or_expression *
logical_or_expression (void)
{
  t_logical_or_expression *ret;
  t_logical_or_expression *new_ret;  
  ret = malloc( sizeof( t_logical_or_expression));
  ret->lo_exp = NULL;
  // logical-OR-expression:
  //   logical-AND-expression
  //   logical-OR-expression || logical-AND-expression
  TRACEIN ("logical_or_expression");
  ret->la_exp = logical_and_expression ();
  while (tok == OROR)
    {
      new_ret = malloc( sizeof( t_logical_or_expression));
      match (OROR);
      new_ret->lo_exp = ret;
      new_ret->la_exp = logical_and_expression ();
      ret = new_ret;
    }
  TRACEOUT ("logical_or_expression");
  return ret;
}

/***************************************************************************/

static t_conditional_expression *
conditional_expression (void)
{
  t_conditional_expression *ret;
  ret = malloc( sizeof( t_conditional_expression));
  // conditional-expression:
  //   logical-OR-expression
  //   logical-OR-expression ? expression : conditional-expression
  TRACEIN ("conditional_expression");
  ret->lo_exp = logical_or_expression ();
  ret->exp = NULL;   ret->cd_exp = NULL;
  if (tok == QUERY)
    {
      match (QUERY);
      ret->exp = expression ();
      match (COLON);
      ret->cd_exp = conditional_expression ();
    }
  TRACEOUT ("conditional_expression");
  return ret;
}

/***************************************************************************/

static t_assignment_expression *
assignment_expression (void)
{
  t_assignment_expression *ret;
  ret = malloc( sizeof( t_assignment_expression));
  ret->as = AS_UNDEFINED;
  // assignment-expression:
  //   conditional-expression
  //   unary-expression assignment-operator assignment-expression
  TRACEIN ("assignment_expression");
  ret->cd_exp = conditional_expression ();
  ret->as_exp = NULL;
  if (is_assign_operator (tok))
    {
      switch( tok ){
      case EQUALS:          ret->as=AS_EQUALS;          break;
      case PLUS_EQUALS:     ret->as=AS_PLUS_EQUALS;     break;
      case MINUS_EQUALS:    ret->as=AS_MINUS_EQUALS;    break;
      case STAR_EQUALS:     ret->as=AS_STAR_EQUALS;     break;
      case SLASH_EQUALS:    ret->as=AS_SLASH_EQUALS;    break;
      case PERCENT_EQUALS:  ret->as=AS_PERCENT_EQUALS;  break;
      case LSHIFT_EQUALS:   ret->as=AS_LSHIFT_EQUALS;   break;
      case RSHIFT_EQUALS:   ret->as=AS_RSHIFT_EQUALS;   break;
      case AND_EQUALS:      ret->as=AS_AND_EQUALS;      break;
      case XOR_EQUALS:      ret->as=AS_XOR_EQUALS;      break;
      case OR_EQUALS:       ret->as=AS_OR_EQUALS;       break;
      default: 
	perror( "unexpcted type of tok" );
	break;
      }
      /* TODO: check that previous expression was unary */
      match (tok);
      ret->as_exp = assignment_expression ();
    }
  TRACEOUT ("assignment_expression");
  return ret;
}

/***************************************************************************/

static t_labeled_statement *
labeled_statement (void)
{
  t_labeled_statement *ret = malloc( sizeof( t_labeled_statement));
  // labeled-statement:
  //   identifier : statement
  //   case-statement
  //   default-statement
  TRACEIN ("labeled_statement");
  ret->identifier = malloc( strlen( token_str) + 1);
  strcpy( ret->identifier, token_str );
  match (IDENTIFIER);
  match (COLON);
  ret->stat = statement ();
  TRACEOUT ("labeled_statement");
  return ret;
}

static t_case_statement *
case_statement (void)
{
  t_case_statement *ret = malloc( sizeof( t_case_statement));
  // case-statement:
  //   case constant-expression : statement
  TRACEIN ("case_statement");
  match (CASE);
  ret->cs_exp = constant_expression ();
  match (COLON);
  ret->stat = statement ();
  TRACEOUT ("case_statement");
  return ret;
}

static t_default_statement *
default_statement (void)
{
  t_default_statement *ret = malloc( sizeof( t_default_statement));
  // default-statement:
  //   default : statement
  TRACEIN ("default_statement");
  match (DEFAULT);
  match (COLON);
  ret->stat = statement ();
  TRACEOUT ("default_statement");
  return ret;
}

static t_if_statement *
if_statement (void)
{
  t_if_statement *ret = malloc( sizeof( t_if_statement));
  // if-statement:
  //   if ( expression ) statement
  //   if ( expression ) statement else statement
  TRACEIN ("if_statement");
  enter_scope ();
  match (IF);
  match (LPAREN);
  ret->exp = expression ();
  match (RPAREN);
  enter_scope ();
  ret->stat1 = statement ();
  exit_scope ();
  ret->stat2 = NULL;
  if (tok == ELSE)
    {
      enter_scope ();
      match (ELSE);
      ret->stat2 = statement ();
      exit_scope ();
    }
  exit_scope ();
  TRACEOUT ("if_statement");
  return ret;
}

static t_switch_statement *
switch_statement (void)
{
  t_switch_statement *ret = malloc( sizeof( t_switch_statement));
  // switch-statement:
  //   switch ( expression ) statement
  TRACEIN ("switch_statement");
  enter_scope ();
  match (SWITCH);
  match (LPAREN);
  ret->exp = expression ();
  match (RPAREN);
  enter_scope ();
  ret->stat = statement ();
  exit_scope ();
  exit_scope ();
  TRACEOUT ("switch_statement");
  return ret;
}

static t_while_statement *
while_statement (void)
{
  t_while_statement *ret = malloc( sizeof ( t_while_statement));
  // while-statement:
  //   while ( expression ) statement
  TRACEIN ("while_statement");
  enter_scope ();
  match (WHILE);
  match (LPAREN);
  ret->exp = expression ();
  match (RPAREN);
  enter_scope ();
  ret->stat = statement ();
  exit_scope ();
  exit_scope ();
  TRACEOUT ("while_statement");
  return ret;
}

static t_do_while_statement *
do_while_statement (void)
{
  t_do_while_statement *ret = malloc( sizeof ( t_do_while_statement));
  // do-while-statement:
  //   do statement while ( expression ) ;
  TRACEIN ("do_while_statement");
  enter_scope ();
  match (DO);
  enter_scope ();
  ret->stat = statement ();
  exit_scope ();
  match (WHILE);
  match (LPAREN);
  ret->exp = expression ();
  match (RPAREN);
  exit_scope ();
  match (SEMI);
  TRACEOUT ("do_while_statement");
  return ret;
}

static t_for_statement *
for_statement (void)
{
  t_for_statement *ret = malloc( sizeof( t_for_statement));
  ret->spec = FOR_UNDEFINED;
  ret->exp1 = ret->exp2 = ret->exp3 = NULL;
  ret->decl = NULL;
  // for-statement:
  //   for ( expression(opt) ; expression(opt) ; expression(opt) ) statement
  //   for ( declaration expression(opt) ; expression(opt) ) statement 
  TRACEIN ("for_statement");
  enter_scope ();
  match (FOR);
  match (LPAREN);
  if (tok != SEMI)
    {
      if (is_declaration (tok))
	{
	  ret->spec = FOR_DECL;
	  ret->decl = declaration ();
	}
      else
	{
	  ret->spec = FOR_EXP;
	  ret->exp1 = expression ();
	  match (SEMI);
	}
    }
  else
    {
      ret->spec = FOR_EXP;
      match (SEMI);
    }
  if (tok != SEMI)
    ret->exp2 = expression ();
  match (SEMI);
  if (tok != RPAREN)
    ret->exp3 = expression ();
  match (RPAREN);
  enter_scope ();
  ret->stat = statement ();
  exit_scope ();
  exit_scope ();
  TRACEOUT ("for_statement");
  return ret;
}

static t_break_statement *
break_statement (void)
{
  t_break_statement *ret = malloc( sizeof( t_break_statement));
  // break-statement:
  //   break ;
  TRACEIN ("break_statement");
  match (BREAK);
  match (SEMI);
  TRACEOUT ("break_statement");
  return ret;
}

static t_continue_statement *
continue_statement (void)
{
  t_continue_statement *ret = malloc( sizeof( t_continue_statement));
  // continue-statement:
  //   continue ;
  TRACEIN ("continue_statement");
  match (CONTINUE);
  match (SEMI);
  TRACEOUT ("continue_statement");
  return ret;
}

static t_goto_statement * 
goto_statement (void)
{
  t_goto_statement *ret = malloc( sizeof( t_goto_statement));
  // goto-statement:
  //   goto identifier ;
  TRACEIN ("goto_statement");
  match (GOTO);
  ret->identifier = malloc( strlen( token_str) + 1);
  strcpy( ret->identifier, token_str);
  match (IDENTIFIER);
  match (SEMI);
  TRACEOUT ("goto_statement");
  return ret;
}

static t_return_statement *
return_statement (void)
{
  t_return_statement *ret = malloc( sizeof( t_return_statement));
  ret->exp = NULL;
  // return-statement:
  //   return expression(opt) ;
  TRACEIN ("return_statement");
  match (RETURN);
  if (tok != SEMI)
    ret->exp = expression ();
  match (SEMI);
  TRACEOUT ("return_statement");
  return ret;
}

static t_empty_statement *
empty_statement (void)
{
  t_empty_statement *ret = malloc( sizeof( t_empty_statement));
  // empty-statement:
  //   ;
  TRACEIN ("empty_statement");
  match (SEMI);
  TRACEOUT ("empty_statement");
  return ret;
}

static t_expression_statement *
expression_statement (void)
{
  t_expression_statement *ret = malloc( sizeof( t_expression_statement)); 
  ret->lb_stat = NULL;
  ret->exp = NULL;
  // expression-statement:
  //   expression ;
  TRACEIN ("expression_statement");

  if (tok == IDENTIFIER && lex_colon_follows ())
    {
      ret->lb_stat = labeled_statement ();
    }
  else
    {
      ret->exp = expression ();
      match (SEMI);
    }
  TRACEOUT ("expression_statement");
  return ret;
}

static t_statement *
statement (void)
{
  t_statement *ret = malloc( sizeof( t_statement));
  ret->spec = ST_UNDEFINED;
  ret->ex_stat = NULL;  ret->cs_stat = NULL;  ret->df_stat = NULL;
  ret->if_stat = NULL;  ret->sw_stat = NULL;  ret->wh_stat = NULL;
  ret->dw_stat = NULL;  ret->fo_stat = NULL;  ret->br_stat = NULL;
  ret->ct_stat = NULL;  ret->gt_stat = NULL;  ret->rt_stat = NULL;
  ret->cp_stat = NULL;  ret->em_stat = NULL;
  // statement:
  //   labeled-statement
  //   compound-statement
  //   expression-statement
  //   selection-statement
  //   iteration-statement
  //   jump-statement
  TRACEIN ("statement");
  switch (tok)
    {
    case IDENTIFIER:
      ret->spec = ST_EXP;
      ret->ex_stat = expression_statement ();
      break;
    case CASE:
      ret->spec = ST_CASE;
      ret->cs_stat = case_statement ();
      break;
    case DEFAULT:
      ret->spec = ST_DEFAULT;
      ret->df_stat = default_statement ();
      break;
    case IF:
      ret->spec = ST_IF;
      ret->if_stat = if_statement ();
      break;
    case SWITCH:
      ret->spec = ST_SWITCH;
      ret->sw_stat = switch_statement ();
      break;
    case WHILE:
      ret->spec = ST_WHILE;
      ret->wh_stat = while_statement ();
      break;
    case DO:
      ret->spec = ST_DO_WHILE;
      ret->dw_stat = do_while_statement ();
      break;
    case FOR:
      ret->spec = ST_FOR;
      ret->fo_stat = for_statement ();
      break;
    case BREAK:
      ret->spec = ST_BREAK;
      ret->br_stat = break_statement ();
      break;
    case CONTINUE:
      ret->spec = ST_CONTINUE;
      ret->ct_stat = continue_statement ();
      break;
    case GOTO:
      ret->spec = ST_GOTO;
      ret->gt_stat = goto_statement ();
      break;
    case RETURN:
      ret->spec = ST_RETURN;
      ret->rt_stat = return_statement ();
      break;
    case LBRACE:
      ret->spec = ST_COMPOUND;
      ret->cp_stat = compound_statement ();
      break;
    case SEMI:
      ret->spec = ST_EMPTY;
      ret->em_stat = empty_statement ();
      break;
    default:
      if (is_expression (tok)){
	ret->spec = ST_EXP;
	ret->ex_stat = expression_statement ();
      }
      break;
    }
  if( ret->spec == ST_UNDEFINED ){
    free( ret );
    ret = NULL;
  }
  TRACEOUT ("statement");
  return ret;
}

static t_compound_statement *
compound_statement (void)
{
  t_compound_statement *ret = malloc( sizeof( t_compound_statement));
  block_item_list **curbi = &(ret->bi_list);
  ret->bi_list = NULL;
  // compound-statement:
  //   { block-item-list(opt) }
  // block-item-list:
  //   block-item
  //   block-item-list block-item
  // block-item:
  //   statement
  //   declaration
  TRACEIN ("compound_statement");
  enter_scope ();
  match (LBRACE);

  while (tok != RBRACE)
    {
      *curbi = malloc( sizeof ( block_item_list));
      (*curbi)->spec = BI_UNDEFINED;
      (*curbi)->stat = NULL;  (*curbi)->decl = NULL;      
      (*curbi)->next = NULL;
      if (is_declaration (tok))
	{
	  if (tok == IDENTIFIER && lex_colon_follows ()){
	    (*curbi)->spec = BI_STAT;
	    (*curbi)->stat = statement ();
	  }
	  else {
	    (*curbi)->spec = BI_DECL;
	    (*curbi)->decl = declaration ();
	  }
	}
      else
	{
	  (*curbi)->spec = BI_STAT;
	  (*curbi)->stat = statement ();
	}
      curbi = &((*curbi)->next);
    }
  exit_scope ();
  match (RBRACE);
  TRACEOUT ("compound_statement");
  return ret;
}

/***************************************************************************/

static t_enumerator *
enumerator (void)
{
  t_enumerator *ret = malloc( sizeof( t_enumerator));
  ret->identifier = NULL;
  ret->cs_exp = NULL;
  // enumerator:
  //   enumeration-constant
  //   enumeration-constant = constant-expression
  // enumeration-constant:
  //   identifier
  TRACEIN ("enumerator");
  if (tok == IDENTIFIER)
    {
      check_not_typedef ();
      install_symbol (Lexeme->identifier->id_name,
		      Storage_class[stack_ptr], OBJ_ENUMERATOR);
      ret->identifier = malloc( strlen( token_str) + 1);
      strcpy( ret->identifier, token_str);
      match (IDENTIFIER);
    }
  else
    {
      free( ret );
      TRACEOUT ("enumerator");
      return NULL;
    }
  if (tok == EQUALS)
    {
      match (EQUALS);
      ret->cs_exp = constant_expression ();
    }
  TRACEOUT ("enumerator");
  return ret;
}

static t_enum_specifier *
enum_specifier (void)
{
  t_enum_specifier *ret = malloc( sizeof( t_enum_specifier));
  enumerator_list **encur = &(ret->en_list);
  ret->identifier = NULL;
  ret->en_list = NULL;
  // enum-specifier:
  //   enum identifier(opt) { enumerator-list }
  //   enum identifier(opt) { enumerator-list , }
  //   enum identifier
  // enumerator-list:
  //   enumerator
  //   enumerator-list , enumerator
  TRACEIN ("enum_specifier");
  if (tok == ENUM)
    {
      match (ENUM);
    }
  else
    {
      TRACEOUT ("enum_specifier");
      free( ret );
      return NULL;
    }

  if (tok == IDENTIFIER)
    {
      ret->identifier = malloc( strlen( token_str) +1);
      strcpy( ret->identifier, token_str);
      match (IDENTIFIER);
    }
  if (tok == LBRACE)
    {
      *encur = malloc( sizeof( enumerator_list));
      match (LBRACE);
      (*encur)->next = NULL;
      (*encur)->en = enumerator ();
      encur = &((*encur)->next);
      while (tok == COMMA)
	{
	  *encur = malloc( sizeof( enumerator_list));
	  match (COMMA);
	  (*encur)->next = NULL;
	  (*encur)->en = enumerator ();
	  encur = &((*encur)->next);
	}
      match (RBRACE);
    }
  TRACEOUT ("enum_specifier");
  return ret;
}

static t_member *
member (void)
{
  t_member *ret = malloc( sizeof( t_member));
  ret->dclr = NULL;  ret->cs_exp = NULL;
  // struct-declarator:
  //   declarator
  //   declarator(opt) : constant-expression
  TRACEIN ("member");
  if (tok != COLON)
    ret->dclr = declarator (0);
  if (tok == COLON)
    {
      match (COLON);
      ret->cs_exp = constant_expression ();
    }
  TRACEOUT ("member");
  return ret;
}

static t_members *
members (void)
{
  t_members *ret = malloc( sizeof( t_members));
  struct_declaration_list **cursd = &(ret->sdec_list);
  ret->sdec_list = NULL;
  // struct-declaration-list:
  //   struct-declaration
  //   struct-declaration-list struct-declaration
  // struct-declaration:
  //   specifier-qualifier-list struct-declarator-list;
  // struct-declarator-list:
  //   struct-declarator
  //   struct-declarator-list , struct-declarator
  TRACEIN ("members");
  do
    {
      member_list **curmem;
      *cursd = malloc( sizeof( struct_declaration_list));
      (*cursd)->next = NULL;
      stack_ptr++;
      (*cursd)->dspec = declaration_specifiers (1);
      (*cursd)->mem_list = malloc( sizeof( member_list));
      (*cursd)->mem_list->mem = member ();
      (*cursd)->mem_list->next = NULL;
      curmem = &((*cursd)->mem_list->next);
      while (tok == COMMA)
	{
	  (*curmem) = malloc( sizeof( t_member));
	  (*curmem)->next = NULL;
	  match (COMMA);
	  (*curmem)->mem = member ();
	  curmem = &((*curmem)->next);
	}
      match (SEMI);
      stack_ptr--;
      cursd = &((*cursd)->next);
    }
  while (tok != RBRACE);
  TRACEOUT ("members");
  return ret;
}

static t_struct_or_union_specifier *
struct_or_union_specifier (void)
{
  t_struct_or_union_specifier *ret;
  ret = malloc( sizeof( t_struct_or_union_specifier));
  ret->spec = SU_UNDEFINED;
  ret->identifier = NULL;
  ret->mems = NULL;
  // struct-or-union-specifier:
  //   struct-or-union identifier(opt) { struct-declaration-list }
  //   struct-or-union identifier
  TRACEIN ("struct_or_union_specifier");
  Parsing_struct++;

  switch( tok ){
  case STRUCT:  ret->spec = SU_STRUCT;  break;
  case UNION:   ret->spec = SU_UNION;   break;
  default: break;
  }

  match (tok);
  if (tok == IDENTIFIER){
    ret->identifier = malloc( strlen( token_str) + 1);
    strcpy( ret->identifier, token_str);
    match (IDENTIFIER);
  }
  if (tok == LBRACE)
    {
      match (LBRACE);
      ret->mems = members ();
      match (RBRACE);
    }
  Parsing_struct--;
  TRACEOUT ("struct_or_union_specifier");
  return ret;
}

static t_type_name *
type_name (void)
{
  t_type_name *ret = malloc( sizeof( t_type_name));
  // type-name:
  //   specifier-qualifier-list abstract-declarator(opt)
  TRACEIN ("type_name");
  stack_ptr++;
  ret->dspec = declaration_specifiers (1);
  ret->dclr = declarator (1);
  stack_ptr--;
  TRACEOUT ("type_name");
  return ret;
}

static t_declaration_specifiers *
declaration_specifiers (int no_storage_class)
{
  t_declaration_specifiers *ret = malloc( sizeof( t_declaration_specifiers));
  bool type_found = FALSE;
  int qf_index=0;
  ret->spec = TP_UNDEFINED;      ret->st_class = STC_UNDEFINED;
  ret->qf[0] = QF_END;
  ret->et_sign = SG_UNDEFINED;   ret->et_type = TP_UNDEFINED;
  ret->et_long = LG_UNDEFINED;   ret->suspec = NULL;
  ret->espec = NULL;             ret->identifier = NULL;
  // declaration-specifiers:
  //   storage-class-specifier declaration-specifiers(opt)
  //   type-specifier declaration-specifiers(opt)
  //   type-qualifier declaration-specifiers(opt)
  //   function-specifier declaration-specifiers(opt)
  // specifier-qualifier-list:   <-- from struct-declaration, type-name
  //   type-specifier specifier-qualifier-list(opt)
  //   type-qualifier specifier-qualifier-list(opt)

  TRACEIN ("declaration_specifiers");
  assert (stack_ptr >= 0 && stack_ptr < 100);
  Storage_class[stack_ptr] = 0;
  while (is_declaration (tok))
    {
      if (no_storage_class && (TokMap[tok] & TOK_STORAGE_CLASS))
	{
	  fprintf (stderr, "Parse failed: unexpected storage class %s\n",
		   tokname (tok));
	  exit (1);
	}
      if (tok == IDENTIFIER && type_found)
	break;
      if ((TokMap[tok] & TOK_TYPE_SPECIFIER) || tok == IDENTIFIER)
	{
	  type_found = TRUE;
	}
      if (TokMap[tok] & TOK_STRUCT)
	{
	  ret->spec = TP_STRUCT;
	  ret->suspec = struct_or_union_specifier ();
	  break;
	}
      else if (tok == ENUM)
	{
	  ret->spec = TP_ENUM;
	  ret->espec = enum_specifier ();
	  break;
	}
      else
	{
	  bool savedtok = 0;
	  
	  if (TokMap[tok] & TOK_STORAGE_CLASS)
	    {
	      switch( tok ){
	      case TYPEDEF:  ret->st_class = STC_TYPEDEF;  break;
	      case EXTERN:   ret->st_class = STC_EXTERN;   break;
	      case STATIC:   ret->st_class = STC_STATIC;   break;
	      case AUTO:     ret->st_class = STC_AUTO;     break;
	      case REGISTER: ret->st_class = STC_REGISTER; break;
	      default: 
		perror( "unexpcted type of tok" );
		break;
	      }
	      Storage_class[stack_ptr] = tok;
	    }
	  else if (tok == IDENTIFIER)
	    {
	      ret->spec = TP_NAME;
	      ret->identifier = malloc( strlen( token_str) + 1);
	      strcpy( ret->identifier, token_str);
	      savedtok = tok;
	    }
	  else {
	    ret->spec = TP_ELEMENTAL;
	    switch( tok ){
	    case VOID:     ret->et_type = ET_VOID;     break;
	    case CHAR:     ret->et_type = ET_CHAR;     break;
	    case SHORT:    ret->et_type = ET_SHORT;    break;
	    case INT:      ret->et_type = ET_INT;      break;
	    case FLOAT:    ret->et_type = ET_FLOAT;    break;
	    case DOUBLE:   ret->et_type = ET_DOUBLE;   break;
	    case SIGNED:   ret->et_sign = SG_SIGNED;   break;
	    case UNSIGNED: ret->et_sign = SG_UNSIGNED; break;
	    case LONG:
	      ret->et_long = 
		(ret->et_long==LG_UNDEFINED) ? LG_LONG : LG_LONGLONG;
	      break;
	    case CONST:      ret->qf[qf_index++] = QF_CONST;    break;
	    case RESTRICT:   ret->qf[qf_index++] = QF_RESTRICT; break;
	    case VOLATILE:   ret->qf[qf_index++] = QF_VOLATILE; break;
	    default: 
	      perror( "unexpcted type of tok" );
	      break;
	    }
	  }
	  match (tok);
	  if (savedtok == IDENTIFIER)
	    break;
	}
    }
  ret->qf[qf_index] = QF_END;
  TRACEOUT ("declaration_specifiers");
  return ret;
}

static t_pointer *
pointer (void)
{
  t_pointer *ret = malloc( sizeof( t_pointer));
  int i=0;
  // pointer:
  //   * type-qualifier-list(opt)
  //   * type-qualifier-list(opt) pointer
  TRACEIN ("pointer");
  while (tok == STAR)
    {
      ret->pt_qf[i++] = PT_STAR;
      match (STAR);
      while (TokMap[tok] & TOK_TYPE_QUALIFIER)
	{
	  switch( tok ){
	  case CONST:    ret->pt_qf[i++]=PT_CONST;    break;
	  case RESTRICT: ret->pt_qf[i++]=PT_RESTRICT; break;
	  case VOLATILE: ret->pt_qf[i++]=PT_VOLATILE; break;
	  default: 
	    perror( "unexpcted type of tok" );
	    break;
	  }
	  match (tok);
	}
    }
  ret->pt_qf[i] = PT_END;
  TRACEOUT ("pointer");
  return ret;
}

static t_direct_declarator *
direct_declarator (int abstract)
{
  t_direct_declarator *ret = malloc( sizeof( t_direct_declarator));
  ret->spec = DD_UNDEFINED;
  ret->dclr = NULL;
  ret->identifier = NULL;
  ////  See also suffix_declarator 
  // direct-declarator:
  //   identifier
  //   ( declarator )
  //   direct-declarator[ type-qualifier-list(opt) assignment-expression(opt) ]
  //   direct-declarator[static type-qualifier-list(opt) assignment-expression]
  //   direct-declarator[type-qualifier-list static assignment-expression ]
  //   direct-declarator[type-qualifier-list(opt) *]
  //   direct-declarator( parameter-type-list )
  //   direct-declarator[ identifier-list(opt) ]
  // direct-abstract-declarator:
  //   ( abstract-declarator )
  //   direct-abstract-declarator(opt) [ assignment-expression(opt) ]
  //   direct-abstract-declarator(opt) [ * ]
  //   direct-abstract-declarator(opt) ( parameter-type-list(opt) )
  TRACEIN ("direct_declarator");
  if (tok == LPAREN)
    {
      ret->spec = DD_PAREN;
      match (LPAREN);
      ret->dclr = declarator (abstract);
      match (RPAREN);
    }
  else
    {
      if (!abstract)
	{
	  if (tok == IDENTIFIER)
	    {
	      ret->spec = DD_IDENTIFIER;
	      ret->identifier = malloc( strlen( token_str) + 1);
	      strcpy( ret->identifier, token_str);	     	      
	      Saw_ident = 1;
	      if (Storage_class[stack_ptr] == TYPEDEF)
		{
		  install_symbol (Lexeme->identifier->id_name,
				  TYPEDEF, OBJ_TYPEDEF_NAME);
		}
	      else if (!Parsing_struct && !Parsing_oldstyle_parmdecl)
		{
		  install_symbol (Lexeme->identifier->id_name,
				  Storage_class[stack_ptr], OBJ_IDENTIFIER);
		}
	      match (IDENTIFIER);
	    }
	}
      else
	ret->spec = DD_NULL;
    }
  TRACEOUT ("direct_declarator");
  return ret;
}

static t_parameter_list *
parameter_list (int *new_style)
{
  t_parameter_list *ret = malloc( sizeof( t_parameter_list));
  param_list *lastparam = NULL;
  ret->new_style = 0;
  ret->pm_list = NULL;
  ret->va_arg = 0;
  // parameter-list:
  //   parameter-declaration
  //   parameter-list , parameter-declaration
  // parameter-declaration
  //   declaration-specifiers declarator
  //   declaration-specifiers abstract-declarator(opt)
  TRACEIN ("parameter_list");
  if (tok == IDENTIFIER
      && (Cursym == 0 || Cursym->object_type != OBJ_TYPEDEF_NAME))
    {
      ret->new_style = *new_style = 0;
      lastparam = ret->pm_list = malloc( sizeof( param_list));
      lastparam->identifier = malloc( strlen( token_str) + 1);
      strcpy( lastparam->identifier, token_str);
      lastparam->dspec = NULL;  lastparam->dclr = NULL;
      lastparam->next = NULL;
      install_symbol (Lexeme->identifier->id_name, AUTO, OBJ_PARAMETER);
      match (IDENTIFIER);
      while (tok == COMMA)
	{
	  lastparam = lastparam->next = malloc( sizeof( param_list));
	  match (COMMA);
	  if (tok == IDENTIFIER)
	    {
	      check_not_typedef ();
	      lastparam->identifier = malloc( strlen( token_str) + 1);
	      strcpy( lastparam->identifier, token_str);
	      lastparam->dspec = NULL;  lastparam->dclr = NULL;
	      lastparam->next = NULL;
	      install_symbol(Lexeme->identifier->id_name, AUTO, OBJ_PARAMETER);
	      match (tok);
	    }
	  else
	    match (IDENTIFIER);
	}
    }
  else
    {

      /*
       * CHECK: When defining a function, each declarator in a
       * parameter list must contain an identifier.
       */

      ret->new_style = *new_style = 1;
      stack_ptr++;
      lastparam = ret->pm_list = malloc( sizeof( param_list));
      lastparam->identifier = NULL;
      lastparam->dspec = declaration_specifiers (0);
      lastparam->dclr = declarator (0);
      lastparam->next = NULL;
      stack_ptr--;
      while (tok == COMMA)
	{
	  match (COMMA);
	  if (tok == ELLIPSIS)
	    {
	      ret->va_arg = 1;
	      match (ELLIPSIS);
	      break;
	    }
	  stack_ptr++;
	  lastparam = lastparam->next = malloc( sizeof( param_list));
	  lastparam->identifier = NULL;
	  lastparam->dspec = declaration_specifiers (0);
	  lastparam->dclr = declarator (0);
	  lastparam->next = NULL;
	  stack_ptr--;
	}
    }
  TRACEOUT ("parameter_list");
  return ret;
}

static t_suffix_declarator *
suffix_declarator (void)
{
  t_suffix_declarator *ret = malloc( sizeof( t_suffix_declarator));
  ret->spec = SD_UNDEFINED;
  ret->cs_exp = NULL;  ret->param_list = NULL;
  ret->is_func = 0;
  // suffix of (abstract-)direct-declarator
  TRACEIN ("suffix_declarator");
  if (tok == LBRAC)
    {
      ret->spec = SD_ARRAY;
      match (LBRAC);
      ret->cs_exp = constant_expression ();
      match (RBRAC);
    }
  else if (tok == LPAREN)
    {
      int new_style = 0;

      ret->spec = SD_FUNC;
      enter_scope ();
      match (LPAREN);
      ret->param_list = parameter_list (&new_style);
      match (RPAREN);
      if (new_style && tok != LBRACE)
	exit_scope ();
      ret->is_func = Is_func = 1;
    }
  TRACEOUT ("suffix_declarator");
  return ret;
}


static t_declarator *
declarator (int abstract)
{
  t_declarator *ret = malloc( sizeof( t_declarator));
  suffix_declarator_list **cur_sdl = &(ret->sd_list);
  ret->pt = NULL;   ret->ddclr = NULL;   ret->sd_list = NULL;
  // declarator:
  //   pointer(opt) direct-declarator
  // abstract-declarator:
  //   pointer
  //   pointer(opt) direct-abstract-declator
  TRACEIN ("declarator");
  if (tok == STAR)
    {
      ret->pt = pointer ();
    }
  ret->ddclr = direct_declarator (abstract);
  while (tok == LBRAC || tok == LPAREN)
    {
      (*cur_sdl) = malloc( sizeof( suffix_declarator_list));
      (*cur_sdl)->sf_dclr = suffix_declarator ();
      (*cur_sdl)->next = NULL;
      cur_sdl = &((*cur_sdl)->next);
    }
  TRACEOUT ("declarator");
  return ret;
}

static t_designator *
designator (void)
{
  t_designator *ret = malloc( sizeof( t_designator));
  ret->spec = DSG_UNDEFINED;
  ret->cs_exp = NULL;  ret->identifier = NULL;
  // designator:
  //   [ constant-expression ]
  //   . identifier
  TRACEIN ("designator");
  if (tok == LBRAC)
    {
      ret->spec = DSG_AREF;
      match (LBRAC);
      ret->cs_exp = constant_expression ();
      match (RBRAC);
    }
  else if (tok == DOT)
    {
      ret->spec = DSG_STRUCT;
      match (DOT);
      if (tok == IDENTIFIER)
	{
	  ret->identifier = malloc( strlen( token_str) + 1);
	  strcpy( ret->identifier, token_str);	  
	  check_not_typedef ();
	  match (tok);
	}
    }
  TRACEOUT ("designator");
  return ret;
}


static t_initializer *
initializer (int recurse)
{
  t_initializer *ret = malloc( sizeof( t_initializer));
  initializer_list *last_init;
  ret->spec = INIT_UNDEFINED;
  ret->init_list = NULL;
  ret->dsg = NULL;        ret->init = NULL;
  ret->as_exp = NULL;
  // initializer-list:
  //   desigonation(opt) initializer
  //   initializer-list , desigonation(opt) initializer
  // desigonation:
  //   designator-list =
  // designator-list:
  //   designator
  //   designator-list designator
  // initializer:
  //   assignment-expression
  //   { initializer-list }
  //   { initializer-list , }
  TRACEIN ("initializer");
  if (tok == LBRACE)
    {
      ret->spec = INIT_LIST;
      match (LBRACE);
      last_init = ret->init_list = malloc( sizeof( initializer_list));
      last_init->init = initializer (recurse + 1);
      last_init->next = NULL;
      while (tok == COMMA)
	{
	  last_init = last_init->next = malloc( sizeof( initializer_list));
	  match (COMMA);
	  last_init->init = initializer (recurse + 1);
	  last_init->next = NULL;
	}
      match (RBRACE);
    }
  else if (recurse && (tok == LBRAC || tok == DOT))
    {
      ret->spec = INIT_LISTELM;
      // designator(opt) initlalizer
      while (tok == LBRAC || tok == DOT)
	{
	  ret->dsg = designator ();
	}
      match (EQUALS);
      ret->init = initializer (0);
    }
  else
    {
      ret->spec = INIT_ASEXP;
      ret->as_exp = assignment_expression ();
    }
  TRACEOUT ("initializer");
  return ret;
}

static t_function_definition *
function_definition (void)
{
  t_function_definition *ret = malloc( sizeof( t_function_definition));
  declaration_list **cur_decl = &(ret->decl_list);
  ret->cp_stat = NULL;
  ret->decl_list = NULL;
  // function-definition:
  //  declaration-specifiers declartor declaration-list(opt) compound-statement
  //  <---     (declaration)      ---> ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  TRACEIN ("function_definition");

  if (tok == LBRACE)
    {
      ret->cp_stat = compound_statement ();
    }
  else
    {
      Parsing_oldstyle_parmdecl++;
      while (is_declaration (tok))
	{
	  /*
	   * CHECK: The only storage class permitted is
	   * register and initialization is not permitted.
	   * if no declaration is given for a parameter,
	   * its type is taken to be int.
	   */

	  (*cur_decl) = malloc( sizeof( declaration_list));
	  (*cur_decl)->decl = declaration ();
	  (*cur_decl)->next = NULL;
	  cur_decl = &((*cur_decl)->next);
	}
      Parsing_oldstyle_parmdecl--;
      ret->cp_stat = compound_statement ();
    }
  exit_scope ();
  TRACEOUT ("function_definition");
  return ret;
}

static t_init_declarator * 
init_declarator (int check_if_function)
{
  t_init_declarator *ret = malloc( sizeof( t_init_declarator));
  int old_Is_func, old_Saw_ident;
  int func_defn = 0;
  ret->is_fn_def = 0;
  ret->dclr = NULL;
  ret->fn_def = NULL;
  ret->init = NULL;
  // init-declarator-list:
  //   init-declarator
  //   init-declarator-list , init-declarator
  // init-declarator:
  //   declarator
  //   declarator = initializer

  TRACEIN ("init_declarator");

  old_Saw_ident = Saw_ident;
  old_Is_func = Is_func;

  Saw_ident = 0;
  Is_func = 0;
  ret->dclr = declarator (0);

  func_defn = check_if_function &&
      Level == LEVEL_FUNCTION && Is_func && Saw_ident && is_function_body (tok);

  if (Is_func)
    {
      /*
       * CHECK: The only storage class specifiers allowed among the
       * declaration specifiers are extern or static.
       */

      /*
       * CHECK: A function may not return a function or an array.
       */
    }

  Is_func = old_Is_func;
  Saw_ident = old_Saw_ident;
  if (func_defn)
    {
      ret->is_fn_def = 1;
      ret->fn_def = function_definition ();
      TRACEOUT ("init_declarator");
      return ret;
    }
  else
    {
      ret->is_fn_def = 0;
      if (tok == EQUALS)
	{
	  /*
	   * CHECK: not allowed when parsing old style function parameters
	   * or a prototype.
	   */
	  match (EQUALS);
	  ret->init = initializer (0);
	}
    }
  TRACEOUT ("init_declarator");
  return ret;
}

/***************************************************************************/

/*
 * 3) a declaration must have at least one declarator, or its type specifier
 * must declare a structure tag, a union tag, or the members of an enumeration.
 * 4) empty declarations are not permitted.
 */

void prt_declaration( t_declaration *decl )
{
  t_declaration_specifiers *dspec = decl->dspec;
  init_declarator_list *idclr_list = decl->idclr_list;
  if( dspec->spec ){
  case TP_UNDEFINED:
    perror( "illegal declaration." );
    return;
  case TP_STRUCT:
    if( dspec->suspec->mems ){
      fprintf( "(def " );
      prt_enum_specifier( dspec->espec );
      fprintf( ")\n");
      free_members( dspec->suspec->mems );
      dspec->suspec->mems = NULL;
    }
    break;
  case TP_ENUM:
    if( dspec->espec->en_list ){
      fprintf( "(def " );
      prt_struct_or_union_specifier( dspec->suspec );
      fprintf( ")\n");
      free_enumerator_list( dspec->espec->en_list );
    }
    break;
  default:
    ;
  }
  if( idclr_list && idclr_list->next ){
      // separate into multiple "def"s
      init_declarator_list *next = idclr_list->next;
      do{
	idclr_list->next = NULL;
	prt_declaration( decl );
	free_init_declarator_list( idclr_list );
	decl->idclr_list = next;
      } while( next );
      return;
    }
  }

  switch( dspec->spec ){
  case TP_ENUM:
  case TP_STRUCT:
    break;
  else if( 

  if( idclr_list && idclr_list->idclr->is_fn_def ){
    // function definition
    return;
  }


void free_init_declarator_list( init_declarator_list *idclr_list )
{
  init_declarator_list *cur=idclr_list, *nxt;
  while( cur ){
    nxt = cur->next;
    free_init_declarator( cur->decl );
    free( cur );
    cur = next;
  }
}

void free_declaration( t_declaration *decl )
{
  if( decl ){
    free_init_declation_specifiers( decl->dspec );
    free_init_declarator_list( decl->idclr_list );
    free( decl );
  }
}

static t_declaration *
declaration (void)
{
  t_declaration *ret = malloc( sizeof( t_declaration));
  t_init_declarator *tmp_idclr; 
  init_declarator_list *last_idclr;
  ret->is_fn_def = 0;
  ret->dspec = NULL;
  ret->fn_def = NULL;
  ret->idclr_list = NULL;
  // function-definition:
  //   declaration-specifiers declarator declaration-list(opt) compound-statement
  // declaration:
  //   declaration-specifiers init-declarator-list(opt) ;
  TRACEIN ("declaration");

  stack_ptr++;
  ret->dspec = declaration_specifiers (0);
  if (tok == SEMI)
    {
      match (SEMI);
      goto success;
    }
  /* 2) the first declarator at global level may start a function
   * definition.
   */
  if ( (tmp_idclr = init_declarator (Level == LEVEL_GLOBAL))
       -> is_fn_def == 1)
    {
      ret->idclr_list = malloc( sizeof( init_declarator_list));
      ret->idclr_list->idclr = tmp_idclr;
      ret->idclr_list->next = NULL;
      goto success;
    }
  last_idclr = ret->idclr_list = malloc( sizeof( init_declarator_list));
  last_idclr->idclr = tmp_idclr;
  last_idclr->next = NULL;
  while (tok == COMMA)
    {
      last_idclr = last_idclr->next = malloc( sizeof( init_declarator_list));
      match (COMMA);
      last_idclr->idclr = init_declarator (0);
      last_idclr->next = NULL;
    }
  match (SEMI);
success:
  stack_ptr--;
  TRACEOUT ("declaration");
  return ret;
}


/***************************************************************************/

/*
 * 1) translation unit consists of a sequence of external declarations.
 * which are either declarations or function definitions.
 * 2) only at this level can functions be defined.
 */

static void prt_translation_unit( t_translation_unit *t_unit ){
  declaration_list *cur;
  for( cur=t_unit->decl_list ; cur ; cur=cur->next ){
    prt_declaration( cur->decl );
  }
}

static void free_declaration_list( declaration_list *dlist ){
  declaration_list *cur=dlist, *nxt;
  while( cur ){
    nxt = cur->next;
    free_declaration( cur->decl );
    free( cur );
    cur = next;
  }
}

static void free_translation_unit( t_translation_unit *t_unit )
{
  if( t_unit ){
    free_declaration_list( ret->decl_list );
    free( t_unit );
  }
}

static t_translation_unit *
translation_unit (void)
{
  t_translation_unit *ret = malloc( sizeof( t_translation_unit));
  declaration_list **cur_decl = &(ret->decl_list);
  ret->decl_list = NULL;
  // translation-unit:
  //   external-declaration
  //   translation-unit external-declaration
  // 
  // external-decalration:
  //   function-definition
  //   declaration
  TRACEIN ("translation_unit");
  Level = LEVEL_GLOBAL;
  tok = lex_get_token ();
  while (tok != 0)
    {

      if (is_external_declaration (tok))
	{
	  /* 
	   * a function definition looks like a declaration,
	   * hence is initially parsed as one.
	   * the check for 2) is in init_declarator().
	   */
	  (*cur_decl) = malloc( sizeof( declaration_list));
	  (*cur_decl)->decl = declaration ();
	  (*cur_decl)->next = NULL;
	  cur_decl = &((*cur_decl)->next);
	}
      else if (tok == SEMI)
	{
	  /*ERROR(4): empty declarations are not permitted */
	  match (tok);
	}
      else
	{
	  fprintf (stderr, "Parse failed: unexpected input %s\n",
		   tokname (tok));
	  exit (1);
	}
      assert (Level == LEVEL_GLOBAL);
    }
  TRACEOUT ("translation_unit");
  return ret;
}

token_t
name_type (const char *name)
{
  Cursym = find_symbol (Cursymtab, name, 1);
  return IDENTIFIER;
}

static const char *
mygetline (char *arg)
{
  static char line[512];

  line[0] = 0;

  return fgets (line, sizeof line, (FILE *) arg);
}

void
parser_main (int argc, char *argv[])
{
  lex_env_t mylex = { 0 };
  FILE *fp;
  const char *cp = getenv ("DEBUG");

  if (cp != 0)
    {
      DebugLevel = atoi (cp);
    }

  if (argc < 2)
    fp = stdin;
  else
    fp = fopen (argv[1], "r");
  if (fp == 0)
    exit (1);

  sc_out = stdout;

  Lex_env = &mylex;
  Lex_env->le_getline = mygetline;
  Lex_env->le_getline_arg = (char *) fp;
  Lexeme = &Lex_env->le_lexeme;
  init_tokmap ();
  init_symbol_table ();

#if 0
  putenv ("LEX_DEBUG=1");
#endif
  translation_unit ();
  
  return;
}
