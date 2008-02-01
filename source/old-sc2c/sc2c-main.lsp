(defvar *sc2c-package* 'user)
(defvar *auto-include* '("<stdio.h>" "<string.h>" "<stdlib.h>" "<ctype.h>"))
(defvar *step-exec* nil)
(defvar *remove-redundancy* t)
;;(defvar *exec-hoisting* t)
;;(defvar *exec-tls* nil)

(defun sc2c (input-file &key output-file
			((:rule  rule-list) nil)
			((:step  *step-exec*) *step-exec*)
;;			((:hoist *exec-hoisting*) *exec-hoisting*) 
;;			((:tls   *exec-tls*) *exec-tls*)
			&aux (*print-case* :downcase)
			     (*auto-include* *auto-include*))
  (in-package *sc2c-package*)
  
  (if (not (stringp input-file))
      (error "input-file is not string"))
  
  (if (not (probe-file input-file))
      (error "file ~s not found." input-file))
  
  (if (not (stringp output-file))
;;    (setq output-file (change-extension input-file "c")))
      (setq output-file (file-namestring (change-extension input-file "c"))))
  
  (let (x)
    (with-open-file (istream input-file :direction :input)
      (do ((y (read istream nil 'eof) (read istream nil 'eof)))
	  ((eq y 'eof) (setq x (nreverse x)))
	(push y x)))

    (unless (listp rule-list) (setq rule-list (list rule-list)))
  
    ;; マクロ展開
    (setq x (apply-macro x))
    (when *step-exec* (princ x) (break))

    ;; sc2sc(変形規則適用)
    (dolist (rule rule-list)
      (unless (probe-file rule)
	(let ((rule2 (string+ `("rule/" ,rule))))
	  (if (probe-file rule2)
	      (setq rule rule2)
	      (error "rule-file ~s not found." rule))))
      (setq x (sc2sc x rule))
      (setq x (apply-macro x))
      (when *step-exec* (princ x) (break)))
	
#|  ;; sc2sc-???.lsp による変換    
    (setq x (btrans x))
    (when *step-exec* (princ x) (break))
    
    (when *exec-tls*
      (setq x (tls-ext x))
      (when *step-exec* (princ x) (break)))
    
    (when *exec-hoisting*
      (setq x (hoisting2 x))
	(when *step-exec* (princ x) (break)))      
|#    

    ;; 構文木(構造体)生成
    (setq x (par-translation-unit x))
    (when *step-exec* (princ x) (break))

#|  ;; 構造体->構造体への変換
      (when *exec-hoisting*
	(setq x (hs-translation-unit x))
	(when *step-exec* (princ x) (break)))

      (when (and *exec-tls* *thread-local-storage*)
	(setq x (add-tls-arg x))
	(when *step-exec* (princ x) (break))) 
|#
    ;; 構造体->Cコード
    (setq x (cd-translation-unit x))
    (when *step-exec* (princ x) (break))
    
    (with-open-file (*standard-output* output-file :direction :output)
      (princ x))
    
    (princ x))
  
  output-file)


	     