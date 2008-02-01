;;;; パターンマッチ用マクロ
(provide "SCT-RULE2LSP")
(eval-when (:compile-toplevel :load-toplevel :execute)
  (require "SC-DECL" "sc-decl.lsp"))

(in-package "SC-TRANSFORMER")

#+obsolete (scr:require "SCT")
;;(scr:require "SCT-MACRO")
(scr:require "SCT-MACCH")
(scr:require "SCT-LIB")
(scr:require "SC-MISC")


;;; <-- 補助関数
;; パタン内のパタン変数名のリストを得る
(defun get-pattern-variables (pat)
  (assert (pattern-p pat))
  (labels ((gpv-acc (pat acc)
             (cond
              ((atom pat) acc)
              ((or (pat-commaat-p pat)
                   (pat-comma-p pat))
               (list (second pat)))
              (t
               (gpv-acc (cdr pat)
                        (nconc (gpv-acc (car pat) nil) acc))))))
    (delete-duplicates (gpv-acc (pattern-body pat) nil) :test #'eq)))

(defun count-pattern-variables (pat)
  (length (get-pattern-variables pat)))

;; パターンから,@の場所を捜す
(defun search-commaat (pattern)
  (member-if #'pat-commaat-p pattern))

;; patternがnilにマッチするか
(defun matches-to-nil-p (pattern)
  (and (consp pattern)
       (every #'pat-commaat-p pattern)))

;; パターンを,@より前とそれ以降で分割
(defun split-at-commaat (pattern)
  (list-until pattern (search-commaat pattern)))

;; ,pv[]や,@pv[]における[]の中身==>マッチ成立を判定するフォーム
;; マッチング成立ならsave-placeにget-retvalで獲得するための multiple-value-list
;; をセットする（不成立ならsave-placeの値は保証しなくてよい）
;; 返す値はマッチの成否を表してさえいれば特にretvalでなくてよい
(defun []-to-testform (in-[] argform saveplace)
       (cond
        ((cdr in-[])
         (case (car in-[])              ; :evalに続くフォームの値が非nilならマッチ
           ((:eval)
            `(car (setf ,saveplace
                    (multiple-value-list (progn ,@(cdr in-[]))))) )
           ((:member)                   ; :memberに続く値（リスト）に含まれていればマッチ
            `(car (setf ,saveplace
                    (list (member ,argform ,(second in-[]) :test #'equal)))) )
           (otherwise
            (error "Unexpected form: [~{~S~^ ~}]" in-[]))))
        ((null in-[])                   ; ,var[] == ,var（常にマッチ）
         `(setf ,saveplace '(t)))
        ((symbolp (car in-[]))          ; ,var[symbol] --> (funcall #'var symbol)
         (unless (eq (symbol-package (car in-[]))
                     (find-package "RULE"))
           (warn ",~0@*~S[~1@*~S]: ~1@*~S may not be rule-name.~%~
             In order to test whether (~1@*~S ~0@*~S) is non-nil, write ,~0@*~S[#'~1@*~S]."
                 argform (car in-[])))
         `(not (eq 'rule::no-match      ; test if non-rule::no-match
                   (car (setf ,saveplace
                          (multiple-value-list (,(car in-[]) ,argform))))) ) )
        ((atom (car in-[]))             ; ,var[atom] --> (equal atom symbol)
         `(car (setf ,saveplace
                 (list (equal ,(car in-[]) ,argform)))) )
        ((eq 'function (caar in-[]))    ; ,var[#'<form>] --> (funcall #'<form> symbol)
         `(car (setf ,saveplace         ; test if non-nil
                 (multiple-value-list (funcall ,(car in-[]) ,argform)))) )
        (t                              ; atomでも関数フォームでもない --> :member
         ([]-to-testform `(:member ,(car in-[])) argform saveplace))))

;;; optional を含むパタンを展開
(defun extract-optional (pat)
  (assert (pattern-p pat))
  (mapcar #'(lambda (xx) (make-pattern :body xx))
          (apply #'append (extract-optional-rec (pattern-body pat)))))
(defun extract-optional-rec (pbody)
  (if (listp pbody)
      (if (pat-optional-p pbody)
          (nconc (mapcar #'car (extract-optional-rec (cdr pbody))) (list nil))
        (let ((pbody-case-list (mapcar #'extract-optional-rec pbody)))
          (labels ((all-case-append (case-list)
                     (if case-list
                         (mapcan #'(lambda (x)
                                     (mapcar #'(lambda (xx) (append x xx))
                                             (all-case-append
                                              (cdr case-list))))
                                 (car case-list))
                       (list nil))))
            (mapcar #'list (all-case-append pbody-case-list)))))
    (list (list pbody))))

;;;; 
(defun match-make-last (form n)
  (assert (integerp n))
  (cond ((= n 0) 'nil)
        ((= n 1) `(last ,form))
        (t `(last ,form ,n))))

(defun match-make-butlast (form n)
  (assert (integerp n))
  (cond ((= n 0) form)
        ((= n 1) `(butlast ,form))
        (t `(butlast ,form ,n))))

(defun match-make-make-array (n)
  (if (<= n 0) '#()
    `(make-array ,n)))

;;; 補助関数 -->


;; form-varの変数の値が patternsのいずれかにマッチすれば，then-form 
;; しなければ else-form を実行
(defmacro matching-exp (patterns-opt then-form else-form form-var
			&optional (retvals-array-form nil))
  (let* ((patterns (mapcan #'extract-optional patterns-opt))
         (pv-list (mapcar #'get-pattern-variables patterns)) ; pat-vars for each pattern
         (all-pv (delete-duplicates (apply #'append pv-list) :test #'eq)) ; merged pv-list
	 (retvals-array-form (aif retvals-array-form it
				  (match-make-make-array (length all-pv))))
         (retvals-var (gensym "RETVALS")) ; binds array of (or <retvals> 'sct::unused)
         (block-tag (gensym "MATCH-BLOCK"))
         (set-retval-fvar (gensym "SET-RETVAL")))
    `(block ,block-tag
       (let (,@all-pv 
             ;; (position <sym> all-pv) に <retvals> または 'sct::unused
             (,retvals-var ,retvals-array-form))
         (declare (ignorable ,@all-pv ,retvals-var))
         (macrolet ((sct-user:pattern-variable-p (sym)
                      (if (quoted-p sym)
                          `(sct-user:pattern-variable-p ,(second sym))
                        (progn
                          (assert (symbolp sym))
                          `(not (eq 'sct::unused ,sym)))))
                    (sct-user:get-retval (sym)
                      (if (quoted-p sym) ; only for compatibility
                          `(sct-user:get-retval ,(second sym))
                        (progn
                          (assert (symbolp sym))
                          `(values-list (aref ,',retvals-var
                                              ,(position sym ',all-pv))))))
                    (,set-retval-fvar (sym val)
                      (if (quoted-p sym) ; only for compatibility
                          `(,',set-retval-fvar ,(second sym) ,val)
                        (progn
                          (assert (symbolp sym))
                          `(setf (aref ,',retvals-var
                                       ,(position sym ',all-pv))
                             ,val))))
                    )
           (declare (ignorable (function sct-user:pattern-variable-p)
                               (function sct-user:get-retval)
                               (function ,set-retval-fvar)))
           ,.(mapcan
              #'(lambda (pattern pv)
                  (assert (pattern-p pattern))
                  (list
                   ;; マッチング
                   `(when ,(match-check-form (pattern-body pattern) form-var set-retval-fvar)
                      ,.(loop for unused-var in (set-difference all-pv pv)
                            collect `(setq ,unused-var 'sct::unused))
                      (return-from ,block-tag ,then-form))
                   )
                  )
              patterns pv-list)
           (return-from ,block-tag ,else-form))))
    ))

#+obsolete ; too inefficient...
(defmacro matching-exp (patterns-opt then-form else-form form-var)
  (let* ((patterns (mapcan #'extract-optional patterns-opt))
         (pv-list (mapcar #'get-pattern-variables patterns)) ; pat-vars for each pattern
         (all-pv (remove-duplicates (apply #'append pv-list) :test #'eq)) ; merged pv-list
         (patvars-var (gensym "PATVARS")) ; binds list-of (<sym> . <get-retval-values>)
         (block-tag (gensym "MATCH-BLOCK"))
         (reset-pv-fvar (gensym "RESET-PV"))
         (set-retval-fvar (gensym "SET-RETVAL"))
         (do-then-fvar (gensym "DO-THEN"))
         (do-else-fvar (gensym "DO-ELSE")))
    `(block ,block-tag
       (let (,@all-pv ,patvars-var)
         (declare (ignorable ,@all-pv ,patvars-var))
         (labels ((,do-then-fvar ()
                    (return-from ,block-tag ,then-form))
                     (,do-else-fvar ()
                       (return-from ,block-tag ,else-form))
                   (,reset-pv-fvar ()
                     (setq all-pv ,@(mappend (tail-tagger 'nil))))
                   (sct-user:pattern-variable-p (sym)
                     (assoc sym ,patvars-var :test #'eq))
                   (sct-user:get-retval (sym)
                     (let ((hit (sct-user:pattern-variable-p sym)))
                       (if hit
                           (values-list (cdr hit))
                         (error "~S is not a pattern-variable." sym))))
                   (,set-retval-fvar (sym val)
                     (setf (cdr (sct-user:pattern-variable-p sym)) val)))
           (declare (ignorable (function sct-user:pattern-variable-p)
                               (function sct-user:get-retval)
                               (function ,set-retval-fvar)))
           ,@(mapcan
              #'(lambda (pattern pv)
                  (assert (pattern-p pattern))
                  (list
                   ;; patvars <== ((a . nil) (b . nil) ...)
                   `(setq ,patvars-var
                      (list ,@(mapcar (tagger 'list)
                                      (mapcar (tagger 'quote) pv))))
                   ;; reset pattern variables
                   `(,reset-pv-fvar)
                   ;; マッチング
                   `(when ,(match-check-form (pattern-body pattern) form-var set-retval-fvar)
                      (,do-then-fvar))
                   )
                  )
              patterns pv-list)
           (,do-else-fvar))))
    ))

;; 変数form-var がpatternにマッチするための条件式を作る
(defun match-check-form (pbody form-var set-retval-fvar) 
  `(and ,@(sort-check-list
           (match-check-list-for-exp pbody form-var set-retval-fvar))) )

;; check-list を判定が楽（と思われる）
;; かつ必要順（listpが (<= (length ...)) より先など）に並びかえ
(defun sort-check-list (check-list)
  (stable-sort 
   check-list #'<
   :key #'(lambda (check-form)
            (or (and (atom check-form) 0)
                (and (position (car check-form) '(listp eq list-length>= list-length= equal))
                     1)
                (aand (position (car check-form) '(let loop))
                      (+ it 1000))
                (progn 
                  (warn "Unexpected check-form is created: ~S" check-form)
                  500)))))

(defun match-check-list-for-exp (pbody form-var set-retval-fvar) 
  ;; リストの要素に対して
  (cond
   ((symbolp pbody) (list `(eq ',pbody ,form-var)) )
   ((atom pbody)    (list `(equal ',pbody ,form-var)) )
   ((pat-comma-p pbody)
    (let ((pv (second pbody))
          (in-[] (cddr pbody))
          (retval-var (gensym "RETVAL")))
      (list
       `(let (,retval-var)
          (setq ,pv ,form-var)
          (and ,([]-to-testform in-[] pv retval-var)
               (,set-retval-fvar ',pv ,retval-var)))) ))
   ((pat-commaat-p pbody)
    (error "Pattern consists of only ,@<symbol>: ~A" 
           (make-pattern :body pbody)))
   (t
    (match-check-list-for-list pbody form-var set-retval-fvar))))

(defun match-check-list-for-list (pbody form-var set-retval-fvar) 
  ;; リストに対して
  (multiple-value-bind (until-commaat from-commaat)
      (split-at-commaat pbody)
    (let ((len-until (length until-commaat)))
      (list*
       ;; リストの長さがパターンと同じ（,@が残っている場合はパターン以上）
       `(listp ,form-var)
       `(,(if from-commaat 'list-length>= 'list-length=) ,form-var ,len-until)
       (append
        ;; ,@以前の各要素をチェック
        (loop
          :for i :upfrom 0
          :as x :in until-commaat
          :nconc (match-check-list-for-exp x `(nth ,i ,form-var) set-retval-fvar))
        ;; ,@以降をチェック
        (when from-commaat
          (match-check-list-for-commaat 
           from-commaat `(nthcdr ,len-until ,form-var) set-retval-fvar))))
      )))

(defun match-check-list-for-commaat (pbody form-var set-retval-fvar)
  (assert (pat-commaat-p (car pbody)))
  ;; ,@から始まるリストに対して
  (cond
   ((not (search-commaat (cdr pbody)))  ;,@がもうないときは高速化
    (let ((rest-len (length (cdr pbody))) ; ,@を除いたパターンの長さ
          (pv (second (car pbody)))
          (in-[] (cddr (car pbody)))
          (test-var (gensym "TEST"))
          (i-var (gensym "I")) (x-var (gensym "X")) (r-var (gensym "R"))
          (valslist-var (gensym "VALSLIST"))
          (maxvals-var (gensym "MAXVALS")))
      `( ;; ,@より後のパターンをチェック
        ,@(match-check-list-for-list
           (cdr pbody) (match-make-last form-var rest-len) set-retval-fvar)
          ;; ,@部分をチェック（要素数確定）
          (loop
            :with ,r-var
            :with ,test-var := ,(match-make-butlast form-var rest-len)
            :for ,i-var :of-type fixnum :from 1 :to (length ,test-var)
            :as ,x-var :in ,test-var
            :always ,([]-to-testform in-[] x-var r-var) ; 全ての返り値が非nilかチェック
            :collecting ,r-var :into ,valslist-var
            :maximizing (length ,r-var) :into ,maxvals-var ; []が返すvaluesの最大数
            :finally
            (setq ,pv ,test-var)
            (,set-retval-fvar           ; 返り値をpatvarsに記憶
             ',pv (combine-each-nth ,valslist-var :n ,maxvals-var))) )
      ))

   (t                                   ; ,@がまだある場合=>しらみつぶし
    (let ((pv (second (car pbody)))
          (in-[] (cddr (car pbody)))
          (rest-var (gensym "REST"))
          (r-var (gensym "R")) (valslist-var (gensym "VALSLIST"))
          (maxvals-var (gensym "MAXVALS")))
      (list
       `(loop
          :initially (when (endp ,form-var)
                       (return ,(matches-to-nil-p (cdr pbody))))
          :with ,r-var
          :for ,rest-var :on ,form-var  ; rest-varより前を先頭の,@部分だと思う
          :until ,(match-check-form (cdr pbody) rest-var set-retval-fvar)
          :always (and
                   ,@(unless (matches-to-nil-p (cdr pbody)) 
                       `((cdr ,rest-var)) ) ; formを使い切ったときは特別扱い
                   ,([]-to-testform in-[] `(car ,rest-var) r-var) ; []内の関数をチェック
                   )
          :collecting ,r-var :into ,valslist-var
          :maximizing (length ,r-var) :into ,maxvals-var
          :finally                      ; always, neverのチェックに引っかかったら無視される
          (setq ,pv (ldiff ,form-var ,rest-var))
          (,set-retval-fvar 
           ',pv (combine-each-nth ,valslist-var :n ,maxvals-var))))
      ))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; パターンマッチを利用するユーザ用マクロ

;;; ユーザ用マクロ if-match
(defmacro sct-user:if-match (pattern form then-form &optional else-form)
  (let ((form-var (gensym)))
    `(let ((,form-var ,form))
       (matching-exp (,pattern) ,then-form ,else-form ,form-var))))

;;; ユーザ用マクロ when-match
(defmacro sct-user:when-match (pattern form &body body)
  `(if-match ,pattern ,form (progn ,@body)))

;;; ユーザ用マクロ unless-match
(defmacro sct-user:unless-match (pattern form &body body)
  `(if-match ,pattern ,form nil (progn ,@body)))

;;; ユーザ用マクロ cond-match
(defun mklist-pattern (pattern-list-or-a-pattern)
  (if (pattern-p pattern-list-or-a-pattern)
      (list pattern-list-or-a-pattern)
    pattern-list-or-a-pattern))
(defun remove--> (clause-body)
  (if (symbol= '-> (car clause-body))
      (cdr clause-body)
    clause-body))

(defmacro sct-user:cond-match (keyform &body normal-clauses)
  (let ((form-var (gensym "FM"))
        (block-var (gensym "BL"))
	(shared-retvals-var (gensym "SHARED-RETVALS"))
	(n-of-pv-max 0))
    (let ((converted-nclauses
	   (mapcar
            #'(lambda (clause)
                (cond
                 ((symbol= 'otherwise (car clause))
                  `(return-from ,block-var 
                     (progn ,@(remove--> (cdr clause)))))
                 (t
		  (let ((patterns (mklist-pattern (car clause))))
		    (dolist (pat patterns)
		      (let ((c (count-pattern-variables pat)))
			(when (> c n-of-pv-max) (setq n-of-pv-max c))))
		    `(matching-exp
		      ,patterns
		      (return-from ,block-var
			(progn ,@(remove--> (cdr clause))))
		      nil
		      ,form-var
		      ,shared-retvals-var)))))
            normal-clauses)))
      `(let ((,form-var ,keyform)
	     (,shared-retvals-var ,(match-make-make-array n-of-pv-max)))
	 (declare (ignorable ,form-var ,shared-retvals-var))
	 (block ,block-var		; どこかでマッチ成立ならreturn-from
	   ,.converted-nclauses
	   nil				; 多分不必要だが一応
	   )))))
