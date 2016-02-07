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

;;; Utilities for nestfunc.rule
(provide "NESTFUNC-DEFS")

(defpackage "NESTED-FUNCTION"
  (:nicknames "LW" "NESTFUNC")
  (:export :*frame-alist* :*current-func* :*estack-size* :*all-in-estack*
           :make-finfo :top-level-p :finfo-name :finfo-parent-func :finfo-ret-type
           :finfo-argp :finfo-label-list :finfo-var-list :finfo-tmp-list
           :finfo-nf-list :finfo-estack-var
           :with-nestfunc-environment
           :add-global-func
           :thread-origin-p :get-frame :get-frame-name
           :estack-variable-p :static-variable-p :howmany-outer
           :make-frame-def :make-all-frame-decls
           :with-external-decl :add-static-var-def :flush-static-var-defs
           :add-frame-def :add-nestfunc-def :flush-frame-defs :flush-nestfunc-defs
           :with-new-block :add-local-decl :flush-local-decls
           :with-block-item :add-precedent :flush-precedents
           :finfo-add-local-label :finfo-add-local :finfo-next-call-id :finfo-add-resume-label
           :finfo-add-nestfunc
           :make-resume :make-init-efp-esp :make-parmp-init :save-args-into-estack :make-extid
           :make-normalize-nf :make-frame-save :make-frame-resume
           :make-suspend-return :finfo-find-local-label :combine-ret-list
           :simple-exp-p)
  (:use "RULE" "CL" "SC-MISC")
  (:shadow cl:declaration))
(in-package "NESTED-FUNCTION")


(defstruct finfo 
  name                                  ;関数名
  parent-func                           ;親関数のfinfo構造体（非nil iff 入れ子関数）
  ret-type                              ;返り値の型
  argp                                  ;argpの要/不要 (=入れ子関数呼び出しの有無)
  ;; (<復帰位置を示すラベル名> . <フレーム復帰用コード>) のリスト(逆順)
  label-list
  ;; ( <symbol> . <texp> )
  ;; frameには var-list #|と tmp-listの両方を|# 入れる
  ;; （nf-list の orig-name も入れる）
  ;; frame-save, frame-resume はvar-listのみ
  ;; tmp-list は不要か？
  var-list                              ;通常のローカル変数のリスト＋関数のポインタ保存用
  tmp-list                              ;一時変数のリスト（call時の引数，入れ子関数ポインタ保存用）
  nf-list                               ;定義された入れ子関数のリスト ( <orig-name> . <ext-name> )
  ;; 明示的スタックを参照するローカル変数．list of <id(symbol)>
  ;; var-listと重複．入れ子関数は nf-listで管理するのでここには入れない
  ;; search-ptr での収集方針により，var-listでないものも入り得る．
  estack-var
  ;; staticで定義された変数 (<orig-name> . <ext-name>)．var-listと重複．
  static-var
  ;; 局所ラベルのリスト(def ,id __label__)で定義。入れ子関数脱出用
  ;; (<ラベル名> . <復帰コード>)。宣言と逆順
  local-label
  )

;; フレーム構造体の管理
(defstruct frame-struct-info
  name                                  ; 構造体名 (SCのid)
  func-name                             ; 対応する関数名 (SCのid)
  )

;;; global関数名
(defvar *global-funcs* '())

;;; 関数フレームの構造体情報
;;; ( <関数名(symbol)> . <構造体情報> ) のリスト
;;; <構造体情報> := ( <構造体名> ) 
(defvar *frame-alist* '() )
;;; 現在いる関数の情報
(defvar *current-func* nil)

(defvar *estack-size* 65536)
(defvar *all-in-estack* nil)

;;;; 環境設定
(defmacro with-nestfunc-environment (&body body)
  `(let ((*global-funcs* '())
         (*estack-size* (ruleset-param 'rule::estack-size))
         (*all-in-estack* (ruleset-param 'rule::all-in-estack))
         (*frame-alist* '())
         (*current-func* nil))
     ,@body))


;;;; 関数

(defun add-global-func (fid)
  (push fid *global-funcs*))

(defun global-func-p (fid)
  (member fid *global-funcs* :test #'eq))

;;; マルチスレッド環境で，スタックの最初のフレームを判断する条件
(defun thread-origin-p (finfo-or-fid)
  (let ((fid (if (symbolp finfo-or-fid)
                 finfo-or-fid
               (finfo-name finfo-or-fid))))
    (or (eq ~main fid)
        (eq ~thread-origin fid))))      ; 不要か？


;;; 関数名を*frame-alist*から探して関数フレームの構造体情報を得る。
;;; 未登録の場合は登録して、登録した構造体情報を返す。
(defun get-frame (x)
  (let* ((asc (assoc x *frame-alist* :test #'eq)))
    (if asc
        (cdr asc)
      (let* ((strx (identifier0! x :sc2c))
             (frame-name (generate-id (string+ strx "_frame")))
             (frame-info (make-frame-struct-info :name frame-name :func-name x)))
        (push (cons x frame-info) *frame-alist*)
        frame-info))))

;;; 関数名=>関数のフレーム構造体名
(defun get-frame-name (fname)
  (frame-struct-info-name (get-frame fname)))

;;; 現関数（親は含まない）のlocal-variableか？
;;; tmp-list内のもの，入れ子関数は検索対象ではない．
(defun local-variable-p (id &optional (finfo *current-func*))
  (and *current-func*
       (assoc id (finfo-var-list finfo) :test #'eq)))

;;; 現関数（親は含まない）の明示的スタックに値があるlocal-variableか？
(defun estack-variable-p (id &optional (finfo *current-func*)
                                       (skip-lv-check nil)) ; local-variable-p のチェックを省略
  (and (or skip-lv-check (local-variable-p id finfo))
       (member id (finfo-estack-var finfo) :test #'eq)))

;;; 現関数（親は含まない）でstaticで定義されたlocal-variableか？
;;; そうなら，ext-idを返す．
(defun static-variable-p (id &optional (finfo *current-func*)
                                       (skip-lv-check nil)) ; local-variable-p のチェックを省略
  (and (or skip-lv-check (local-variable-p id finfo))
       (cdr (assoc id (finfo-static-var finfo) :test #'eq))))

;;; 現関数(=0)からいくつ外の関数で定義された local-variable/nestfunc か?
;;; 見つからなければ -1
;;; 第二返り値は local-varriable-> :var, nestfunc-> :nestfunc
;;; 第三返り値は 見つかった関数のfinfo
;;; 第四返り値は local-variable-p/nestfunc-extid の返り値
(defun howmany-outer (id &optional (finfo *current-func*))
  (labels ((rec (curfunc acc)
             (acond
              ((null curfunc)
               -1)
              ((local-variable-p id curfunc)
               (values acc :var finfo it))
              ((nestfunc-extid id curfunc)
               (values acc :nestfunc finfo it))
              (t (rec (finfo-parent-func curfunc) (1+ acc))) )))
    (rec finfo 0)))

;;; 与えられた関数情報からフレーム構造体の定義を作る
(defun make-frame-def (fi)
  (let* ((frm-info (get-frame (finfo-name fi)))
         (frame-name (frame-struct-info-name frm-info))
         (member-list (append (finfo-var-list fi)
                             ;; (finfo-tmp-list fi)
                             (mapcar #'(lambda (x) (cons (car x) ~closure-t))
                                     (finfo-nf-list fi))))
         (member-defs (mapcar #'(lambda (x) ~(def ,(car x) ,(cdr x)))
                              member-list)))
    ~(def (struct ,frame-name)
         (def tmp-esp (ptr char))       ; これは、絶対に先頭
       (def argp (ptr char))
       (def call-id int)
       ,@member-defs)))

;;; 全ての関数フレーム構造体の宣言を作る
(defun make-all-frame-decls ()
  (nreverse                             ; nreverseは必須ではない
   (loop for (fn-name . frm-info) in *frame-alist*
       collect (with1 frame-name (frame-struct-info-name frm-info)
                 ~(decl (struct ,frame-name))))) )

;;; トップレベルに追加する宣言の処理
(defvar *static-var-defs* ())
(defvar *frame-defs* ())
(defvar *nestfunc-defs* ())
(defmacro with-external-decl (&body body)
  `(let ((*static-var-defs* ()) (*frame-defs* ()) (*nestfunc-defs* ()))
     ,@body))
(defun add-static-var-def (decl)
  (push decl *static-var-defs*))
(defun flush-static-var-defs ()
  (prog1 (nreverse *static-var-defs*)
    (setq *static-var-defs* ())))
(defun add-frame-def (decl)
  (push decl *frame-defs*))
(defun flush-frame-defs ()
  (prog1 (nreverse *frame-defs*)
    (setq *frame-defs* ())))
(defun add-nestfunc-def (decl)
  (push decl *nestfunc-defs*))
(defun flush-nestfunc-defs ()
  (prog1 (nreverse *nestfunc-defs*)
    (setq *nestfunc-defs* ())))

;;; ブロックの先頭に追加する宣言の処理
(defvar *additional-local-decls* ())
(defmacro with-new-block (&body body)
  `(let ((*additional-local-decls* ())) ,@body))
(defun add-local-decl (decl)
  (push decl *additional-local-decls*))
(defun flush-local-decls ()
  (prog1 (nreverse *additional-local-decls*)
    (setq *additional-local-decls* ())))

;;; 各文の直前に追加する文の処理
(defvar *precedents* ())
(defmacro with-block-item (&body body)
  `(let ((*precedents* ())) ,@body))
(defun add-precedent (item)
  (push item *precedents*))
(defun flush-precedents ()
  (prog1 (nreverse *precedents*)
    (setq *precedents* ())))

;;; 今トップレベルにいるかどうかの判定
(defun top-level-p (&key (finfo *current-func*))
  (not finfo))

;;; --local-- で宣言された局所ラベルを追加
;;; cons の cdr 部はフレーム復帰用コードで，後から追加
(defun finfo-add-local-label (id &key (finfo *current-func*))
  (push (cons id nil) (finfo-local-label finfo)))

;;; *current-func*に変数情報を追加して、declaration文を返す
(defun finfo-add-local (id texp mode &key (init nil) (finfo *current-func*))
  ;; mode： :var or :temp
  (when finfo
    (case mode
      ((:var)                           ; （estack-varでなければ）save/resume の対象
       (when (let ((ttexp (remove-type-qualifier texp)))
               (or (and (listp ttexp)
                        (eq ~array (car ttexp)))
                   *all-in-estack*))
         (pushnew id (finfo-estack-var finfo) :test #'eq))
       (push (cons id texp) (finfo-var-list finfo)))
      ((:tmp)                           ; save/resume の対象にならない
       (push (cons id texp) (finfo-tmp-list finfo)))
      ((:static)                        ; 外に出す．frameにも入れない．
       (let ((ext-id (generate-id
                      (string+ (identifier0! id :sc2c) "_in_" 
                               (identifier0! (finfo-name finfo) :sc2c)))))
         (push (cons id texp) (finfo-var-list finfo))
         (push (cons id ext-id) (finfo-static-var finfo))
         (setq id ext-id)))             ; 名前を衝突しないように変更
      ((:system)                        ; 補助変数（特に副作用なし）
       )
      (otherwise
       (error "unexpected value of 'mode'(~s)" mode))))
  (if init
      ~(def ,id ,texp ,init)
    ~(def ,id ,texp)))

;;; *current-func* に入れ子関数情報を追加
(defun finfo-add-nestfunc (id extid &optional (finfo *current-func*))
  (push (cons id extid)
        (finfo-nf-list finfo)) )

;;; 次のcall-idの数字
(defun finfo-next-call-id (&optional (finfo *current-func*))
  (length (finfo-label-list finfo)))

;;; 復帰位置を示すラベルを追加．ラベル名を返す．
(defun finfo-add-resume-label (&optional (finfo *current-func*) (base-name "L_CALL"))
  (with1 label-id (rule:generate-id base-name)
    (push (cons label-id nil) (finfo-label-list finfo))
    label-id))

;;; 与えられた関数情報から復帰処理を行うstatementを作る
(defun make-resume (fi)
  (unless (or (finfo-label-list fi)
              (finfo-local-label fi))
    (return-from make-resume
      (list ~(label LGOTO nil))))
  (let ((reconst-impossible (or (eq ~main (finfo-name fi))
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
            (push ~((case ,k)
                    ,@(cdar lb) 
                    (goto ,(caar lb)))
                  ret))
          ;; gotoによる入れ子関数から親関数への脱出先
          (do ((ret nil)
               (k -1 (1- k))
               (lb (reverse (finfo-local-label fi)) (cdr lb)))
              ((endp lb) (apply #'append (nreverse ret)))
            (push ~((case ,k)
                    ,@(cdar lb) 
                    (goto ,(caar lb)))
                  ret))))
        (frame-type ~(struct ,(get-frame-name (finfo-name fi)))))
    (list
     ~(if ,(if reconst-impossible
               ~0
             ~esp-flag)
          (begin
           ,@(unless reconst-impossible
               ~( (= esp (cast (ptr char)
                           (bit-xor (cast size-t esp) esp-flag)))
                  (= efp (cast (ptr ,frame-type) esp))
                  (= esp (aligned-add esp (sizeof ,frame-type)))
                  (= (mref-t (ptr char) esp) 0) ))
           (label LGOTO
                  (switch (fref (mref efp) call-id) ,@case-goto))
           ,@(when (finfo-label-list fi)
               ~( (goto ,(caar (last (finfo-label-list fi)))) )))))))

;;; efp(xfp)の設定および espをフレームサイズ分移動させる
(defun make-init-efp-esp (fi)
  (let ((frame-type  ~(struct ,(get-frame-name (finfo-name fi)))))
    (list*
     ~(= efp (cast (ptr ,frame-type) esp))
     ~(= esp (aligned-add esp (sizeof ,frame-type)))
     ~(= (mref-t (ptr char) esp) 0)
     (when (and *all-in-estack* (finfo-parent-func fi))
       (list ~(= (fref efp -> xfp) xfp) )))
    ))

;;; parmp の初期値
(defun make-parmp-init (&optional (all-in-estack *all-in-estack*))
  ~(cast (ptr char)
     ,(if all-in-estack
          ~esp
        ~(bit-xor (cast size-t esp) esp-flag))) )

;;; （*all-in-estack*時）引数の値をestackに保存
(defun save-args-into-estack (argid-list argtexp-list
                              &optional (finfo *current-func*))
  ;; ちょっと手抜きで型情報 (the)なし
  (mapcar #'(lambda (id texp)
              (if (finfo-parent-func finfo)
                  ~(= (fref efp -> ,id) (pop-arg ,texp parmp)) 
                ~(= (fref efp -> ,id) ,id) ))
          argid-list
          argtexp-list) )

;;; 入れ子関数のid -> トップレベルに移した関数のid
(defun make-extid (id &optional (pfinfo *current-func*))
  (generate-id (string+ (identifier0! id :sc2c) "_in_"
                        (identifier0! (finfo-name pfinfo) :sc2c))) )

;;; idが現在の関数（親は除く）内で定義された入れ子関数か？
;;; もしそうなら，ext-name を返す
(defun nestfunc-extid (id &optional (finfo *current-func*))
  (and *current-func*
       (cdr (assoc id (finfo-nf-list finfo) :test #'eq))))

;;; 入れ子関数の参照 -> etackへの参照
;;; （pfinfo: 親関数情報）
(defun nestfunc-in-estack (fid &optional (pfinfo *current-func*))
  (declare (ignore pfinfo))
  ~(ptr (fref efp -> ,fid)))

;;; 与えられた関数情報から入れ子関数を正規化するコードを作る
(defun make-normalize-nf (&optional (fi *current-func*))
  (let ((nf-list (finfo-nf-list fi)))
    (apply #'nconc
           (mapcar
            #'(lambda (x) 
                ~( (= (fref efp -> ,(car x) fun)
                      ,(cdr x))
                   (= (fref efp -> ,(car x) fr)
                      (cast (ptr void) efp)) ))
            nf-list))))

;;; 与えられた関数情報からフレーム情報を保存するコードを作る
(defun make-frame-save (&optional (fi *current-func*))
  (mapcar
   #'(lambda (x)
       ~(= (fref efp -> ,(car x)) ,(car x)))
   (remove-if #'(lambda (x)
                  (or (estack-variable-p (car x) fi t)
                      (eq ~closure-t (cdr x))))
              (finfo-var-list fi))))

;;; 与えられた関数情報からフレーム情報を復活するコードを作る
(defun make-frame-resume (&optional (fi *current-func*))
  (mapcar
   #'(lambda (x) 
       ~(= ,(car x) (fref efp -> ,(car x))))
   (remove-if #'(lambda (x)
                  (or (estack-variable-p (car x) fi t)
                      (eq ~closure-t (cdr x))))
              (finfo-var-list fi))))

;;; 与えられた関数情報から関数中断用のreturnを生成するコードを作る
(defun make-suspend-return (&optional (fi *current-func*))
  (cond ((finfo-parent-func fi)
         ;;~(return (fref efp -> tmp-esp)))
         (error "make-suspend-return called in lightweight-func"))
        ((eq ~void (finfo-ret-type *current-func*))
         ~(return))
        (t
         ~(return (SPECIAL ,(finfo-ret-type *current-func*))))))

;;; 与えられたラベル名が、親関数の局所ラベルとして定義されているか調べる。
;;; 定義されていなければ,返り値はnil。定義されていれば、
;;; (values <自分からみて何番目の親関数に定義されていたか>
;;;         <その定義 ( <label> . <復帰処理> )> 
;;;         <そのラベルが関数内で何番目にpushされたものか>)
(defun finfo-find-local-label (lid &optional (fi *current-func*) &aux (lids (identifier0! lid :sc2c)))
  (labels ((find-local-label-tail (cfi acc &aux memb)
             (cond ((null cfi)
                    nil)
                   ((let* ((memb0 (member 
                                   lids
                                   (finfo-local-label cfi)
                                   :test #'string=
                                   :key #'(lambda (x) (identifier0! (car x) :sc2c)))))
                      (setq memb memb0))
                    (values acc (car memb) (length memb)))
                   (t
                    (find-local-label-tail
                     (finfo-parent-func cfi) (1+ acc))))))
    (find-local-label-tail fi 0)))

;; beginとかの(Nfb0 body)の返り値から、
;; beginそのものの返り値を作る。
;; ((r1-1 r1-2 r1-3 r1-4) ... (rn-1 rn-2 rn-3 rn-4))
;; => ( (,@r1-1 ... ,@rn-1)
;;      (,@r1-2 ... ,@rn-2) 
;;      nil
;;      (,@prev-4th ,@r1-3 ,r1-4 ... ,@rn-3 ,rn-4 ) )
(defun combine-ret-list (ret-list &optional prev-4th)
  (let ((fst (mapcar #'first ret-list))
        (scd (mapcar #'second ret-list))
        (thd-4th (mapcar
                  #'(lambda (x) ~(,@(third x) ,(fourth x)))
                  ret-list)))
    (list (apply #'append fst)
          (apply #'append scd)
          nil
          (remove nil (apply #'append prev-4th thd-4th)))))

;; 一時変数を使う必要がない，つまり
;; * 入れ子関数呼出し中に変更が加えられない(permit-change=nil時）
;; * 副作用を起こさない
;; ことが保証できる式
(defun simple-exp-p (the-exp &optional (permit-change nil))
  (let ((type (second the-exp))
        (exp (third the-exp)))
    (or (and (symbolp exp) permit-change)
        (and (global-func-p exp) (not (local-variable-p exp)))
        (eq 'type::undefined type)
        (sc-number exp)
        (sc-character exp)
        (sc-string exp))))
