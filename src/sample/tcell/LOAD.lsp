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

(or #+allegro t
    #+sbcl    t
    (error "Sorry! This programs work only on Allegro Common Lisp or SBCL!"))

#+mswindows
(setq *locale* (find-locale "japan.EUC"))

#+sbcl
(setq sb-impl::*default-external-format* :euc-jp)

;; Uncomment to ignore logging code
(push :tcell-no-transfer-log *features*)

;; The most debuggable (and yet reasonably fast) code, use
(proclaim '(optimize (speed 3) (safety 3) (space 1) (debug 3)))
;; (proclaim '(optimize (speed 3) (safety 0) (space 1)))

;; compile and load external lisp modules
(load (compile-file (or (probe-file "sc-misc.lsp") "../../sc-misc.lsp")
                    :output-file "sc-misc.fasl"))
#+sbcl
(progn
  (require :sb-bsd-sockets)
  (require :sb-posix)
  (load (compile-file "sbcl-compat-mp.lsp"))
  (load (compile-file "packages.lisp"))
  (load (compile-file "acl-excl.lisp"))
  (load (compile-file "lw-buffering.lisp"))
  (load (compile-file "chunked-stream-mixin.lisp"))
  (load (compile-file "acl-socket.lisp")))

(load (compile-file "queue.lsp"))
(load (compile-file "server.lsp"))


;; abbreviation for make-and-start-server
(defun ms (&rest args)
  (apply #'tsv:make-and-start-server args))

;; for evaluation in the gero cluster in ylab
(defun gs (&rest args)
  (apply #'tsv:make-and-start-server :auto-resend-task 0 :local-host "gero00" args))
