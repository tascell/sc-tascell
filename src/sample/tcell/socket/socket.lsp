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

;; 実行例
;; (exec-with-remote-io "../fib-xcc")   ; 単体
;; (start-procs 5 "../fib-xcc")         ; 複数同時起動
;; (kill-procs)                         ; start-procsの後始末

(eval-when (:compile-toplevel :load-toplevel :execute)
  (unless (featurep :allegro)
    (error "Sorry! This programs work only on Allegro Common Lisp!"))
  (when (featurep :mswindows)
    (setq *locale* (find-locale "japan.EUC")))
  (compile-file-if-needed "../../../sc-misc.lsp")
  (load "../../../sc-misc"))
#+sc-system (use-package "SC-MISC")
(use-package :socket)

(defvar *remote-io-host* "localhost")
(defvar *remote-io-port* 8888)

(defun exec-with-remote-io (command &key (host *remote-io-host*) (port *remote-io-port*))
  (with-open-stream (sock (make-socket :remote-host host :remote-port port))
    (multiple-value-bind (cmd-input cmd-output cmd-error process)
	(excl:run-shell-command
	 command :input :stream :output :stream :error-output :stream
	 :separate-streams t :wait nil)
      (unwind-protect 
	   (let ((finish-send-server nil)
		 (finish-send-command nil))
	     (loop
		;; 
		(unless finish-send-command
		  (input-buffer-to-output sock cmd-input)
		  (force-output cmd-input)
		  (when (eof-p sock)
		    (setq finish-send-command t)
		    (finish-output cmd-input)
		    (shutdown sock :direction :input)
		    (close cmd-input)))
		;; 
		(unless finish-send-server
		  (input-buffer-to-output cmd-output sock
                                          *standard-output*) ;debug print
		  (force-output sock)
		  (force-output)        ;debug print
		  (when (eof-p cmd-output)
		    (setq finish-send-server t)
		    (finish-output sock)
		    (shutdown sock :direction :output)))
	       ;; 
	       ;;(sleep 1/2)
	       (mp:wait-for-input-available (list sock cmd-output))
	       ;; 
		(unless (open-stream-p sock)
		  (sys:reap-os-subprocess :pid process :wait t)
		  (return))
		))
	;; Protected form
	(close cmd-input)
	(close cmd-output)
	(close cmd-error))
      )))

(defparameter *procs-sleep-time* 0.1)
(let ((*procs* '()))
  (defun start-procs (n &rest args)
    (loop for i from 1 to n
        do (let ((new-proc
                  (mp:process-run-function (symbol-name (gensym "PROC"))
                    #'apply #'exec-with-remote-io args)))
             (sleep *procs-sleep-time*)
             (push new-proc *procs*)))
    *procs*)
  (defun kill-procs ()
    (prog1
        (mapc #'mp:process-kill *procs*)
      (setq *procs* '()))))

(defun exec-with-remote-io-server-terminal
    (&key (port *remote-io-port*) (input *standard-input*) (output *standard-output*))
  (with-open-stream (p-sock (make-socket :connect :passive :local-port port :reuse-address t))
    (loop
       (with-open-stream (strm (accept-connection p-sock))
	 (format *error-output* "~&[Start: ~S]~%" strm)
	 (let ((finish-send-client nil))
	   (loop
	      ;; 
	      ;; 
	      (unless finish-send-client
		(do ((line (read-line input) (read-line input)))
		    ((cond ((string= "." line)     ; 
			    (force-output strm)
			    t)
			   ((string= ".end" line)  ; EOF
			    (setq finish-send-client t)
			    (finish-output strm)
			    (shutdown strm :direction :output)
			    t)
			   ((and (>= (length line) 1)
				 (eql #\. (aref line 0)))
			    (format strm "~A~%" (string-left-ntrim line 1))
			    nil)
			   (t
			    (format strm "~A~%" line)
			    nil))
		     )))
	      ;; 
	      (input-buffer-to-output strm output)
	      (force-output output)
	      (when (eof-p strm)
		(shutdown strm :direction :input)
		(return))))
	 (format *error-output* "~&[Finish]~%"))
       )))
