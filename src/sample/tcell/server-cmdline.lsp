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

(defvar *force-compile* nil)
(defvar *force-compile-server* nil)

#+sbcl
(declaim (sb-ext:muffle-conditions sb-ext:compiler-note))

(let ((*load-verbose* nil))
  (load "LOAD.lsp"))
(use-package "MISC")

;; exit command
#+(or allegro sbcl clisp)
(setf (symbol-function 'bye)
      #+allegro #'exit
      #+sbcl #'(lambda (x) (sb-ext:quit :unix-status x))
      #+clisp #'ext::bye)

;;;
#+(or allegro clisp sbcl)
(let ((args (or #+allegro (cdr (sys:command-line-arguments)) #+clisp ext:*args*
                #+sbcl (cdr sb-ext:*posix-argv*)))
      (exit-status 1))
  (unwind-protect
      (with* (hostname "localhost"
              port 9865
              n-wait-children 1
              initial-task nil
              logfile nil
              server nil
              socket-format :bivalent)
        (do* ((rest args (cdr rest))
              (hd (car rest) (car rest)))
            ((endp rest))
          (case (string-ref hd 0)
            ((#\-)
             (setq rest (cdr rest))
             (unless rest
               (format *error-output* "~&Insufficient number of arguments: ~S~%" args)
               (bye exit-status))
             (with1 parm (car rest)
               (case (string-ref hd 1)
                 ((#\h)                 ; hostname
                  (setq hostname parm))
                 ((#\p)                 ; port number
                  (setq port (parse-integer parm)))
                 ((#\w)                 ; # of children
                  (setq n-wait-children (parse-integer parm)))
                 ((#\i)                 ; initial task
                  (setq initial-task parm))
                 ((#\s)                 ; server
                  (setq server parm))
                 ((#\T)                 ; server
                  (let ((num (parse-integer parm :junk-allowed t)))
                    (setq socket-format
                      (if (or (not (integerp num))
                              (and (integerp num) (> num 0)))
                          :text
                        :bivalent))))
                 ((#\L)                 ; show server log
                  (let ((num (parse-integer parm :junk-allowed t)))
                    (setq tsv:*transfer-log*
                      (if (or (not (integerp num))
                              (and (integerp num) (> num 0)))
                          t))
                    (when (not (integerp num))
                      (setq logfile parm))))
                 ((#\C)                 ; force compile lisp files
                  (let ((num (parse-integer parm :junk-allowed t)))
                    (cond
                     ((eql num 2)
                      (setq *force-compile* nil)
                      (setq *force-compile-server* t))
                     ((> num 0)
                      (setq *force-compile* t)
                      (setq *force-compile-server* t))
                     (t
                      (setq *force-compile* nil)
                      (setq *force-compile-server* nil)))))
                 (otherwise
                  (format *error-output* "~&Unknown option: ~S~%" hd)
                  (bye exit-status)))))
            (otherwise
             (format *error-output* "~&Unknown option: ~S~%" hd)
             (bye exit-status))))
        (unless (or initial-task server)
          (warn "Neither initial task (-i option) nor parent server (-s) given.")
          (bye exit-status))
        (print `(make-and-start-server :local-host ,hostname
                                       :children-port ,port
                                       :n-wait-children ,n-wait-children
                                       :auto-initial-task ,initial-task
                                       :parent-host ,server
                                       :parent-port ,port
                                       :socket-format ,socket-format
                                       :auto-exit ,t))
        (force-output)
        (flet ((exec ()
                 (tsv::make-and-start-server :local-host hostname
                                             :children-port port
                                             :n-wait-children n-wait-children
                                             :auto-initial-task initial-task
                                             :parent-host server
                                             :parent-port port
                                             :socket-format socket-format
                                             :auto-exit t)))
          (if (and tsv:*transfer-log* logfile)
              (unwind-protect
                  (progn
                    (setq tsv:*transfer-log-output* (open logfile :direction :output :if-exists :rename))
                    (format *error-output* "~&Created a log file ~S~%" logfile)
                    (exec))
                (finish-output tsv:*transfer-log-output*)
                (close tsv:*transfer-log-output*))
            (exec)))
        (setq exit-status 0))
    (format t "Exiting Tascell Server. Exit status is ~D~%" exit-status)
    (bye exit-status)))
