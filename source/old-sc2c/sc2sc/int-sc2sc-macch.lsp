;;; マクロキャラクター再定義

(defun redefine-macro-character
    (&optional (readtable *readtable*)
     &key 
     (rcomma comma) (rcomma-at comma-at) (rbquote bquote)
     (roptional optional))
  ;; ,<symbol>  => (comma <symbol>)
  ;; ,@<symbol> => (comma-at <symbol>)
  (set-macro-character 
   #\,
   #'(lambda (stream char)
       (if (eq #\@ (peek-char nil stream))
	   (progn
	     (read-char stream)
	     (list rcomma-at (read stream)))
	   (list rcomma (read stream))))
   nil readtable)
  ;; `<symbol> => (bquote <symbol>)
  (set-macro-character
   #\`
   #'(lambda (stream char)
       (list rbquote (read stream)))
   nil readtable)
  ;; { <list> } => (optional <list>)
  (set-macro-character #\} (get-macro-character #\) ))
  (set-macro-character 
   #\{
   #'(lambda (stream char)
       (let ((optional-list (read-delimited-list #\} stream t)))
	 `(,roptional ,@optional-list)))
   t readtable))

	   
    
     
	 