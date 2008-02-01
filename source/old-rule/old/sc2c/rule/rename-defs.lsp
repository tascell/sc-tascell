(provide "RENAME-DEFS")
(in-package "RENAME")
(require "SC-UTIL")

(use-package "SC-TRANSFORMER")
(use-package "SC-MISC")
(use-package "SC-UTIL")


(defvar *id-alist* nil)      ; list of (<id> . <block-level>)
(defvar *replace-alist* nil) ; list of (<oldid> . <newid>)
(defvar *used-id-list* nil)  ; 使用済識別子(string)のリスト
			     ; 新しい識別子名を作ったときに追加
(defvar *block-id* 0)        ; 現在のブロックID
(defvar *next-block-id* 0)   ; 次に使用するブロックID-1
       
; get *block-id* of id
(defun assoc-id-level (id)
  (cdr (assoc (par-identifier id)
	      *id-alist* :key #'par-identifier :test #'string=)))

; get replacement-id of old-id
(defun assoc-replacement-id (id)
  (or
   (cdr (assoc (par-identifier id) 
	       *replace-alist* :key #'par-identifier :test #'string=))
   id))

; create new identifier
(defun create-new-id (base-id &optional (used-id-list *used-id-list*))
  (let ((n 2) ns (base-idstr (par-identifier base-id)))
    (loop
     (setq ns (string+ base-idstr "__" (write-to-string n)))
     (unless (member ns used-id-list :test 'string=)
       (push ns *used-id-list*)
       (return (id-to-scid ns)))
     (incf n))))

; entry identifier
; 返り値は更新先の (values *id-alist* *replace-alist*) 
(defun entry-identifier (id)
  (let ((lev (assoc-id-level id)))
    (if lev 
	(if (= *block-id* lev)     
	    ;同一スコープで宣言済 ->そのまま
	    (values *id-alist* *replace-alist*)
	    ;別のスコープで宣言済
	    (let* ((new-id (create-new-id id)))
	      (values
	       (cons (cons id *block-id*) *id-alist*)
	       (cons (cons id new-id) *replace-alist*))))
      ; 未宣言
      (values 
       (cons (cons id *block-id*) *id-alist*)
       (cons (cons id id) *replace-alist*)))))

; replace all identifier in s-expression
(defun repl-id (x)
  (if (listp x)
      (mapcar #'repl-id x)
      (assoc-replacement-id x)))

; entry id and bind *id-alist* *replace-alist* 
(defmacro bind-alists (id repl &body form)
  `(if (listp ,id)
    (let ((,repl nil))
      (bind-alists2 ,id ,repl ,@form))
    (progn
      (multiple-value-setq
	  (*id-alist* *replace-alist*) (entry-identifier ,id))
      (let ((,repl (repl-id ,id))) 
	,@form))))
(defmacro bind-alists2 (id-list repl &body form)
  `(if (null ,id-list)
    (progn ,@form)
    (progn
      (multiple-value-setq
	  (*id-alist* *replace-alist*) (entry-identifier (car ,id-list)))
      ;;    (format t "id-list:~S (repl-id (car ,id-list)):~S~%" 
      ;; ,id-list (repl-id (car ,id-list)))
      (let ((,repl `(,@,repl ,(repl-id (car ,id-list)))))
	(bind-alists2 (cdr ,id-list) ,repl ,@form)))))

; increment block-level
(defmacro inc-block-level (&body form)
  `(let ((*replace-alist* *replace-alist*)
	 (*block-id* (incf *next-block-id*)))    
    ,@form))

; begin function
(defmacro begin-function (&body form)
  `(let ((*id-alist* *id-alist*)
	 (*replace-alist* *replace-alist*)
	 (*block-id* (incf *next-block-id*)))
    ,@form))

)
