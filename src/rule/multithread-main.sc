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

;;; main function for MT-SC, automatically included by multithread.rule

(%ifndef NESTFUNC-TAG
  ((%defconstant NESTFUNC-TAG fn))) 

(def (main argc argv) (fn int int (ptr (ptr char)))
     (def MAIN-RET --label--)
     (def ln int 0)
     (def msc-main-ret int)
     (def (main-c cp rsn) (NESTFUNC-TAG (ptr void) thst-ptr reason)
	  (switch rsn
		  (case rsn-cont)
		  (switch ln
			  (case 0) (goto L0)
			  (case 1) (goto L1))
		  (return)
		  (case rsn-retval)
		  (return (cast (ptr void) (ptr msc-main-ret))))
	  (return)
	  (label L0 (= ln 1))
	  (= msc-main-ret (msc-main main-c argc argv))
	  (label L1
		 (goto MAIN-RET)))
     (= (fref (mref thst-top) c) main-c)
     (= (fref (mref thst-top) stat) thr-runnable)
     (inc thst-top)
     (do-while 1 (scheduling))
     (label MAIN-RET
	    (return msc-main-ret)))
