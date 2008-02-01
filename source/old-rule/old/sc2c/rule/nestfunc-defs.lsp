;;; nestfunc.rule で使う変数、関数の定義
(provide "NESTFUNC-DEFS")
(in-package "NESTFUNC")

(require "SC-UTIL")
(use-package "SC-TRANSFORMER")
(use-package "SC-MISC")
(use-package "SC-UTIL")

; 使用済みidentifierのリスト
(defvar *used-id-list* nil)
; 宣言のみの関数のリスト
(defvar *decl-only-list* nil)

; 関数フレームの構造体情報
; ( <(元のソースでの)関数名> . <構造体情報> ) のリスト
; <構造体情報> := ( <構造体名> ) 
(defvar *frame-alist* nil)

; 現在いる関数の情報
(defvar *current-func* nil)
(defstruct finfo 
  name           ;関数名
  parent-func    ;親関数のfinfo構造体への参照
  ret-type       ;返り値の型
  argp           ;argpの要/不要 (=入れ子関数呼び出しの有無)
  ;;(<復帰位置を示すラベル名> . <フレーム復帰用コード>) のリスト(逆順)
  label-list
  ;; ( <symbol> . <texp> )
  ;; frameには var-list と tmp-listの両方を入れる
  ;; （nf-list の int-name も入れる）
  ;; 中断前にスタックに退避するのはvar-listのみ
  ;; 復帰後、回復するのは var-list と tmp-list(?) の両方
  var-list       ;通常のローカル変数のリスト
  tmp-list       ;一時変数のリスト
  nf-list        ;定義された入れ子関数のリスト ( <int-name> . <ext-name> )
  ;; 局所ラベルのリスト(def ,id __label__)で定義。入れ子関数脱出用
  ;; (<ラベル名> . <復帰コード>)。 逆順
  local-label
  ;; 通常関数内でもestackで扱うローカル変数(<id-string>)
  estack-var
  )

;estack[]のサイズ
(defconstant *estack-size* 65536)

;;;; 関数

; 関数名を*frame-alist*から探して関数フレームの構造体情報を得る。
; 未登録の場合は登録して、登録した構造体情報を返す。
(defun get-frame (x)
  (let* ((strx (par-identifier x))
	 (asc
	  (assoc strx *frame-alist* :test #'string=)))
    (if asc
	(cdr asc)
	(let* ((basename (string+ strx "_frame"))
	       (frame-name (generate-id basename *used-id-list*))
	       (frame-info `(,frame-name )))
	  (push (cons strx frame-info) *frame-alist*)
	  frame-info))))

; 関数名=>関数のフレーム構造体名
(defun get-frame-name (fname)
  (car (get-frame fname)))

; 与えられた関数情報からフレーム構造体の定義を作る
(defun make-frame-decl (fi)
  (let ((frame-name (get-frame-name (finfo-name fi)))
	(member-list (append (finfo-var-list fi) (finfo-tmp-list fi)
			     (mapcar #'(lambda (x)
					 (cons (car x) 'closure-t))
				     (finfo-nf-list fi)))))
    `(def (struct ,frame-name)
	  (def tmp-esp (ptr char))    ;; これは、絶対に先頭
	  (def argp (ptr char))
	  (def call-id int)
	  ,@(mapcar
	     #'(lambda (x)
		 `(def ,(car x) ,(cdr x)))
	     member-list))))

; *current-func*に変数情報を追加して、declaration文を返す
(defun add-local (id texp mode &key  (init nil) (finfo *current-func*))
  (let ((lw2ct-texp (lw2ct texp)))
    ; mode： :var or :temp
    (when (and finfo mode)
      (case mode
	((:var)
	 (when (let ((ttexp (remove-type-qualifier texp)))
		 (and (listp ttexp)
		      (eq 'array (car ttexp))))
	   (push (par-identifier id) (finfo-estack-var finfo)))
	 (push (cons id lw2ct-texp) (finfo-var-list finfo)))
	((:tmp)
       (push (cons id lw2ct-texp) (finfo-tmp-list finfo)))
	(otherwise
	 (error "unexpected value of 'mode'(~s)" mode))))
    (if init
	`(def ,id ,lw2ct-texp ,init)
      `(def ,id ,lw2ct-texp))))

; lightweight => closure-t
(defun lw2ct (texp)
  (if (and texp (listp texp))
      (if (eq 'lightweight (car texp))
	  'closure-t
	(mapcar #'lw2ct texp))
    texp))


; 与えられた関数情報から復帰処理を行うstatementを作る
(defun make-resume (fi)
  (unless (or (finfo-label-list fi)
	      (finfo-local-label fi))
    (return-from make-resume
      `((label @LGOTO nil))))
  (let ((main-p (eq 'main (finfo-name fi)))
	(case-goto
	 (append
	  ;; 入れ子関数呼び出し終了後の復帰
	  (do ((ret nil)
	       (k 0 (1+ k))
	       (lb (reverse (finfo-label-list fi)) (cdr lb)))
	      ((endp lb) (apply #'append (nreverse ret)))
	    (push `((case ,k)
		   ,@(cdar lb) 
		    (goto ,(caar lb)))
		   ret))
	  ;; gotoによる入れ子関数から親関数への脱出先
	  (do ((ret nil)
	       (k -1 (1- k))
	       (lb (reverse (finfo-local-label fi)) (cdr lb)))
	      ((endp lb) (apply #'append (nreverse ret)))
	    (push `((case ,k)
		    ,@(cdar lb) 
		    (goto ,(caar lb)))
		   ret))))
	(frame-type `(struct ,(get-frame-name (finfo-name fi)))))
    `((if ,(if main-p '0 'esp-flag)
	  (begin
	   ,@(unless main-p
	       `( (bit-xor= (cast size-t esp) esp-flag)
		  (= efp (cast (ptr ,frame-type) esp))
		  (= esp (aligned-add esp (sizeof ,frame-type)))
		  (= (mref-t (ptr char) esp) 0) ))
;;;	   ,@(when (finfo-parent-func fi)
;;;		   `( (if (and (bit-and esp-flag 2)
;;;			       (>= (fref efp -> call-id) 0))
;;;			  (return (cast (ptr char) 2)))))
	   (label @LGOTO
		  (switch (fref (mref efp) call-id) ,@case-goto))
	   ,@(when (finfo-label-list fi)
	       `( (goto ,(caar (last (finfo-label-list fi)))) )))))))

; 与えられた関数情報からefp,esp,new-espの初期化処理を行うstatementを作る。
(defun make-init (fi)
  (let ((frame-type  `(struct ,(get-frame-name (finfo-name fi)))))
    `((= efp (cast (ptr ,frame-type) esp))
      (= esp (aligned-add esp (sizeof ,frame-type)))
      (= (mref-t (ptr char) esp) 0)
      )))

; 与えられた関数情報から入れ子関数を正規化するコードを作る
(defun make-normalize-nf (fi)
  (let ((nf-list (finfo-nf-list fi)))
    (apply #'nconc
	   (mapcar
	    #'(lambda (x) 
		`( (= (fref efp -> ,(car x) fun)
		      ,(cdr x))
		   (= (fref efp -> ,(car x) fr)
		      (cast (ptr void) efp)) ))
	    nf-list))))

;; 与えられた関数情報からフレーム情報を保存するコードを作る
(defun make-frame-save (fi)
  (mapcan
   #'(lambda (x)
       (cond ((eq 'closure-t (cdr x))
	      nil)
	     (t
	      `( (= (fref efp -> ,(car x)) ,(car x)) ))
	     ))
   (remove-if #'(lambda (x)
		  (member (par-identifier (car x)) (finfo-estack-var fi)
			  :test #'string=))
	      (finfo-var-list fi))))

;; 与えられた関数情報からフレーム情報を復活するコードを作る
(defun make-frame-resume (fi)
  (mapcar
   #'(lambda (x) 
       `(= ,(car x) (fref efp -> ,(car x))))
   (remove-if #'(lambda (x)
		  (member (par-identifier (car x)) (finfo-estack-var fi)
			  :test #'string=))
	      (finfo-var-list fi))))

;; 与えられた関数情報から関数中断用のreturnを生成するコードを作る
(defun make-suspend-return (fi)
  (cond ((finfo-parent-func fi)
	 ;;`(return (fref efp -> tmp-esp)))
	 (error ("make-suspend-return called in lightweight-func")))
	((eq 'void (finfo-ret-type *current-func*))
	 '(return))
	(t
	 `(return (SPECIAL ,(finfo-ret-type *current-func*))))))

;; 与えられたラベル名が、親関数の局所ラベルとして定義されているか調べる。
;; 定義されていなければ,返り値はnil。定義されていれば、
;; (values <自分からみて何番目の親関数に定義されていたか>
;;         <その定義 ( <label> . <復帰処理> )> 
;;         <そのラベルが関数内で何番目にpushされたものか>)
(defun find-local-label (lid fi &aux (lids (par-identifier lid)))
  (labels ((find-local-label-tail (cfi acc &aux memb)
	     (cond ((null cfi)
		     nil)
		   ((let* ((memb0 (member 
				   lids
				   (finfo-local-label cfi)
				   :test #'string=
				    :key #'(lambda (x)
					    (par-identifier (car x))))))
		      (setq memb memb0))
		    (values acc (car memb) (length memb)))
		   (t
		    (find-local-label-tail
		     (finfo-parent-func cfi) (1+ acc))))))
    (find-local-label-tail fi 0)))

; beginとかの(Nfb0 body)の返り値から、
; beginそのものの返り値を作る。
; ((r1-1 r1-2 r1-3 r1-4) ... (rn-1 rn-2 rn-3 rn-4))
; => ( (,@r1-1 ... ,@rn-1)
;      nil nil
;      (,@prev-4th ,@r1-2 ... ,@rn-2 ,@r1-3 ,r1-4 ... ,@rn-3 ,rn-4 ) )
(defun combine-ret-list (ret-list &optional prev-4th)
  (let ((fst (mapcar #'first ret-list))
	(scd (mapcar #'second ret-list))
	(thd-4th (mapcar
		  #'(lambda (x) `(,@(third x) ,(fourth x)))
		   ret-list)))
    (list (apply #'append fst)
	  nil
	  nil
	  (remove nil (apply #'append prev-4th 
			      (apply #'append scd)
			      thd-4th)))))

; identifier, constant, string-literal
; ==> nil以外  (いちいち一時変数を使う必要がないことが明らかな式)
(defun simple-exp-p (x)
  (or
   ;;(par-identifier x)
   (par-constant x)
   (par-string-literal x)))

