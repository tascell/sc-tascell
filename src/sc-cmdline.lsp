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

;;; sc-cmdline.lsp: evaluated when Lisp is executed by bin/sc2c.
;;;                 This loads "init.lsp" and translates a specified SC file and exit Lisp

(let ((*load-verbose* nil))
  (load (make-pathname :name "init" :type "lsp"
                       :directory (pathname-directory *load-pathname*))))

;;; 
#+(or allegro clisp)
(let ((args (or #+allegro (cdr (sys:command-line-arguments)) #+clisp ext:*args*))
      (exit-status 1))
  (unwind-protect
      (with* (default (list :default)
                 input-file nil
                 rule default
                 output-file default
                 sc2c-rule default
                 intermediate default
                 predefinitions default
                 indent default)
        (do* ((rest args (cdr rest))
              (hd (car rest) (car rest)))
            ((endp rest))
          (case (string-ref hd 0)
            ((#\-)
             (pop rest)
             (unless rest
               (format *error-output* "~&Insufficient number of arguments: ~S~%" args)
               (bye exit-status))
             (with1 parm (car rest)
               (case (string-ref hd 1)
                 ((#\r)                 ; add rule-set
                  (when (eq rule default) (setq rule nil))
                  (setq rule (append1 rule (read-from-string parm))))
                 ((#\o)                 ; output-file
                  (setq output-file parm))
                 ((#\s)                 ; sc2c-rule
                  (with1 elm (read-from-string (car rest))
                    (setq sc2c-rule elm)
                    (unless (or (symbolp elm)
                                (listp elm))
                      (format *error-output* "~&Illegal parameter value for -s: ~S~%" (car rest))
                      (bye exit-status))
                    ))
                 ((#\m)                 ; intermediate
                  (with1 elm (read-from-string parm)
                    (setq intermediate (and elm (not (eql 0 elm))))))
                 ((#\i)                 ; indent command
                  (with1 elm (read-from-string parm)
                    (setq indent (if elm parm nil))))
                 ((#\D)                 ; predefinitions (constant only)
                  (destructuring-bind (symstr valstr) (split-string-1 parm '(#\=))
                    (when (eq predefinitions default) (setq predefinitions nil))
                    (setq predefinitions
                      (append1 predefinitions
                               (sc-file:with-sc-read-environment
                                   ~(%defconstant ,(read-from-string symstr)
                                      ,(read-from-string valstr)))))))
                 (otherwise
                  (format *error-output* "~&Unknow option: ~S~%" hd)
                  (bye exit-status)))))
            (otherwise
             (setq input-file hd))))
        (unless input-file
          (format *error-output* "~&No input file.~%")
          (bye exit-status))
        (apply #'sc2c input-file
               (loop for kwd in (list :rule :output-file :sc2c-rule :intermediate :predefinitions :indent)
                   as val in (list rule output-file sc2c-rule intermediate predefinitions indent)
                   unless (eq default val)
                   nconc (list kwd val)))
        (setq exit-status 0))
    (bye exit-status)))
