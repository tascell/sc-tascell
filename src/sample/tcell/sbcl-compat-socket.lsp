;;; Copyright (c) 2010 Tasuku Hiraishi <tasuku@media.kyoto-u.ac.jp>
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

;;;; Wrapper for enabling SBCL to use Allegro CL Network interfaces.
;;;; Neeeds sc-misc.lsp

(defpackage "SOCKET"
  (:use "CL" "SB-BSD-SOCKETS" "MISC")
  (:export "MAKE-SOCKET" "ACCEPT-CONNECTION" "REMOTE-HOST" "REMOTE-PORT"
           "IPADDR-TO-HOSTNAME" "IPADDR-TO-DOTTED"))

(in-package :socket)

(deftype process () 'sb-thread:thread)
(deftype process-lock () 'sb-thread:mutex)

;;; Threads
(defun process-run-function (name fn &rest args)
  (sb-thread:make-thread #'(lambda () (apply fn args))
                         :name name))

(defun process-wait (name fn &rest args)
  (declare (ignore name))
  (cond 
   ((eq #'gate-open-p fn)
    (apply #'wait-for-gate-open args))
   (t
    (let ((ret))
      (loop
          until (setq ret (apply fn args))
          do (sleep *process-wait-interval*))
      ret))))

(defun process-wait-with-timeout (name timeout fn &rest args)
  (sb-ext:with-timeout timeout
    (apply #'process-wait name fn args)))

(defun process-kill (proc)
  (sb-thread:terminate-thread proc))

;;; Lock
(defun make-process-lock ()
  (sb-thread:make-mutex))

(defmacro with-process-lock ((&rest locks) &body body)
  `(sb-thread:with-mutex (,@locks) ,@body))

;;; Gates
(defstruct _gate
  state
  mutex
  condition)

(defun make-gate (open)
  (make-_gate :state open :mutex (sb-thread:make-mutex) :condition (sb-thread:make-waitqueue)))

(defun open-gate (gate)
  (sb-thread:with-mutex ((_gate-mutex gate))
    (setf (_gate-state gate) t)
    (sb-thread:condition-broadcast (_gate-condition gate))))

(defun close-gate (gate)
  (sb-thread:with-mutex ((_gate-mutex gate))
    (setf (_gate-state gate) nil)))

(defun gate-open-p (gate)
  (_gate-state gate))

(defun wait-for-gate-open (gate &optional timeout)
  (if timeout
      (sb-ext:with-timeout timeout
        (wait-for-gate-open gate))
    (loop
        until (gate-open-p gate)
        do (sb-thread:with-mutex ((_gate-mutex gate))
             (sb-thread:condition-wait (_gate-condition gate) (_gate-mutex gate))))))
