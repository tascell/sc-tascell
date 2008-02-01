;;; 規則ファイル読み込み用のreadtable
(provide 'sct-macch)
(in-package "SC-TRANSFORMER")

(require 'sct)

(defun make-pattern-readtable
    (&key 
     (rcomma comma) (rcomma-at comma-at) (rbquote bquote)
     (roptional optional)
     &aux
     (in-[]-readtable (copy-readtable))
     (ret-readtable (copy-readtable)))
  ;; ,<symbol>  => (comma <symbol>)
  ;; ,@<symbol> => (comma-at <symbol>)
  (set-macro-character 
   #\,
   #'(lambda (stream char &aux fst scd rest)
       (if (char= #\@ (peek-char nil stream))
	   (progn
	     (read-char stream)
	     (setq fst rcomma-at
		   scd (read stream)))
	   (setq fst rcomma
		 scd (read stream)))
       (when (eql #\[ (peek-char nil stream nil))
	 (read-char stream)
	 (setq rest
	       (in-readtable in-[]-readtable
			     (read-delimited-list #\] stream t))))
       (cons fst (cons scd rest)))
   nil ret-readtable)
  ;; [ <list> ] => occurs error 
  (set-macro-character #\] (get-macro-character #\)) nil in-[]-readtable)
  (set-macro-character 
   #\[
   #'(lambda (stream char)
       (error "[ appered with neither , nor ,@"))
   nil ret-readtable)
  ;; `<symbol> => (bquote <symbol>)
  (set-macro-character
   #\`
   #'(lambda (stream char)
       (list rbquote (read stream)))
   nil ret-readtable)
  ;; { <list> } => (optional <list>)
  (set-macro-character #\} (get-macro-character #\)) nil ret-readtable)
  (set-macro-character 
   #\{
   #'(lambda (stream char)
       (let ((optional-list (read-delimited-list #\} stream t)))
	 `(,roptional ,@optional-list)))
   nil ret-readtable)
  ;;
  ret-readtable)

(defmacro in-readtable (readtable &body forms)
  `(let ((*readtable* ,readtable))
    ,@forms))


(defun string2pat (str) 
  (in-readtable *pattern-readtable*
    (read-from-string str)))
