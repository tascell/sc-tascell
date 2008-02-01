;;;; SCコンパイラ

;;; 2003/11/?? 変換規則が２つ目以降の引数を受け取るのに対応
;;; 2003/11/14 sc2scにマクロ展開をsc2sc内で呼び出すオプションを追加
;;; 2003/11/28 rule-file を複数指定できるようにした。
;;; 2004/05/06 マッチしたパタン変数名のリストをmatched-symbolとして
;;;            利用可能にした
;;; 2004/06/06 matched-symbol を ( symbol . retval ) のリストに変更
;;;            シンボルがパターン変数に含まれるかどうかを調べる
;;;             pattern-variable-p と [関数名]の返り値を獲得する 
;;;             get-retval をマクロとして追加
;;; 2004/06/08 ルール返り値のmultiple-valueに対応
;;;            ,@val[<func>] に対する (get-retval val) は
;;;            (values <返り値1のリスト> <返り値2のリスト> ... )
;;; 2004/06/12 デバッグ機能を強化
;;;             (rule-trace <rule-name> <rule-name> ...)
;;;             (rule-untrace <rule-name> <rule-name> ...)
;;;            で、Lispの trace, untrace のようなことができる。
;;; 2004/06/23 { ... } で optional なパターンが記述可能に
;;;            ( a {b c} ) == ( a b c ) or ( a )

(provide 'sc2sc)
(require 'sc-misc)


(defvar *rule-path* '("./rule"))
(defvar *rule-list* nil) ; list of (name . 
                         ;          [list of ((pattern ,@arg-list) . output)])
(defvar *flet-list* nil) ; list of (rname (x) (apply-rule rname x))
(defvar *otherwise-default* nil)
(defvar *trace-rule* nil)
(defvar *trace-all* nil)
(defvar *trace-unmatch-verbose* nil)
(defvar *trace-depth* 0)
(defvar *pattern-match-debug* nil)
(defconstant comma 'BQ-COMMA)          
(defconstant comma-at 'BQ-COMMA-ATSIGN)
(defconstant bquote 'QUOTE)        
(defconstant optional 'OPTIONAL)

#| ;; sc2scディレクトリ内から単独でロードするとき
(load "../sc-defs")
(load "../sc2c-parse")
(load "../sc-misc")
(load "int-sc2sc-macch")
(load "int-sc2sc-lib")
|#

;;; rnameで指定されたルールを適用
(defun apply-rule (rname x arg-list
			 &aux pat-list bind-list output match-pat
			 (trace-p 
			  (or *trace-all*
			      (member rname *trace-rule* :test #'eq)))
			 (*trace-depth* (if trace-p 
					    (1+ *trace-depth*)
					    *trace-depth*))
			 (indent-space (make-string *trace-depth*
						    :initial-element #\Space)))
  (let (rule)
    (unless
	(setq rule (car (member rname *rule-list* :key #'car :test #'eq)))
      (error "rule ~s is undefined." rname))
    (setq pat-list (cdr rule))) ; list of ((pattern ,@arg-list) . output)

  ;; trace-print
  (when trace-p
    (format t "~A~S] (~S ~S)~%" indent-space *trace-depth* rname 
	    (abbrev-list x)))

  (dolist (pat pat-list)
    ;; trace-print
    (when (and trace-p *trace-unmatch-verbose*)
      (format t "~A== ~A ==~%" indent-space (pattern-print (caar pat))))
    (when (setq bind-list (match-pattern (caar pat) x))
      ;; trace-print
      (when trace-p 
	(if *trace-unmatch-verbose*
	    (format t "~A!!! match !!!~%" indent-space)
	    (format t "~A== ~A ==~%" indent-space (pattern-print (caar pat)))))
      (setq match-pat (caar pat))
      (setq output (cdr pat))
      ;; set arguments except pattern
      (do ((rarg arg-list (cdr rarg))
	   (karg (cdar pat) (cdr karg)))
	  ((or (endp rarg) (endp karg))
	   (if (not (endp rarg))
	       (error "~s:too many arguments." rname)
	     (if (not (endp karg))
		 (error "~s:too few arguments." rname)
	       t)))
	(push `(,(car karg) ',(car rarg)) bind-list))
      (return bind-list)))

  (let ((out
	 (if bind-list
	     ;; get return value
	     (multiple-value-list
		 (eval-with-rule output bind-list))
	     ;;	(eval `(flet ,*flet-list*
	     ;;		(let ,bind-list ,output)))))
	     (prog1
		 *otherwise-default*
	       (when trace-p (format t "~A<no pattern matched>~%"
				     indent-space))))))

    ;; trace-print
    (when trace-p
      (format t "~A~S[(~S ~A)~%"
	      indent-space *trace-depth* rname 
	      (string+ (mapcar (compose #'prin1-to-string #'abbrev-list) out)
		       " ")))
    
    (apply #'values out)))

;;; ルール適用を有効にした状態でeval
(defmacro eval-with-rule (sexp &optional bind-list)
  `(eval (list 'flet *flet-list* (list 'let ,bind-list ,sexp))))

;;; patternの内部表現を、, や ,@ を使った形の文字列に直す
(defun pattern-print (pat)
  (if (listp pat)
      (cond
	((eq comma (car pat))
	 (format nil ",~S" (second pat)))
	((eq comma-at (car pat))
	 (format nil ",@~S" (second pat)))
	(t
	 (format nil "(~A)"
		 (string+ (mapcar #'pattern-print pat) " "))))
      (format nil "~S" pat)))
	 
;;; マッチした場合は変数のバインドリストx(含 `(x ,x), matched-symbol)
;;; しなかった場合はnil
(defvar *pattern-variable-list* nil) ; list of (symbol variable retval)
(defun match-pattern (pattern x)
  (let ((*pattern-variable-list* nil))
    (if (match-pattern2 (list pattern) (list x))
	(let* ((bind-list
		(mapcar #'(lambda (pv) (list (first pv) (second pv)))
			*pattern-variable-list*)))
	  (push `(x ',x) bind-list)
	  (push `(matched-symbol
		  ',(mapcar #'(lambda (pv)
				(cons (first pv) (third pv)))
			    *pattern-variable-list*))
		bind-list))
	nil)))

(defun match-pattern2 (pattern x)
  (unless (listp pattern) (error "type of ~s is not list." pattern))
  (do* ((rpat pattern (cdr rpat))
	(rx x (cdr rx))
	(cpat (car rpat) (car rpat)) (cx (car rx) (car rx)))
       ((endp rpat) (endp rx))
    ;;
    (when *pattern-match-debug* 
      (format t "~a <=> ~s~%" (pattern-print cpat) cx))
    ;;
    (cond
      ((symbolp cpat)                  ; cpat = <symbol>
       (unless (eq cpat cx) (return nil)))
      ((consp cpat)
       (cond
	 ((and (eq comma (car cpat))
	       (symbolp (cadr cpat)))  ; cpat = (comma <symbol>)
	  (when (endp rx) (return nil))
	  (multiple-value-bind (sname testfunc) 
	      (get-sname-and-testfunc (cadr cpat))
	    (let ((retval nil))	       
	      (when (and testfunc
			 (not (car 
			       (setq retval
				     (multiple-value-list
				       (eval-with-rule 
					`(funcall ,testfunc ,sname)
					`((,sname ',cx))))))))
		(return nil))
	      (push `(,sname ',cx ,retval) *pattern-variable-list*))))
	 ((and (eq comma-at (car cpat))
	       (symbolp (cadr cpat)))  ; cpat = (comma-at <symbol>)
	  (let (new-pvlist flag (rrx rx))
	    (dotimes (i (1+ (length rx)))
	      (let ((*pattern-variable-list* *pattern-variable-list*))
		(setq flag (match-pattern2 (cdr rpat) rrx))
		(when flag (setq new-pvlist *pattern-variable-list*)))
	      (when flag
		(let ((bx (butlast rx (- (length rx) i)))
		      sname testfunc (retval nil))
		  (multiple-value-setq
		      (sname testfunc) (get-sname-and-testfunc (cadr cpat)))
		  (when (or (null testfunc)
			    (null bx)
			    (first
			     (setq retval
				   (multiple-value-list
				    (eval-with-rule
				     `(check-mapcar ,testfunc ,sname)
				     `((,sname ',bx)))))))
		    (setq *pattern-variable-list*
			  (cons `(,sname ',bx ,retval) new-pvlist))
		    (return-from match-pattern2 t))))
	      (setq rrx (cdr rrx))
	      ) ; dotimes
	    )
	  (return-from match-pattern2 nil))
	 (t                            ; cpat = <list>
	  (unless (and (listp cx)
		       (match-pattern2 cpat cx))
	    (return nil)))))
      (t (error "unexpected type of pattern ~s" cpat)))))

;; sname[testfunc] -> sname & testfunc
(defun get-sname-and-testfunc (symbol &aux testfunc fnm symstrl)
  (setq symstrl (concatenate 'list (string symbol)))
  (if (and (setq fnm (member #\[ symstrl :test #'char=))
	   (= 1 (length (member #\] fnm :test #'char=))))
      (progn
	(setq testfunc (cdr fnm))
	(rplacd fnm nil)
	(values
	 (with-input-from-string 
	     (st (concatenate 'string (butlast symstrl)))
	   (read st))
	 (with-input-from-string
	     (st (concatenate 'string (butlast testfunc)))
	   (let ((read-st (read st)))
	     (if (symbolp read-st)
		 `(function ,read-st)
		 read-st)))))
      (values symbol nil)))


;;; optional を含むパタンを展開
(defun extract-optional (y)
  (if (listp y)
      (if (eq optional (car y))
	  (append (mapcar #'car (extract-optional (cdr y))) (list nil))
	  (let ((y-case-list (mapcar #'extract-optional y)))
	    (labels ((all-case-append (case-list)
		       (if case-list
			   (mapcan #'(lambda (x)
				       (mapcar #'(lambda (xx) (append x xx))
					       (all-case-append
						(cdr case-list))))
				   (car case-list))
			   (list nil))))
	      (mapcar #'list (all-case-append y-case-list)))))
      (list (list y))))

;;; 変形規則ファイルの読み込み
;;; 返り値は rule-list と flet-list の values
(defun load-rule (rule-file &optional (load-path *rule-path*)
		  &aux (rule-list nil) (flet-list nil) (input-data nil))
  (unless (stringp rule-file)
    (error "~s is not string" rule-file))
  (dolist (cpath (cons "." load-path)
	   (error "file ~s not found." rule-file))
    (let ((file-temp nil))
      (when (probe-file  
	     (setq file-temp (format nil "~A/~A" cpath rule-file)))
	(return (setq rule-file file-temp)))))
  
  ;; p-readtable : パタン読み込み用のreadtable
  ;; n-readtable : 普通のreadtable
  (let* ((p-readtable (copy-readtable))
	 (n-readtable *readtable*)
	 (*readtable* p-readtable))
    (redefine-macro-character p-readtable)
    ;; read file
    (with-open-file (istream rule-file :direction :input)
      (do ((y (read istream nil 'eof) (read istream nil 'eof))
	   (patlist nil))
	  ((eq y 'eof) (setq input-data (nreverse input-data)))
	;;(print y)
	(if (eq '-> y)
	    (progn
	      (let ((*readtable* n-readtable))
		(setq y (read istream)))
	      ;; "<pat1> <pat2> -> (...)" ==> "<pat1> -> (...) <pat2> -> (...)"
	      (dolist (pat patlist)
		(push pat input-data)
		(push '-> input-data)
		(push y input-data))
	      (setq patlist nil))
	    ;; パタンを覚えておく
	    (setq patlist
		  (append (apply #'append (extract-optional y)) 
			  patlist))))))
  
  ;; rule-list を生成
  (do* ((xx input-data (cdr xx))
	(x (car xx) (car xx))
	(md 0 (rem (1+ md) 3))
	(p nil) (pat nil))
       ((endp xx)
	(progn
	  (setq rule-list (nreverse rule-list))
	  (dolist (rule rule-list rule-list)
	    (rplacd rule (nreverse (cdr rule))))))
    (case md
      ((0) ; read rule-name, pattern and arguments
       (unless (listp x)
	 (error "syntax error at ~s" x))
       (unless (symbolp (car x))
	 (error "~s is not rule name" (car x)))
       (unless (setq p (member (car x) rule-list :key #'car :test #'eq))
	 (setq p (push `(,(car x)) rule-list)))
       (setq pat (cdr x)))
      ((1) ; read symbol '->'
       (unless (eq '-> x)
	 (setq md 2)))
      ((2) ; read function value
       (rplacd (car p) (cons `(,pat . ,x) (cdar p))))
      (otherwise 
       (error "internal error:unexpected value of md"))))

  ;;; flet-listを生成。  `(flet ,flet-list ...) で
  ;;; 通常の関数呼び出しをapply-ruleの呼び出しに化けさせる
  (setq flet-list nil)
  (dolist (rule rule-list flet-list)
    (push `(,(car rule) (x &rest arg-list)
	    (apply-rule ',(car rule) x arg-list)) 
	  flet-list))
  
  (values rule-list flet-list)
  )


;;; sc-file-or-list: 変換対象のSCファイル名またはS式
;;; rule-file: 変換規則ファイル名 またはそのリスト
;;; output-file: 出力ファイル
;;; mac: 事前にマクロ展開を行うか(t or nil)
(defun sc2sc (sc-file-or-list rule-file &key output-file (mac t)
	      &aux (*rule-list* nil) (*flet-list* nil))
  ;; rule-fileが複数指定された場合
  (when (listp rule-file)
    (when (null rule-file) (return-from sc2sc sc-file-or-list))
    (format t "applying ~S:~%" (car rule-file))
    (return-from sc2sc
       (sc2sc
	(sc2sc sc-file-or-list (car rule-file) :mac mac
	       :output-file output-file)
	(cdr rule-file) :output-file output-file :mac mac)))  

  ;; 変形規則ファイルの読み込み
  (multiple-value-setq (*rule-list* *flet-list*)
    (load-rule rule-file))		       

  ;;; Read SC source
  (let (x)
    (if (stringp sc-file-or-list)
	;; sc-file-or-list が文字列ならそのファイルを読み込む
	(progn
	  (unless (probe-file sc-file-or-list)
	    (error "file ~s not found." sc-file-or-list))
	  (with-open-file (istream sc-file-or-list :direction :input)
	    (do ((y (read istream nil 'eof) (read istream nil 'eof)))
		((eq y 'eof) (setq x (nreverse x)))
	      (push y x))))
	;; 文字列でなければscプログラムとして受け取る
	(setq x sc-file-or-list))

    (when mac
      (setq x (apply-macro x)))
    
    ;;; apply-rule to whole program
    (setq x (apply-rule (caar *rule-list*) x nil))
    (when output-file          
      (with-open-file (*standard-output* output-file :direction :output)
	(if (listp x)
	    (dolist (xx x) (print xx))
	    (princ x))))
    x)
)

;;;;テスト用;;;;;
(defun mp-test (&aux pattern x)
  (let ((*readtable* (copy-readtable)))
    (format t "pattern:")
    (redefine-macro-character) (setq pattern (read)))
  (format t "x      :") (setq x (read))
  (let ((*pattern-match-debug* t))
    (match-pattern pattern x)))


(defun rule-test (rule-file sexp)
  (multiple-value-bind (*rule-list* *flet-list*) (load-rule rule-file)
    (eval-with-rule sexp)))

#|
(defun test ()
  (load "int-sc2sc.lsp")
  (sc2sc "/home/lab4/hiraisi/sc2c/source/strtest.sc"
         "/home/lab4/hiraisi/sc2c/source/rule/basic.rule"))
|#
