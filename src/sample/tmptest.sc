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

;;; test for temp.rule
(%rule (:sc1-to-sc0 :type-info :temp :untype))

(def (g x) (fn int int)
  (return (* x x)))

(def (h x) (fn int int)
  (return (+ x x)))

(def (i x) (fn int int)
  (return (- x (* 2 x))))


(def (f1 x) (fn int int)
  (return (if-exp (> (g x) 100) 
		  (h x)
		  (i x))))

(def (f2 x) (fn int int)
  (def y int)
  (if (and  (> (g x) 100)
	    (< (h x) 200))
      (if (or (> (g x) 200)
	      (> (h x) 300))
	  (return (g (i (h x))))
	  (return (= y 300)))
      (return (exps (= y (g x)) (= y (h y)) (= x (g (i (h y))))))))
      
(def (f3 x) (fn int int)
  (def i int)
  (for ((= i 0) (> x (f2 i)) (inc i))
    (if (> i 10)
	(continue))
    (if (> i 100)
	(break)
	(+= i 3)))
  (return (f1 i)))
