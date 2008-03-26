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

;;; MT-SC example
(%ifndef* NF-TYPE
  (%defconstant NF-TYPE GCC)) ; one of (GCC LW-SC CL-SC XCC XCCCL)
(%include "rule/multithread-setrule.sh")
(%rule (:multithread-sc1 :multithread-type :multithread-temp :multithread :untype))

(c-exp "#include<stdio.h>")
(c-exp "#include<stdlib.h>")

(def (pfib n) (fn int int)
  (def x int) (def y int) (def nn int 0) (def c cont 0)
  (if (<= n 2) 
      (return 1)
      (begin
        (thread-create nil
          (= x (pfib (- n 1)))
	  (inc nn)
	  (if (== nn 0)
	      (thread-resume c)))
	(= y (pfib (- n 2)))
	(= nn (- nn 1))
	(if (< nn 0)
	    (thread-suspend c0
	      (= c c0)))
	(return (+ x y)))))

(def (msc-main argc argv) (fn int int (ptr (ptr char)))
  (def n int 4)
  (if (> argc 1)
      (= n (c-exp "atoi(~A)" (aref argv 1))))  
  (c-exp "printf(~A, ~A, ~A);" "fib(%d)=%d~%" n (pfib n))
  (return n))

