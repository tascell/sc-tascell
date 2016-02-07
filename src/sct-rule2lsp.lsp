;;; Copyright (c) 2008 Tasuku Hiraishi <hiraisi@kuis.kyoto-u.ac.jp>
;;; All rights reserved.

;;; Redistribution and use in source and binary forms, with or without
;;; modification, are permitted provided that the following conditions
;;; are met:
;;; 1. Redistributions of source code must retain the above copyright
;;;    notice, this list of conditions and the following disclaimer.
;;; 2. Redistributions in binary form must reproduce the above copyright
;;;    notice, this list of conditions and the following disclaimer in the
;;;    documentation and/or other materials provided with the distribution.

;;; THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
;;; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;;; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;;; ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
;;; FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;;; DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
;;; OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
;;; HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
;;; LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
;;; OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
;;; SUCH DAMAGE.

;;; sct-rule2lsp: Pattern matcher

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
  (declare (type :pattern pat))
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
  (declare (type :pattern pat))
  (length (get-pattern-variables pat)))

;; パターンから,@の場所を捜す
(defun search-commaat (pbody)
  (member-if #'pat-commaat-p pbody))

;; patternがnilにマッチするか
(defun matches-to-nil-p (pbody)
  (and (consp pbody)
       (every #'pat-commaat-p pbody)))

;; パターンを,@より前とそれ以降で分割
(defun split-at-commaat (pattern)
  (list-until pattern (search-commaat pattern)))

;; ,pv[]や,@pv[]における[]の中身==>マッチ成立を判定するフォーム
;; マッチング成立ならsave-placeにget-retvalで獲得するための multiple-value-list
;; をセットする（不成立ならsave-placeの値は保証しなくてよい）
;; 返す値はマッチの成否を表してさえいれば特にretvalでなくてよい
(defun bracket-to-testform (in-bracket argform saveplace)
  (cond
   ((cdr in-bracket)
    (case (car in-bracket)              ; :evalに続くフォームの値が非nilならマッチ
      ((:eval)
       `(car (setf ,saveplace
               (multiple-value-list (progn ,@(cdr in-bracket))))) )
      ((:member)                        ; :memberに続く値（リスト）に含まれていればマッチ
       `(car (setf ,saveplace
               (list (member ,argform ,(second in-bracket) :test #'equal)))) )
      (otherwise
       (error "Unexpected form: [~{~S~^ ~}]" in-bracket))))
   ((null in-bracket)                   ; ,var[] == ,var（常にマッチ）
    `(setf ,saveplace '(t)))
   ((symbolp (car in-bracket))          ; ,var[symbol] --> (funcall #'var symbol)
    (unless (eq (symbol-package (car in-bracket))
                (find-package "RULE"))
      (warn ",~0@*~S[~1@*~S]: ~1@*~S may not be rule-name.~%~
             In order to test whether (~1@*~S ~0@*~S) is non-nil, write ,~0@*~S[#'~1@*~S]."
            argform (car in-bracket)))
    `(not (eq 'rule::no-match           ; test if non-rule::no-match
              (car (setf ,saveplace
                     (multiple-value-list (,(car in-bracket) ,argform))))) ) )
   ((atom (car in-bracket))             ; ,var[atom] --> (equal atom symbol)
    `(car (setf ,saveplace
            (list (equal ,(car in-bracket) ,argform)))) )
   ((eq 'function (caar in-bracket))    ; ,var[#'<form>] --> (funcall #'<form> symbol)
    `(car (setf ,saveplace              ; test if non-nil
            (multiple-value-list (funcall ,(car in-bracket) ,argform)))) )
   (t                                   ; atomでも関数フォームでもない --> :member
    (bracket-to-testform `(:member ,(car in-bracket)) argform saveplace))))

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
;;; 補助関数 -->


;; (<pattern> . <action>) のリストとform-varを受けとって
;; form-varの値が<pattern>にマッチすれば
;; <action>を実行するようなフォームを生成
;; 先頭の共通する部分の処理を一回ですませるようにしている
;; :action のところの gensym は全ての:actionが equalにならないようにして，
;; 同じactionでも統合されないようにするためのダミー．
;; （同一パターン・アクションでもnext-pattern マクロの動作を保証するため）
(defun match-action-form (pats-act-list form-var &optional otherwise-action)
  (match-check-to-form
   (integrate-ml-actions
    (mapcar #'(lambda (x) (nconc (pattern-matching-list (car x))
                                 (list `(:action ,(gensym "ACT") ,(cdr x)))))
            (extract-pats-act pats-act-list)))
   (list form-var)
   otherwise-action)
  )

(defun extract-pats-act (pats-act-list)
  (loop for (pats . act) in pats-act-list
      nconc (loop for pat in (mapcan #'extract-optional pats)
                collect (cons pat act))))


;;;;;;;;;;;;

;; match-check-to-formの変換関数で利用する
;; (<pattern-variable> . <return-value-varialble>) のリスト
(defvar *pv-list*)
;; マッチング処理全体を包むブロック名．action終了後脱出するために使う
(defvar *match-block*)

(defun get-pv-ret (pv &optional (pv-list *pv-list*))
  (let ((pv-ret (cdr (assoc pv pv-list))))
    (assert (and pv-ret (symbolp pv-ret)))
    pv-ret))

(defun get-focus-var (focus-stk)
  (let ((var (car focus-stk)))
    (assert (and var (symbolp var)))
    var))

(defun get-tmp-var (rsvars)
  (get-focus-var rsvars))

(defun get-escape-var (escape-stk)
  (let ((var (car escape-stk)))
    (assert (and var (symbolp var)))
    var))

(defun flatten-block (form)
  (if (tagged-p 'progn form)
      (cdr form)
    (list form)))

;;;
(defun match-check-to-form (mc focus-stk &optional otherwise-action)
  (let ((*pv-list* ())
        (*match-block* (gensym "MBLOCK"))
        (tag-otherwise (gensym "OTW")))
    `(block ,*match-block*
       (tagbody
         ,@(flatten-block (match-check-list-to-form (list mc) focus-stk (list tag-otherwise) ()))
         ;;どれかにマッチしたならここまでにreturn-fromしているので
         ;;go以外ではここにはこない
         ,tag-otherwise
         ,@(when otherwise-action
             (list `(return-from ,*match-block* ,otherwise-action)))))
    ))

(defun match-check-list-to-form (mc-list focus-stk escape-stk rsvars
                                 &aux (mc0 (car mc-list)) (mc-rest (cdr mc-list)))
  (cond
   ((endp mc-list) nil)
   ((atom mc0)
    (case mc0
      ((nil)      nil)
      ((:nil)     (match-check-list-to-form-judge (tagger 'null) mc-rest focus-stk escape-stk rsvars))
      ((:non-nil) (match-check-list-to-form-judge #'identity mc-rest focus-stk escape-stk rsvars))
      ((:listp)
       (cond 
        ((eq :non-nil (car mc-rest))
         (match-check-list-to-form-judge (tagger 'consp) (cdr mc-rest) focus-stk escape-stk rsvars))
        ((eq :nil (car mc-rest))
         (match-check-list-to-form mc-rest focus-stk escape-stk rsvars))
        (t
         (match-check-list-to-form-judge (tagger 'listp) mc-rest focus-stk escape-stk rsvars))))
      ((:car) (match-check-list-to-form-move (tagger 'car) mc-rest focus-stk escape-stk rsvars))
      ((:cdr) (match-check-list-to-form-move (tagger 'cdr) mc-rest focus-stk escape-stk rsvars))
      ((:push-focus) (match-check-list-to-form-push-focus mc-rest focus-stk escape-stk rsvars))
      ((:pop-focus)  (match-check-list-to-form-pop-focus  mc-rest focus-stk escape-stk rsvars))
      (otherwise
       (error "unknown check tag: ~S" mc0))))
   (t
    (case (car mc0)
      ((:br)     (assert (endp mc-rest)) (match-check-to-form-br mc0 focus-stk escape-stk rsvars))
      ((:action) (assert (endp mc-rest)) (match-check-to-form-action mc0 focus-stk escape-stk rsvars))
      ((:eq)    (match-check-list-to-form-judge
                 #'(lambda (x) `(eq ,x ',(second mc0))) mc-rest focus-stk escape-stk rsvars))
      ((:equal) (match-check-list-to-form-judge
                 #'(lambda (x) `(equal ,x ',(second mc0))) mc-rest focus-stk escape-stk rsvars))
      ((:type)
       (cond
        ((and (consp (car mc-rest))
              (member (caar mc-rest) '(:eq :equal)))
         (match-check-list-to-form mc-rest focus-stk escape-stk rsvars))
        (t
         (match-check-list-to-form-judge
          #'(lambda (x) `(typep ,x ',(second mc0))) mc-rest focus-stk escape-stk rsvars))))
      ((:new-pv)  (match-check-list-to-form-new-pv (second mc0) mc-rest focus-stk escape-stk rsvars))
      ((:pv)      (match-check-list-to-form-pv      mc0 mc-rest focus-stk escape-stk rsvars))
      ((:pv-rest) (match-check-list-to-form-pv-rest mc0 mc-rest focus-stk escape-stk rsvars))
      ((:pv-*)     (match-check-list-to-form-pv-*     mc0 mc-rest focus-stk escape-stk rsvars))
      ((:postpv-*) (match-check-list-to-form-postpv-* mc0 mc-rest focus-stk escape-stk rsvars))
      (otherwise
       (error "unknown check tag: ~S" mc0))))
   ))

(defun match-check-list-to-form-judge (focus2testform mc-rest focus-stk escape-stk rsvars)
  (let ((focus-var (get-focus-var focus-stk))
        (escape-var (get-escape-var escape-stk)))
    `(if ,(funcall focus2testform focus-var)
         ,(match-check-list-to-form mc-rest focus-stk escape-stk rsvars)
       (go ,escape-var))
    ))

(defun match-check-list-to-form-move (focus2move mc-rest focus-stk escape-stk rsvars)
  (let ((focus-var (get-focus-var focus-stk)))
    `(progn (setq ,focus-var ,(funcall focus2move focus-var))
            ,@(flatten-block (match-check-list-to-form mc-rest focus-stk escape-stk rsvars)))
    ))

(defun match-check-list-to-form-push-focus (mc-rest focus-stk escape-stk rsvars)
  (let ((focus-var (get-focus-var focus-stk)))
    (if rsvars
        (let ((tmp-var (get-tmp-var rsvars)))
          `(progn
             (setq ,tmp-var ,focus-var)
             ,@(flatten-block (match-check-list-to-form mc-rest (cons tmp-var focus-stk) escape-stk
                                                        (cdr rsvars))))
          )
      (with-fresh-variables new-tmp-var
        `(let ((,new-tmp-var ,focus-var))
           ,@(flatten-block (match-check-list-to-form mc-rest (cons new-tmp-var focus-stk) escape-stk
                                                      rsvars)))
        ))))

(defun match-check-list-to-form-pop-focus (mc-rest focus-stk escape-stk rsvars)
  (let ((focus-var (get-focus-var focus-stk)))
    (match-check-list-to-form mc-rest (cdr focus-stk) escape-stk
                              (cons focus-var rsvars))))

;; branch
(defun match-check-to-form-br (mc focus-stk escape-stk rsvars)
  ;; (:br <path1> <path2>)
  (destructuring-bind (tag path1 path2) mc
    (assert (eq :br tag))
    (let ((fcbak-vars (mapcar #'(lambda (x) (gensym (string+ (symbol-name x) "BAK")))
                              focus-stk))
          (else-tag (gensym "ELSE")))
      (if path2
          `(let (,.(loop for fv in focus-stk as bk in fcbak-vars
                       collect (list bk fv)))
             (tagbody
               ,@(let ((*pv-list* *pv-list*))
                   (flatten-block
                    (match-check-list-to-form path1 focus-stk (cons else-tag escape-stk) rsvars)))
               (error "Unexpected control flow: probably bugs in match-check-to-form.")
               ,else-tag
               ,.(loop for fv in focus-stk as bk in fcbak-vars
                     collect `(setq ,fv ,bk))
               ,@(flatten-block (match-check-to-form-br path2 focus-stk escape-stk rsvars))))
        (match-check-list-to-form path1 focus-stk escape-stk rsvars))
      )))

(defun match-check-to-form-action (mc focus-stk escape-stk rsvars)
  (declare (ignore focus-stk rsvars))
  (let ((escape-var (get-escape-var escape-stk)))
    (destructuring-bind (tag dummy form) mc
      (declare (ignore dummy))
      (assert (eq :action tag))
      `(macrolet ((sct-user:pattern-variable-p (sym)
                    ;; (if (quoted-p sym)    ; only for compatibility
                    ;;     `(sct-user:pattern-variable-p ,(second sym))
                    ;;   (progn
                    (assert (symbolp sym))
                    (when (assoc sym ',*pv-list*) t))
                  (sct-user:if-pattern-variable (sym then-form &optional else-form)
                    (assert (symbolp sym))
                    (if (assoc sym ',*pv-list*) then-form else-form))
                  (sct-user:get-retval (sym)
                    ;; (if (quoted-p sym)    ; only for compatibility
                    ;;     `(sct-user:get-retval ,(second sym))
                    ;;   (progn
                    (assert (symbolp sym))
                    `(values-list ,(cdr (assoc sym ',*pv-list*))))
                  (sct-user:next-pattern ()
                    '(go ,escape-var))
                  )
         (declare (ignorable (function sct-user:pattern-variable-p)
                             (function sct-user:get-retval)))
         (return-from ,*match-block* ,form))
      )))

(defun match-check-list-to-form-new-pv (pv mc-rest focus-stk escape-stk rsvars)
  (with-fresh-variables (pv-ret)
    (push (cons pv pv-ret) *pv-list*)
    `(let (,pv ,pv-ret)
       (declare (ignorable ,pv ,pv-ret))
       ,@(flatten-block
          (match-check-list-to-form mc-rest focus-stk escape-stk rsvars)))
    ))

(defun match-check-list-to-form-pv (mc0 mc-rest focus-stk escape-stk rsvars)
  (destructuring-bind (tag pv in-bracket) mc0
    (assert (eq :pv tag))
    (let ((pv-ret (get-pv-ret pv))
          (focus-var (get-focus-var focus-stk))
          (escape-var (get-escape-var escape-stk)))
      `(progn
         (setq ,pv ,focus-var)
         ,@(if in-bracket
               (list `(if ,(bracket-to-testform in-bracket pv pv-ret)
                          ,(match-check-list-to-form mc-rest focus-stk escape-stk rsvars)
                        (go ,escape-var)))
             (flatten-block (match-check-list-to-form mc-rest focus-stk escape-stk rsvars))))
      )))

(defun match-check-list-to-form-pv-rest (mc0 mc-rest focus-stk escape-stk rsvars)
  (destructuring-bind (tag pv rest-len in-bracket) mc0
    (assert (eq :pv-rest tag))
    (let ((pv-ret (get-pv-ret pv))
          (focus-var (get-focus-var focus-stk))
          (escape-var (get-escape-var escape-stk)))
      (with-fresh-variables (pred-var head-var succ-var clct-var rclct-var rtmp-var maxvals-var)
        `(let ((,succ-var ,focus-var)
               ,.(when in-bracket (list rtmp-var)))
           ,.(loop for i of-type fixnum from 1 upto rest-len
                 if (= i rest-len)
                 nconc (list `(when (endp ,succ-var) (go ,escape-var))
                             `(setq ,succ-var (cdr ,succ-var)))
                 else
                 nconc (list `(setq ,succ-var (cdr ,succ-var))))
           (loop :for ,pred-var :on ,focus-var
             :as ,head-var := (car ,pred-var)
             :until (endp ,succ-var)
             :collecting ,head-var :into ,clct-var
             ,.(when in-bracket
                 (list :do `(unless ,(bracket-to-testform in-bracket head-var rtmp-var)
                              (go ,escape-var))
                       :collecting rtmp-var :into rclct-var
                       :maximizing `(length ,rtmp-var) :into maxvals-var))
             :do (setq ,succ-var (cdr ,succ-var))
             :finally
             (setq ,pv ,clct-var)
             (setq ,focus-var ,pred-var)
             ,.(when in-bracket
                 (list `(setq ,pv-ret (when ,rclct-var (combine-each-nth ,rclct-var :n ,maxvals-var))))))
           ,@(flatten-block (match-check-list-to-form mc-rest focus-stk escape-stk rsvars)))
        ))))

(defun match-check-list-to-form-pv-* (mc0 mc-rest focus-stk escape-stk rsvars)
  (destructuring-bind (tag pv in-bracket mc-list) mc0
    (assert (eq :pv-* tag))
    (let ((pv-ret (get-pv-ret pv))
          (focus-var (get-focus-var focus-stk))
          (escape-var (get-escape-var escape-stk)))
      (with-fresh-variables (retry-tag skip-tag cur-var head-var rtmp-var nvals-var maxvals-var)
        `(let ((,cur-var ,focus-var) ,head-var
               ,.(when in-bracket (list rtmp-var `(,maxvals-var 0))))
           ,.(when in-bracket (list `(declare (fixnum ,maxvals-var))))
           (setq ,pv nil)
           (setq ,pv-ret nil)
           (tagbody
             (go ,skip-tag)
             ,retry-tag
             (when (endp ,cur-var) (go ,escape-var))
             (setq ,head-var (car ,cur-var))
             ,.(when in-bracket
                 (list `(unless ,(bracket-to-testform in-bracket head-var rtmp-var)
                          (go ,escape-var))
                       `(let ((,nvals-var (length ,rtmp-var)))
                          (declare (fixnum ,nvals-var))
                          (when (> ,nvals-var ,maxvals-var)
                            (setq ,maxvals-var ,nvals-var)))
                       `(push ,rtmp-var ,pv-ret)))
             (push ,head-var ,pv)
             (setq ,cur-var (cdr ,cur-var))
             (setq ,focus-var ,cur-var)
             ,skip-tag
             ,@(flatten-block (match-check-list-to-form
                               (append mc-list
                                       `((:postpv-* ,pv ,.(when in-bracket (list pv-ret maxvals-var))))
                                       mc-rest)
                               focus-stk (cons retry-tag escape-stk) rsvars))))
        ))))

(defun match-check-list-to-form-postpv-* (mc0 mc-rest focus-stk escape-stk rsvars)
  (destructuring-bind (tag pv &optional pv-ret maxvals-var) mc0
    (assert (eq :postpv-* tag))
    `(progn
       (setq ,pv (nreverse ,pv))
       ,.(when pv-ret
           (assert (symbolp pv-ret))
           (assert (and maxvals-var (symbolp maxvals-var)))
           (list `(setq ,pv-ret (nreverse ,pv-ret))
                 `(setq ,pv-ret (combine-each-nth ,pv-ret :n ,maxvals-var))))
       ,@(flatten-block (match-check-list-to-form mc-rest focus-stk (cdr escape-stk) rsvars)))
    ))

;;;;;;;;;;;;


;; (,@<match-check-list> (:action ,<dummy> ,<action>)) のリストを受けとり
;; <match-check-list> の 先頭共通部分を統合することで分岐構造にする．
;; (:br <path1> <path2>)
;; path<n>は :car, (:new-pv ..)，(:br ...)，(:action ..) などからなるリスト
;; path1の途中で失敗したら最新の:brまで引き返してpath2に移る
(defun integrate-ml-actions (ml-action-list)
  (cond
   ((endp ml-action-list) ())
   ((endp (car ml-action-list)) (integrate-ml-actions (cdr ml-action-list)))
   (t
    ;; 先頭共通部分が1つでもある部分(succ)と，それ以降(rest)を分ける
    (multiple-value-bind (succ rest)
        (let ((ml0-head (caar ml-action-list)))
          (list-until-if #'(lambda (rem) (not (equal (caar rem) ml0-head)))
                         ml-action-list))
      ;; succのmatching-listの最長の先頭共通部分headを抜き出して，
      ;; 残った部分をrest2に
      (multiple-value-bind (head rest2)
          (apply #'head-intersection #'equal succ)
        `(:br (,.head ,@(flatten-branch (integrate-ml-actions rest2)))
              ,(integrate-ml-actions rest))
          )))))

(defun flatten-branch (br)
  (cond
    ((null br) ())
    ((and (consp br)
          (eq :br (car br))
          (null (third br)))
     (second br))
    (t (list br))))

;; patternにマッチするために必要な条件のリストを返す
(defun match-check-list (pat)
  (declare (type :pattern pat))
  (match-check-list-for-exp (pattern-body pat)))

(defun match-check-list-for-exp (pbody) 
  ;; リストの要素に対して
  (cond
   ((symbolp pbody) (list `(:type ,(type-of pbody)) `(:eq ,pbody)))
   ((atom pbody)    (list `(:type ,(type-of pbody)) `(:equal ,pbody)))
   ((pat-comma-p pbody)
    (let ((pv (second pbody))
          (in-bracket (cddr pbody)))    ; [...]
      (list 
       `(:new-pv ,pv)
       `(:pv ,pv ,in-bracket))))
   ((pat-commaat-p pbody)
    (error "Pattern consists of only ,@<symbol>: ~A" 
           (make-pattern :body pbody)))
   (t
    (match-check-list-for-list pbody))))

(defun match-check-list-for-list (pbody)
  ;; リストに対して
  (multiple-value-bind (until-commaat from-commaat) ; パターンを,@前後で分割
      (split-at-commaat pbody)
    (declare (list until-commaat from-commaat))
    (list*
     :listp
     (nconc
      ;; ,@以前の各要素をチェック
      (loop :for x :in until-commaat
        :nconc (nconc (list :non-nil :push-focus :car)
                      (match-check-list-for-exp x)
                      (list :pop-focus :cdr)))
      ;; ,@以降をチェック
      (if from-commaat
          (match-check-list-for-commaat from-commaat)
        (list :nil))))
    ))

(defun match-check-list-for-commaat (pbody)
  (assert (pat-commaat-p (car pbody)))
  ;; ,@から始まるリストに対して
  (cond
   ((not (search-commaat (cdr pbody)))  ;,@がもうないときは高速化
    (let ((rest-len (length (cdr pbody))) ; ,@を除いたパターンの長さ
          (pv (second (car pbody)))
          (in-bracket (cddr (car pbody))))
      (list*
       ;; ,@部分をチェック（要素数確定）
       `(:new-pv ,pv)
       `(:pv-rest ,pv ,rest-len ,in-bracket)
       ;; ,@より後のパターンをチェック
       (match-check-list-for-list (cdr pbody)) )
      ))
   (t                                   ; ,@がまだある場合=>しらみつぶし
    (let ((pv (second (car pbody)))
          (in-bracket (cddr (car pbody))))
      (list
       `(:new-pv ,pv)
       `(:pv-* ,pv ,in-bracket
               ,(match-check-list-for-list (cdr pbody))))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; パターンマッチを利用するユーザ用マクロ

;;; ユーザ用マクロ if-match
(defmacro sct-user:if-match (pattern form then-form &optional else-form)
  (with-fresh-variables form-var
    `(let ((,form-var ,form))
       ,(match-action-form
         `((,(list pattern) . ,then-form))
         form-var
         else-form))))

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
  (with-fresh-variables form-var
    (let* ((otherwise-action)
           (pats-act-list               ; list of (<patterns> . <action>)
            (loop for clause in normal-clauses
                if (symbol= 'otherwise (car clause))
                do
                  (setq otherwise-action `(progn ,@(remove--> (cdr clause))))
                  (loop-finish)
                else
                collect
                (let ((patterns (mklist-pattern (car clause)))
                      (action `(progn ,@(remove--> (cdr clause)))))
                  (cons (mapcan #'extract-optional patterns)
                        action)))
            ))
      `(let ((,form-var ,keyform))
         (declare (ignorable ,form-var))
         ,(match-action-form pats-act-list form-var otherwise-action))
      )))
