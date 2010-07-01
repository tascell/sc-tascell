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

(defvar *force-compile* nil)
(defvar *force-compile-server* nil)

;; Uncomment to ignore logging code
;; (push :tcell-no-transfer-log *features*)

#+mswindows
(setq *locale* (find-locale "japan.EUC"))

#+sbcl
(setq sb-impl::*default-external-format* :euc-jp)


;; The most debuggable (and yet reasonably fast) code, use
(proclaim '(optimize (speed 3) (safety 3) (space 1) (debug 3)))
;; (proclaim '(optimize (speed 3) (safety 0) (space 1)))

#+sbcl
(defun compile-file-if-needed (x &rest compile-file-args &key force-compile &allow-other-keys)
  (let ((flag t) (temp nil))
    (dolist (e compile-file-args)
      (cond
       ((eq e :force-compile)
        (setq flag nil))
       ((not flag)
        (setq flag t))
       (t
        (push e temp))))
    (setq compile-file-args (nreverse temp)))
  (let ((fasl (apply #'compile-file-pathname x compile-file-args)))
    (if (and (not force-compile)
             (and (probe-file fasl)
                  (> (file-write-date fasl) (file-write-date x))))
        fasl
      (apply #'compile-file x compile-file-args))))

;; compile and load external lisp modules
(load (compile-file-if-needed (or (probe-file "sc-misc.lsp") "../../sc-misc.lsp")
                              :output-file "sc-misc.fasl"
                              :force-compile *force-compile*))

#+sbcl
(progn
  (require :sb-bsd-sockets)
  (require :sb-posix)
  (dolist (f '("sbcl-compat-mp.lsp"
               "packages.lisp"
               "acl-excl.lisp"
               "lw-buffering.lisp"
               "chunked-stream-mixin.lisp"
               "acl-socket.lisp"))
    (load (compile-file-if-needed f :force-compile *force-compile*))))

(load (compile-file-if-needed "queue.lsp" :force-compile *force-compile*))
(load (compile-file-if-needed "server.lsp" :force-compile *force-compile-server*))

;; abbreviation for make-and-start-server
(defun ms (&rest args)
  (apply #'tsv:make-and-start-server args))

;; for evaluation in the gero cluster in ylab
(defun gs (&rest args)
  (apply #'tsv:make-and-start-server :auto-resend-task 0 :local-host "gero00" args))
