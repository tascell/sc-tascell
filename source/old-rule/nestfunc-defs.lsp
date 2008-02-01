;;; nestfunc.rule で使う変数、関数の定義
(provide "NESTFUNC-DEFS")
(in-package "NESTED-FUNCTION")

(scr:require "SC-UTIL")

(defstruct finfo 
  name           ;関数名
  parent-func    ;親関数のfinfo構造体（非nil iff 入れ子関数）
  ret-type       ;返り値の型
  argp           ;argpの要/不要 (=入れ子関数呼び出しの有無)
  ;; (<復帰位置を示すラベル名> . <フレーム復帰用コード>) のリスト(逆順)
  label-list
  ;; ( <symbol> . <texp> )
  ;; frameには var-list と tmp-listの両方を入れる
  ;; （nf-list の orig-name も入れる）
  ;; 中断前にスタックに退避するのはvar-listのみ
  ;; 復帰後、回復するのは var-list と tmp-list(?) の両方
  var-list     ;通常のローカル変数のリスト
  tmp-list     ;一時変数（call関数ポインタ，引数保存用）のリスト
  nf-list      ;定義された入れ子関数のリスト ( <orig-name> . <ext-name> )
  ;; 局所ラベルのリスト(def ,id __label__)で定義。入れ子関数脱出用
  ;; (<ラベル名> . <復帰コード>)。 逆順
  local-label
  ;; 明示的スタックを参照するローカル変数．list of <id-string>
  ;; var-listとは独立．入れ子関数は nf-listで管理するのでここには入れない
  estack-var
  )

;;;; 関数

;;; 関数名を*frame-alist*から探して関数フレームの構造体情報を得る。
;;; 未登録の場合は登録して、登録した構造体情報を返す。
(defun get-frame (x)
  (let* ((strx (identifier-sc2c x))
	 (asc
	  (assoc strx *frame-alist* :test #'string=)))
    (if asc
	(cdr asc)
	(let* ((basename (string+ strx "_frame"))
	       (frame-name (generate-id basename *used-id-list*))
	       (frame-info `(,frame-name )))
	  (push (cons strx frame-info) *frame-alist*)
	  frame-info))))

;;; 関数名=>関数のフレーム構造体名
(defun get-frame-name (fname)
  (car (get-frame fname)))

;;; 現関数（親は含まない）のlocal-variableか？
;;; tmp-list内のもの，入れ子関数は検索対象ではない．
(defun local-variable-p (id &optional (finfo *current-func*))
  (and *current-func*
       (assoc (identifier-sc2c id)
              (finfo-var-list finfo)
              :key #'identifier-sc2c
              :test #'string=)) )

;;; 現関数（親は含まない）の明示的スタックに値があるlocal-variableか？
(defun estack-variable-p (id &optional (finfo *current-func*))
  (and (local-variable-p id finfo)
       (member (identifier-sc2c id)
               (finfo-estack-var finfo)
               :test #'string=) ))
       
;;; 現関数(=0)からいくつ外の関数で定義された local-variable/nestfunc か?
;;; 見つからなければ -1
;;; 第二返り値は local-varriable-> :var, nestfunc-> :nestfunc
;;; 第三返り値は local-variable-p/nestfunc-extid の返り値
(defun howmany-outer (id &optional (finfo *current-func*))
  (labels ((rec (curfunc acc)
             (acond
               ((null curfunc)
                -1)
               ((local-variable-p id curfunc)
                (values acc :var it))
               ((nestfunc-extid id curfunc)
                (values acc :nestfunc it))
               (t (rec (finfo-parent-func curfunc) (1+ acc))) )))
    (rec finfo 0)))

;;; 与えられた関数情報からフレーム構造体の定義を作る
(defun make-frame-decl (fi)
  (let ((frame-name (get-frame-name (finfo-name fi)))
	(member-list (append (finfo-var-list fi)
                             (finfo-tmp-list fi)
			     (mapcar #'(lambda (x)
                                         (cons (car x) 'closure-t))
                                     (finfo-nf-list fi)))))
    `(def (struct ,frame-name)
	  (def tmp-esp (ptr char))    ; これは、絶対に先頭
	  (def argp (ptr char))
	  (def call-id int)
	  ,@(mapcar #'(lambda (x)
                        `(def ,(car x) ,(cdr x)))
                    member-list))))

;;; *current-func*に変数情報を追加して、declaration文を返す
(defun add-local (id texp mode &key (init nil) (finfo *current-func*))
  (let ((lw2ct-texp (lw2ct texp)))
    ;; mode： :var or :temp
    (when finfo
      (case mode
	((:var)
	 (when (let ((ttexp (remove-type-qualifier texp)))
		 (or (and (listp ttexp)
                          (eq 'array (car ttexp)))
                     *all-in-estack*))
	   (pushnew (identifier-sc2c id) (finfo-estack-var finfo)
                    :test #'string=))
	 (push (cons id lw2ct-texp) (finfo-var-list finfo)))
	((:tmp)
         (push (cons id lw2ct-texp) (finfo-tmp-list finfo)))
        ((:system)  ; 何もしない
         )
	(otherwise
	 (error "unexpected value of 'mode'(~s)" mode))))
    (if init
	`(def ,id ,lw2ct-texp ,(Nfe-xfp init))
      `(def ,id ,lw2ct-texp))))

;;; *current-func* に入れ子関数情報を追加
(defun add-nestfunc (id extid &optional (finfo *current-func*))
  (push (cons id extid)
        (finfo-nf-list *current-func*)) )

;;; lightweight => closure-t
(defun lw2ct (texp)
  (if (and texp (listp texp))
      (if (eq 'lightweight (car texp))
	  'closure-t
	(mapcar #'lw2ct texp))
    texp))


;;; 与えられた関数情報から復帰処理を行うstatementを作る
(defun make-resume (fi)
  (unless (or (finfo-label-list fi)
	      (finfo-local-label fi))
    (return-from make-resume
      `((label @LGOTO nil))))
  (let ((reconst-impossible (or (eq 'main (finfo-name fi))
                                (finfo-parent-func fi)
                                *all-in-estack*))
                                        ; スタックの積み直し中に成り得ない
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
    `((if ,(if reconst-impossible
               '0
               'esp-flag)
	  (begin
	   ,@(unless reconst-impossible
	       `( (= esp (cast (ptr char)
                          (bit-xor (cast size-t esp) esp-flag)))
		  (= efp (cast (ptr ,frame-type) esp))
		  (= esp (aligned-add esp (sizeof ,frame-type)))
		  (= (mref-t (ptr char) esp) 0) ))
	   (label @LGOTO
		  (switch (fref (mref efp) call-id) ,@case-goto))
	   ,@(when (finfo-label-list fi)
	       `( (goto ,(caar (last (finfo-label-list fi)))) )))))))

;;; efp(xfp)の設定および espをフレームサイズ分移動させる
(defun make-init-efp-esp (fi)
  (let ((frame-type  `(struct ,(get-frame-name (finfo-name fi)))))
    `((= efp (cast (ptr ,frame-type) esp))
      (= esp (aligned-add esp (sizeof ,frame-type)))
      (= (mref-t (ptr char) esp) 0)
      ,@(when (and *all-in-estack*
                   (finfo-parent-func fi))
          `( (= (fref efp -> xfp) xfp) ))
      )))

;;; （*all-in-estack*時）引数の値をestackに保存
(defun save-args-into-estack (argid-list argtexp-list
                              &optional (finfo *current-func*))
  (when *all-in-estack*
    ;; ちょっと手抜きで型情報なし
    (mapcar #'(lambda (id texp)
                (if (finfo-parent-func finfo)
                    `(= (fref efp -> ,id) (pop-arg ,texp parmp)) 
                    `(= (fref efp -> ,id) ,id) ))
            argid-list
            argtexp-list) ))

;;; 入れ子関数のid -> トップレベルに移した関数のid
(defun make-extid (id &optional (pfinfo *current-func*) (usdid *used-id-list*))
  (generate-id (string+ (identifier-sc2c id) "_in_"
                        (identifier-sc2c (finfo-name pfinfo)))
               usdid))


;;; idが現在の関数（親は除く）内で定義された入れ子関数か？
;;; もしそうなら，ext-name を返す
(defun nestfunc-extid (id &optional (finfo *current-func*))
  (and *current-func*
       (cdr (assoc (identifier-sc2c id)
                   (finfo-nf-list finfo)
                   :key #'identifier-sc2c
                   :test #'string=)) ))
  

;;; 入れ子関数の参照 -> etackへの参照
;;; （pfinfo: 親関数情報）
(defun nestfunc-in-estack (fid &optional (pfinfo *current-func*))
  `(ptr (fref efp -> ,fid)))

;;; 与えられた関数情報から入れ子関数を正規化するコードを作る
(defun make-normalize-nf (&optional (fi *current-func*))
  (let ((nf-list (finfo-nf-list fi)))
    (apply #'nconc
	   (mapcar
	    #'(lambda (x) 
		`( (= (fref efp -> ,(car x) fun)
		      ,(cdr x))
		   (= (fref efp -> ,(car x) fr)
		      (cast (ptr void) efp)) ))
	    nf-list))))

;;; 与えられた関数情報からフレーム情報を保存するコードを作る
(defun make-frame-save (&optional (fi *current-func*))
  (mapcan
   #'(lambda (x)
       (cond ((eq 'closure-t (cdr x))
	      nil)
	     (t
	      `( (= (fref efp -> ,(car x)) ,(car x)) ))
	     ))
   (remove-if #'(lambda (x)
		  (member (identifier-sc2c (car x)) (finfo-estack-var fi)
			  :test #'string=))
	      (finfo-var-list fi))))

;;; 与えられた関数情報からフレーム情報を復活するコードを作る
(defun make-frame-resume (&optional (fi *current-func*))
  (mapcar
   #'(lambda (x) 
       `(= ,(car x) (fref efp -> ,(car x))))
   (remove-if #'(lambda (x)
		  (member (identifier-sc2c (car x)) (finfo-estack-var fi)
			  :test #'string=))
	      (finfo-var-list fi))))

;;; 与えられた関数情報から関数中断用のreturnを生成するコードを作る
(defun make-suspend-return (&optional (fi *current-func*))
  (cond ((finfo-parent-func fi)
	 ;;`(return (fref efp -> tmp-esp)))
	 (error "make-suspend-return called in lightweight-func"))
	((eq 'void (finfo-ret-type *current-func*))
	 '(return))
	(t
	 `(return (SPECIAL ,(finfo-ret-type *current-func*))))))

;;; 与えられたラベル名が、親関数の局所ラベルとして定義されているか調べる。
;;; 定義されていなければ,返り値はnil。定義されていれば、
;;; (values <自分からみて何番目の親関数に定義されていたか>
;;;         <その定義 ( <label> . <復帰処理> )> 
;;;         <そのラベルが関数内で何番目にpushされたものか>)
(defun find-local-label (lid fi &aux (lids (identifier-sc2c lid)))
  (labels ((find-local-label-tail (cfi acc &aux memb)
	     (cond ((null cfi)
		     nil)
		   ((let* ((memb0 (member 
				   lids
				   (finfo-local-label cfi)
				   :test #'string=
				    :key #'(lambda (x)
					    (identifier-sc2c (car x))))))
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
;      (,@r1-2 ... ,@rn-2) 
;      nil
;      (,@prev-4th ,@r1-3 ,r1-4 ... ,@rn-3 ,rn-4 ) )
(defun combine-ret-list (ret-list &optional prev-4th)
  (let ((fst (mapcar #'first ret-list))
	(scd (mapcar #'second ret-list))
	(thd-4th (mapcar
		  #'(lambda (x) `(,@(third x) ,(fourth x)))
		   ret-list)))
    (list (apply #'append fst)
	  (apply #'append scd)
	  nil
	  (remove nil (apply #'append prev-4th thd-4th)))))

; identifier, constant, string-literal
; ==> nil以外  (いちいち一時変数を使う必要がないことが明らかな式)
(defun simple-exp-p (x)
  (or
   ;;(identifier-sc2c x)
   (par-constant x)
   (par-string-literal x)))

