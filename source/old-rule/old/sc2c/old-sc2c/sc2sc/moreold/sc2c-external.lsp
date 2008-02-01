(defun sc2c (input-file &optional output-file)
  (if (not (stringp input-file))
      (error "input-file is not string"))

  (if (not (probe-file input-file))
      (error (concatenate 'string "file "
			  (write-to-string input-file) " not found.")))

  (if (not (stringp output-file))
      (setq output-file (change-extension input-file "c")))

  (with-open-file (input-stream input-file :direction :input)
    (with-open-file (output-stream output-file :direction :output)
      (if (external-definition input-stream output-stream)
	  output-file
	  (error "compile failed")))))

;;; istreamから入力を受け取ってostreamに変換結果を出力
(defun external-definition
    (&optional (istream *standard-input*) (ostream *standard-output*))
  (setq *lev* 0)
  (setq *block-index* (make-array 256))
  (do ((x (read istream nil 'eof) (read istream nil 'eof)))
      ((eq x 'eof) t)
    (let (outstr)
      (setq outstr (declaration x))
      (if (null outstr) (return-from external-definition))
      (princ outstr ostream)
      (princ #\Linefeed ostream)
      (princ #\Linefeed ostream))))  