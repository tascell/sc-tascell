;;; Copyright (c) 2009 Tasuku Hiraishi <tasuku@media.kyoto-u.ac.jp>
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

;;; server-cmdline.lsp: evaluated when Lisp is executed by "server-batch".
;;;                     This executes a Tascell server in batch mode.

(let ((*load-verbose* nil))
  (load "LOAD.lsp"))

;; exit command
#+(or allegro cmu sbcl clisp)
(setf (symbol-function 'bye)
      #+allegro #'exit
      #+(or cmu sbcl) #'quit
      #+clisp #'ext::bye)

;;;
#+(or allegro clisp sbcl)
(let ((args (or #+allegro (cdr (sys:command-line-arguments)) #+clisp ext:*args*
                #+sbcl (cdr (sb-ext:*posix-argv*))))
      (exit-status 1))
  (unwind-protect
      (with* (hostname "localhost"
              port 8888
              n-children 1
              initial-task nil)
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
                 ((#\h)                 ; hostname
                  (setq hostname parm))
                 ((#\p)                 ; port number
                  (setq port (parse-integer parm)))
                 ((#\n)                 ; # of children
                  (setq n-children (parse-integer parm)))
                 ((#\t)                 ; initial task
                  (setq initial-task parm))
                 (otherwise
                  (format *error-output* "~&Unknown option: ~S~%" hd)
                  (bye exit-status)))))
            (otherwise
             (format *error-output* "~&Unknown option: ~S~%" hd)
             (bye exit-status))))
        (unless initial-task
          (warn "No initial task given.")
          (bye exit-status))
        (make-and-start-server :local-host hostname
                               :children-port port
                               :n-wait-children n-children
                               :auto-initial-task initial-task
                               :auto-exit t)
        (setq exit-status 0))
    (bye exit-status)))
