#ifndef C_PARSER_H
#define C_PARSER_H

#define EOS '\0'
static FILE *sc_out;    // output stream for SC code

void cid2scid (const char*, char*); // identifier C to SC
void string_c2sc( const char *, char * ); // string C to SC

/***/
typedef struct s_constant_expression t_constant_expression;
static void prt_constant_expression( t_constant_expression * );
static void free_constant_expression( t_constant_expression * );

typedef struct s_expression t_expression;
static void prt_expression( t_expression * );
static void free_expression( t_expression * );

typedef struct s_primary_expression t_primary_expression;
static void prt_primary_expression( t_primary_expression * );
static void free_primary_expression( t_primary_expression * );

typedef struct s_postfix_operator t_postfix_operator;
static void prt_postfix_operator( t_postfix_operator * );
static void free_postfix_operator( t_postfix_operator * );

typedef struct s_postfix_operators t_postfix_operators;
static void prt_postfix_operators( t_postfix_operators * );
static void free_postfix_operators( t_postfix_operators * );

typedef struct s_sizeof_expression t_sizeof_expression;
static void prt_sizeof_expression( t_sizeof_expression * );
static void free_sizeof_expression( t_sizeof_expression * );

typedef struct s_unary_expression t_unary_expression;
static void prt_unary_expression( t_unary_expression * );
static void free_unary_expression( t_unary_expression * );

typedef struct s_multiplicative_expression t_multiplicative_expression;
static void prt_multiplicative_expression( t_multiplicative_expression * );
static void free_multiplicative_expression( t_multiplicative_expression * );

typedef struct s_additive_expression t_additive_expression;
static void prt_additive_expression( t_additive_expression * );
static void free_additive_expression( t_additive_expression * );

typedef struct s_shift_expression t_shift_expression;
static void prt_shift_expression( t_shift_expression * );
static void free_shift_expression( t_shift_expression * );

typedef struct s_relational_expression t_relational_expression;
static void prt_relational_expression( t_relational_expression * );
static void free_relational_expression( t_relational_expression * );

typedef struct s_equality_expression t_equality_expression;
static void prt_equality_expression( t_equality_expression * );
static void free_equality_expression( t_equality_expression * );

typedef struct s_and_expression t_and_expression;
static void prt_and_expression( t_and_expression * );
static void free_and_expression( t_and_expression * );

typedef struct s_exclusive_or_expression t_exclusive_or_expression;
static void prt_exclusive_or_expression( t_exclusive_or_expression * );
static void free_exclusive_or_expression( t_exclusive_or_expression * );

typedef struct s_inclusive_or_expression t_inclusive_or_expression;
static void prt_inclusive_or_expression( t_inclusive_or_expression * );
static void free_inclusive_or_expression( t_inclusive_or_expression * );

typedef struct s_logical_and_expression t_logical_and_expression;
static void prt_logical_and_expression( t_logical_and_expression * );
static void free_logical_and_expression( t_logical_and_expression * );

typedef struct s_logical_or_expression t_logical_or_expression;
static void prt_logical_or_expression( t_logical_or_expression * );
static void free_logical_or_expression( t_logical_or_expression * );

typedef struct s_conditional_expression t_conditional_expression;
static void prt_conditional_expression( t_conditional_expression * );
static void free_conditional_expression( t_conditional_expression * );

typedef struct s_assignment_expression t_assignment_expression;
static void prt_assignment_expression( t_assignment_expression * );
static void free_assignment_expression( t_assignment_expression * );

typedef struct s_labeled_statement t_labeled_statement;
static void prt_labeled_statement( t_labeled_statement * );
static void free_labeled_statement( t_labeled_statement * );

typedef struct s_case_statement t_case_statement;
static void prt_case_statement( t_case_statement * );
static void free_case_statement( t_case_statement * );

typedef struct s_default_statement t_default_statement;
static void prt_default_statement( t_default_statement * );
static void free_default_statement( t_default_statement * );

typedef struct s_if_statement t_if_statement;
static void prt_if_statement( t_if_statement * );
static void free_if_statement( t_if_statement * );

typedef struct s_switch_statement t_switch_statement;
static void prt_switch_statement( t_switch_statement * );
static void free_switch_statement( t_switch_statement * );

typedef struct s_while_statement t_while_statement;
static void prt_while_statement( t_while_statement * );
static void free_while_statement( t_while_statement * );

typedef struct s_do_while_statement t_do_while_statement;
static void prt_do_while_statement( t_do_while_statement * );
static void free_do_while_statement( t_do_while_statement * );

typedef struct s_for_statement t_for_statement;
static void prt_for_statement( t_for_statement * );
static void free_for_statement( t_for_statement * );

typedef struct s_break_statement t_break_statement;
static void prt_break_statement( t_break_statement * );
static void free_break_statement( t_break_statement * );

typedef struct s_continue_statement t_continue_statement;
static void prt_continue_statement( t_continue_statement * );
static void free_continue_statement( t_continue_statement * );

typedef struct s_goto_statement t_goto_statement;
static void prt_goto_statement( t_goto_statement * );
static void free_goto_statement( t_goto_statement * );

typedef struct s_return_statement t_return_statement;
static void prt_return_statement( t_return_statement * );
static void free_return_statement( t_return_statement * );

typedef struct s_empty_statement t_empty_statement;
static void prt_empty_statement( t_empty_statement * );
static void free_empty_statement( t_empty_statement * );

typedef struct s_expression_statement t_expression_statement;
static void prt_expression_statement( t_expression_statement * );
static void free_expression_statement( t_expression_statement * );

typedef struct s_statement t_statement;
static void prt_statement( t_statement * );
static void free_statement( t_statement * );

typedef struct s_compound_statement t_compound_statement;
static void prt_compound_statement( t_compound_statement * );
static void free_compound_statement( t_compound_statement * );

typedef struct s_enumerator t_enumerator;
static void prt_enumerator( t_enumerator * );
static void free_enumerator( t_enumerator * );

typedef struct s_enum_specifier t_enum_specifier;
static void prt_enum_specifier( t_enum_specifier * );
static void free_enum_specifier( t_enum_specifier * );

typedef struct s_member t_member;
static void prt_member( t_member * );
static void free_member( t_member * );

typedef struct s_members t_members;
static void prt_members( t_members * );
static void free_members( t_members * );

typedef struct s_struct_or_union_specifier t_struct_or_union_specifier;
static void prt_struct_or_union_specifier( t_struct_or_union_specifier * );
static void free_struct_or_union_specifier( t_struct_or_union_specifier * );

typedef struct s_type_name t_type_name;
static void prt_type_name( t_type_name * );
static void free_type_name( t_type_name * );

typedef struct s_declaration_specifiers t_declaration_specifiers;
static void prt_declaration_specifiers( t_declaration_specifiers * );
static void free_declaration_specifiers( t_declaration_specifiers * );

typedef struct s_pointer t_pointer;
static void prt_pointer( t_pointer * );
static void free_pointer( t_pointer * );

typedef struct s_direct_declarator t_direct_declarator;
static void prt_direct_declarator( t_direct_declarator * );
static void free_direct_declarator( t_direct_declarator * );

typedef struct s_parameter_list t_parameter_list;
static void prt_parameter_list( t_parameter_list * );
static void free_parameter_list( t_parameter_list * );

typedef struct s_suffix_declarator t_suffix_declarator;
static void prt_suffix_declarator( t_suffix_declarator * );
static void free_suffix_declarator( t_suffix_declarator * );

typedef struct s_declarator t_declarator;
static void prt_declarator( t_declarator * );
static void free_declarator( t_declarator * );

typedef struct s_designator t_designator;
static void prt_designator( t_designator * );
static void free_designator( t_designator * );

typedef struct s_initializer t_initializer;
static void prt_initializer( t_initializer * );
static void free_initializer( t_initializer * );

typedef struct s_function_definition t_function_definition;
static void prt_function_definition( t_function_definition * );
static void free_function_definition( t_function_definition * );

typedef struct s_init_declarator t_init_declarator;
static void prt_init_declarator( t_init_declarator * );
static void free_init_declarator( t_init_declarator * );

typedef struct s_declaration t_declaration;
static void prt_declaration( t_declaration * );
static void free_declaration( t_declaration * );

typedef struct s_translation_unit t_translation_unit;
static void prt_translation_unit( t_translation_unit * );
static void free_translation_unit( t_translation_unit * );

/***/
struct s_constant_expression {
  t_conditional_expression *cond_exp;
};

struct s_expression {
  t_expression *exp;
  t_assignment_expression *ass_exp;
};

struct s_primary_expression {
  /* parenthesized expression handled in unary_expression() */
  enum { PR_UNDEFINED, PR_IDENTIFIER, PR_CONSTANT, PR_STRING_LITERAL } spec;
  char *str;
};

struct s_postfix_operator {
  enum
    { PF_UNDEFINED, PF_AREF, PF_CALL, PF_DOT, PF_ARROW, PF_INC, PF_DEC } spec;
  t_expression *exp; // AREF, CALL
  char *identifier;  // DOT, ARROW 
};

struct s_postfix_operators {
  t_postfix_operator *pf;
  t_postfix_operators *next;
};

struct s_sizeof_expression {
  enum { SZ_UNDEFINED, SZ_PR_EXP, SZ_UN_EXP, SZ_TYPE } spec;
  t_expression *exp;          // PR_EXP
  t_postfix_operators *pfs;   // PR_EXP
  t_unary_expression *un_exp; // UN_EXP
  t_type_name *tn;            // TYPE
};

struct s_unary_expression {
  enum { UE_UNDEFINED, UE_SIZEOF, UE_CAST, UE_PAREN, UE_UN_OP, UE_PRIM } spec;
  t_sizeof_expression *sz_exp;  // SIZEOF
  t_type_name *tn;              // CAST
  t_unary_expression *un_exp;   // CAST, UN_OP
  t_expression *exp;            // PAREN
  t_postfix_operators *pfs;     // PAREN, PRIM
  enum { 
    UOP_UNDEFINED, UOP_PLUSPLUS, UOP_MINUSMINUS, UOP_AND, UOP_STAR, 
    UOP_PLUS, UOP_MINUS, UOP_TILDE, UOP_NOT } uop;   // UN_OP
  t_primary_expression *pr_exp; // PRIM
};

struct s_multiplicative_expression {
  enum { MOP_UNDEFINED, MOP_STAR, MOP_SLASH, MOP_PERCENT } mop;
  t_multiplicative_expression *ml_exp;
  t_unary_expression *un_exp;
};

struct s_additive_expression {
  enum { AOP_UNDEFINED, AOP_PLUS, AOP_MINUS } aop;
  t_additive_expression *ad_exp;
  t_multiplicative_expression *ml_exp;
};

struct s_shift_expression {
  enum { SOP_UNDEFINED, SOP_LSHIFT, SOP_RSHIFT } sop;
  t_shift_expression *sh_exp;
  t_additive_expression *ad_exp;
};

struct s_relational_expression {
  enum 
    { ROP_UNDEFINED, ROP_GREATERTHAN, ROP_LESSTHAN, ROP_GTEQ, ROP_LESSEQ } rop;
  t_relational_expression *rl_exp;
  t_shift_expression *sh_exp;
};

struct s_equality_expression {
  enum { EOP_UNDEFINED, EOP_EQEQ, EOP_NOTEQ } eop;
  t_equality_expression *eq_exp;
  t_relational_expression *rl_exp;
};

struct s_and_expression {
  t_and_expression *an_exp;
  t_equality_expression *eq_exp;
};

struct s_exclusive_or_expression {
  t_exclusive_or_expression *xo_exp;
  t_and_expression *an_exp;
};

struct s_inclusive_or_expression {
  t_inclusive_or_expression *or_exp;
  t_exclusive_or_expression *xo_exp;
};

struct s_logical_and_expression {
  t_logical_and_expression *la_exp;
  t_inclusive_or_expression *or_exp;
};

struct s_logical_or_expression {
  t_logical_or_expression *lo_exp;
  t_logical_and_expression *la_exp;
};

struct s_conditional_expression {
  t_logical_or_expression *lo_exp;
  t_expression *exp;
  t_conditional_expression *cd_exp;
};

struct s_assignment_expression {
  enum { AS_UNDEFINED, AS_EQUALS, AS_PLUS_EQUALS, AS_MINUS_EQUALS,
	 AS_STAR_EQUALS, AS_SLASH_EQUALS, AS_PERCENT_EQUALS,
         AS_LSHIFT_EQUALS, AS_RSHIFT_EQUALS,
	 AS_AND_EQUALS, AS_XOR_EQUALS, AS_OR_EQUALS } as;
  t_conditional_expression *cd_exp;
  t_assignment_expression *as_exp;
};

struct s_labeled_statement {
  char *identifier;
  t_statement *stat;
};

struct s_case_statement {
  t_constant_expression *cs_exp;
  t_statement *stat;
};

struct s_default_statement {
  t_statement *stat;
};

struct s_if_statement {
  t_expression *exp;
  t_statement *stat1;
  t_statement *stat2;
};

struct s_switch_statement {
  t_expression *exp;
  t_statement *stat;
};

struct s_while_statement {
  t_expression *exp;
  t_statement *stat;
};
 
struct s_do_while_statement {
  t_expression *exp;
  t_statement *stat;
};

struct s_for_statement {
  enum { FOR_UNDEFINED, FOR_DECL, FOR_EXP } spec;
  t_declaration *decl; // DECL
  t_expression *exp1;  // EXP
  t_expression *exp2;
  t_expression *exp3;
  t_statement *stat;
};

struct s_break_statement {
  int dummy;
};

struct s_continue_statement {
  int dummy;
};

struct s_goto_statement {
  char *identifier;
};

struct s_return_statement {
  t_expression *exp;
};

struct s_empty_statement {
  int dummy;
};

struct s_expression_statement {
  t_labeled_statement *lb_stat;
  t_expression *exp;
};

struct s_statement {
  enum { ST_UNDEFINED, ST_EXP, ST_CASE, ST_DEFAULT, ST_IF, ST_SWITCH,
	 ST_WHILE, ST_DO_WHILE, ST_FOR, ST_BREAK, ST_CONTINUE, ST_GOTO,
	 ST_RETURN, ST_COMPOUND, ST_EMPTY } spec;
  t_expression_statement *ex_stat;
  t_case_statement *cs_stat;
  t_default_statement *df_stat;
  t_if_statement *if_stat;
  t_switch_statement *sw_stat;
  t_while_statement *wh_stat;
  t_do_while_statement *dw_stat;
  t_for_statement *fo_stat;
  t_break_statement *br_stat;
  t_continue_statement *ct_stat;
  t_goto_statement *gt_stat;
  t_return_statement *rt_stat;
  t_compound_statement *cp_stat;
  t_empty_statement *em_stat;
};

typedef struct s_block_item_list {
  enum { BI_UNDEFINED, BI_STAT, BI_DECL } spec;
  t_statement *stat;   // STAT
  t_declaration *decl; // DECL
  struct s_block_item_list *next;
} block_item_list;

struct s_compound_statement {
  block_item_list *bi_list;
};

struct s_enumerator {
  char *identifier;
  t_constant_expression *cs_exp;  // if exists
};

typedef struct tag_enumerator_list {
  t_enumerator *en;
  struct tag_enumerator_list *next;
} enumerator_list;

struct s_enum_specifier {
  char *identifier;          // if exists
  enumerator_list *en_list;  // if exists  
};

struct s_member {
  t_declarator *dclr;
  t_constant_expression *cs_exp; // if exists (bit-field)
};

typedef struct tag_member_list {
  t_member *mem;
  struct tag_member_list *next;
} member_list;

typedef struct tag_struct_declaration_list {
  t_declaration_specifiers *dspec;
  member_list *mem_list;
  struct tag_struct_declaration_list *next;
} struct_declaration_list;

struct s_members {
  struct_declaration_list *sdec_list;
};

struct s_struct_or_union_specifier {
  enum { SU_UNDEFINED, SU_STRUCT, SU_UNION } spec;
  char *identifier;  // if exists
  t_members *mems;   // if exists
};

struct s_type_name {
  t_declaration_specifiers *dspec;
  t_declarator *dclr;
};

struct s_declaration_specifiers {
  enum { TP_UNDEFINED, TP_ELEMENTAL, TP_STRUCT, TP_ENUM, TP_NAME } spec;
  enum { STC_UNDEFINED,  STC_TYPEDEF, STC_EXTERN, STC_STATIC, STC_AUTO, 
	 STC_REGISTER } st_class;
  enum { QF_END, QF_CONST, QF_RESTRICT, QF_VOLATILE } qf[32];
  enum { SG_UNDEFINED, SG_SIGNED, SG_UNSIGNED } et_sign; // ELEMENTAL
  enum { ET_UNDEFINED,                                   // ELEMENTAL
	 ET_VOID, ET_CHAR, ET_SHORT, ET_INT, ET_FLOAT, ET_DOUBLE } et_type;
  enum { LG_UNDEFINED, LG_LONG, LG_LONGLONG } et_long;   // ELEMENTAL
  t_struct_or_union_specifier *suspec;                   // STRUCT
  t_enum_specifier *espec;                               // ENUM
  char *identifier;                                      // NAME
};

struct s_pointer {
  enum { PT_END, PT_STAR, PT_CONST, PT_RESTRICT, PT_VOLATILE } pt_qf[32];
};

struct s_direct_declarator {
  enum { DD_UNDEFINED, DD_NULL, DD_PAREN, DD_IDENTIFIER } spec;
  t_declarator *dclr;  // PAREN
  char* identifier;    // IDENTIFIER
};

typedef struct tag_param_list {
  char *identifier;                   // oldstyle
  t_declaration_specifiers *dspec;    // newstyle
  t_declarator *dclr;                 // newstyle
  struct tag_param_list *next;
} param_list;

struct s_parameter_list {
  int new_style;
  param_list *pm_list;
  int va_arg;
};

struct s_suffix_declarator {
  enum { SD_UNDEFINED, SD_ARRAY, SD_FUNC } spec;
  t_constant_expression *cs_exp;   // ARRAY
  t_parameter_list *param_list;    // FUNC
  int is_func;
};

typedef struct tag_suffix_declarator_list {
  t_suffix_declarator *sf_dclr;
  struct tag_suffix_declarator_list *next;
} suffix_declarator_list;

struct s_declarator {
  t_pointer *pt;
  t_direct_declarator *ddclr;
  suffix_declarator_list *sd_list;
};

struct s_designator {
  enum { DSG_UNDEFINED, DSG_AREF, DSG_STRUCT } spec;
  t_constant_expression *cs_exp;  // AREF
  char *identifier;               // STRUCT
};

typedef struct tag_initalizer_list {
  t_initializer *init;
  struct tag_initalizer_list *next;
} initializer_list;

struct s_initializer {
  enum { INIT_UNDEFINED, INIT_LIST, INIT_LISTELM, INIT_ASEXP } spec;
  initializer_list *init_list;      // LIST
  t_designator *dsg;                // LISTELM
  t_initializer *init;              // LISTELM
  t_assignment_expression *as_exp;  // ASEXP
};

typedef struct tag_declaration_list {
  t_declaration *decl;
  struct tag_declaration_list *next;
} declaration_list;

struct s_function_definition {
  t_compound_statement *cp_stat;
  declaration_list *decl_list;    // old_style
};

struct s_init_declarator {
  int is_fn_def; 
  t_declarator *dclr;
  t_function_definition *fn_def;  // when   is_fn_def
  t_initializer *init;            // unless is_fn_def
};

typedef struct tag_init_declarator_list {
  t_init_declarator *idclr;
  struct tag_init_declarator_list *next;
} init_declarator_list;

struct s_declaration {
  t_declaration_specifiers *dspec;
  init_declarator_list *idclr_list;
};

typedef struct tag_declaration_unit {
  t_declaration *decl;
  struct tag_declaration_unit *next;
} declaration_unit;

struct s_translation_unit {
  declaration_list *decl_list;
};


#endif
