;;; 構文解析で生成する構造体の定義
;;; 文法は"sc2c/sc_bnf.dvi"を参照のこと

;;;
;;; 1 External Definitions
;;;

;;; translation-unit:
(defstruct translation-unit
  edecl-list)

;;; external-declaration
(defstruct external-declaration
  decl)

;;; 
;;; 2 Declarations
;;;

;;; declaration
(defstruct decl-variable
  id
  st-class
  texp
  init
  bitfield)       ;; nil or exp

(defstruct decl-function
  defp            ;; t or nil
  id
  st-class
  arg-id-list
  texp
  arg-texp-list
  va-argp         ;; t or nil
  inline          ;; t or nil
  reg-arg-list
  item-list)

(defstruct decl-struct
  defp            ;; t or nil
  s-or-u          ;; 'struct or 'union
  id
  sdecl-list)

(defstruct decl-enum
  id
  elist)

;;; inlined-declaration

;; variable, function, struct, enum -> "declaration"

(defstruct decl-defs
  st-class
  texp
  id-list
  init-list
  bitfield)  ;; nil or exp

(defstruct typedef-texp
  id
  texp)

(defstruct typedef-struct
  id
  s-or-u     ;; 'struct or 'union
  sdecl-list)

(defstruct typedef-enum
  id
  elist)

;;; struct-declaration

;;: enumerator
(defstruct enumerator
  id
  value)      ;; nil or exp

;;: initializer
(defstruct initializer
  compoundp    ;; 'array, 'struct or nil
  init-list)

;;;
;;; 3 Type-expressions
;;;


;;; type-expression
(defstruct type-exp
  spec
  qlist
  plev
  array)

(defstruct texp-array
  plev
  sub-list    ;; nil->[]
  array)

(defstruct tspec-function
  texp
  arg-texp-list
  va-argp)

;;; type-specifier
(defstruct tspec-basic
  type)

(defstruct tspec-name
  id)

(defstruct tspec-enum
  id)

(defstruct tspec-struct
  s-or-u    ;; 'struct or 'union
  id)

;;; 
;;; 4 Statements
;;;

(defstruct null-stat)

;;; compound-statement
(defstruct compound-stat
  item-list)

;;; expression-statement
(defstruct expression-stat
  exp)

;;; labeled-statement
(defstruct label-name
  id
  stat)

(defstruct label-case
  exp)

(defstruct label-default)

;;; selection-statement
(defstruct if-stat
  exp
  then-stat
  else-stat)

(defstruct switch-stat
  exp
  item-list)

;;; iteration-statement
(defstruct while-stat
  exp
  item-list)

(defstruct do-while-stat
  exp
  item-list)

(defstruct for1-stat
  exp1-list
  exp2
  exp3
  item-list)

(defstruct for2-stat
  decl-list 
  exp2
  exp3
  item-list
  for1p)         ;;decl-listがexp-listに置き換えられたとき、
                 ;;for1-statとしてコード生成する。

;;; jump-statement
(defstruct goto-stat
  id)

(defstruct continue-stat)

(defstruct break-stat)

(defstruct return-stat
  exp)

;;;
;;;
;;; 5 Expressions
;;;

;;; expression

(defstruct exp-identifier
  id)

(defstruct exp-constant
  const)

(defstruct exp-string
  str)

(defstruct exp-funcall
  exp
  arg-exp-list)

(defstruct exp-aref
  exp
  aref-exp-list)

(defstruct exp-fref
  exp
  id-list
  op-list)  ;; list of "." or "->"

(defstruct exp-incdec
  exp
  i-or-d)        ;; "++" or "--"

(defstruct exp-preincdec
  exp
  i-or-d)        ;; "++" or "--"

(defstruct exp-unary-op
  op
  exp
  asoc-lev)

(defstruct exp-sizeof
  exp)

(defstruct exp-sizeof-t
  texp)

(defstruct exp-cast
  texp
  exp)

(defstruct exp-operator
  op
  exp-list
  asoc-lev)

(defstruct exp-comparator
  op
  exp1
  exp2
  asoc-lev)

(defstruct exp-ifexp
  exp1
  exp2
  exp3)

(defstruct exp-assignment
  op
  exp1
  exp2
  asoc-lev)

(defstruct exp-exps
  exp-list)




