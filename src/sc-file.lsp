;;; SCファイルの読み書き関数
(provide "SC-FILE")
(eval-when (:compile-toplevel :load-toplevel :execute)
  (require "SC-DECL" "sc-decl.lsp"))

(scr:require "SC-MISC")

(in-package "SC-FILE")

(defmacro with-sc-read-environment (&body body)
  `(let ((*package* *code-package*)
	 (*readtable* *code-readtable*))
     ,@body))

(defun read-sc-file (filespec)
  (with-sc-read-environment
      (read-file filespec)))

(defun write-sc-file (filespec sc-code)
  (with-sc-read-environment
      (write-file filespec sc-code :overwrite t :write-string nil)))
