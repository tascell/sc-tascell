;;; Copyright (c) 2009-2010 Tasuku Hiraishi <tasuku@media.kyoto-u.ac.jp>
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

(unless (featurep :allegro)
  (error "Sorry! This programs work only on Allegro Common Lisp!"))
(when (featurep :mswindows)
  (setq *locale* (find-locale "japan.EUC")))

;; The most debuggable (and yet reasonably fast) code, use
;; (proclaim '(optimize (speed 3) (safety 1) (space 1))); (debug 3)))
(proclaim '(optimize (speed 3) (safety 0) (space 1)))

;; compile and load external lisp modules
(load (compile-file-if-needed (or (probe-file "sc-misc.lsp") "../../sc-misc.lsp")
                              :output-file "sc-misc.fasl"))
(load (compile-file-if-needed "queue.lsp"))
(load (compile-file-if-needed "server.lsp"))

;; Uncomment to ignore logging code
;; (push :tcell-no-transfer-log *features*)

;; —ª‹L
(defun ms (&rest args)
  (apply #'make-and-start-server args))

;; gero‚Å‚Ì•]‰¿—p
(defun gs (&rest args)
  (apply #'make-and-start-server :auto-resend-task 0 :local-host "gero00" args))
