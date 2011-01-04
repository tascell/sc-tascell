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

;;; Additional definitons of indentation for .sc and .rule files.
;;; available with the Allegro CL interface for GNU Emacs
;;; http://www.franz.com/emacs/

(let ((tag 'fi:lisp-indent-hook))
  ;; sc-misc macros
  (put 'with tag '((1 1 quote) (0 t 1)))
  (put 'with* tag '(like with))
  (put 'with1 tag 2)
  (put 'awhen tag '(like when))
  (put 'acond tag '(like cond))
  (put 'aif tag '(like if))
  (put 'error-indent tag '(like when))
  (put 'string-case tag '(like case))
  (put 'string-case-eager tag '(like case))
  (put 'with-fresh-variables tag '(like let))
  ;; .rule
  (put 'defrule tag '((1 (3 t) ((1 0 quote) (0 t nil))) (0 t 2)))
  (put 'extendrule tag '(like defrule))
  (put 'if-pattern-variable tag '(like if))
  (put 'if-match tag 4)
  (put 'when-match tag 3)
  (put 'unless-match tag 3)
  ;; SC
  (put '%defmacro tag '(like defmacro))
  (put '%defconstant tag 1)
  (put '%if tag '(like if))
  (put '%ifdef tag '(like if))
  (put '%ifndef tag '(like if))
  (put '%if* tag '(like when))
  (put '%ifdef* tag '(like when))
  (put '%ifndef* tag '(like when))
  (put 'static-def tag '(like def))
  (put 'begin tag 0)
  (put 'for tag 1)
  (put 'switch tag 1)
  (put 'cast tag 1)
  (put 'if-exp tag 2)
  ;; Tascell
  (put 'do-many tag 6)
  (put 'do-two tag 2)
  (put 'tcell-broadcast tag 1)
  )

;; (defadvice fi::lisp-invoke-method (around sharp-question
;;                                           (form-start method depth count state
;;                                                       indent-point))
;;   (let (ret)
;;     (cond ((and form-start
;;                 (eq (char-after (- form-start 1)) ?\?) ; pattern notation #?(...)
;;                 (eq (char-after (- form-start 2)) ?\#))
;;            (setq ret (fi::lisp-indent-quoted-list depth count state indent-point))
;;            (message "%S,%S:pattern indent" state ret))
;;           ((and form-start
;;                 (eq (char-after (- form-start 1)) ?\~)) ; SC-backquote ~(...)
;;            (setq ret (fi::lisp-indent-quoted-list depth count state indent-point))
;;            (message "%S,%S:SC-backquote indent" state ret))
;;           (t
;;            (setq ret ad-do-it)
;;            (message "%S,%S" state ret)))
;;     ret))
;; (ad-activate 'fi::lisp-invoke-method t)
;; (ad-deactivate 'fi::lisp-invoke-method)

(modify-syntax-entry ?\~ "'   " fi:lisp-mode-syntax-table)
(modify-syntax-entry ?\[ "(]  " fi:lisp-mode-syntax-table)
(modify-syntax-entry ?\] ")[  " fi:lisp-mode-syntax-table)
(modify-syntax-entry ?\{ "(}  " fi:lisp-mode-syntax-table)
(modify-syntax-entry ?\} "){  " fi:lisp-mode-syntax-table)
