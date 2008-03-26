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

;;; Designated-initializer example

(%rule ())

(decl (csym::printf) (csym::fn int (ptr (const char)) va-arg))

(deftype div-t struct
  (def quot int)
  (def rem int))

(def (union uni)
  (def any-member int)
  (def flt-member float))

(%defconstant MAX 10)

(def (struct sab)
  (def a (array int 3))
  (def b int))

(def (main) (fn int void)
  (def s1 (array char) "abc")
  (def t1 (array char 3) "abc")
  (def p (ptr char) "abc")
  (def nm (array (ptr (const char)) 2)
    (array ((aref-this 0) "member zero")
	   ((aref-this 1) "member one")))
  (def answer div-t
    (struct ((fref-this quot) 2) ((fref-this rem) -1)))
  (def w (array (struct sab) 3)
    (array ((fref (aref-this 0) a) (array 1))
	   ((aref (fref (aref-this 1) a) 0) 2)))
  (def a (array int MAX)
    (array 1 3 5 7 9 ((aref-this (- MAX 5)) 8) 6 4 2 0))
  (def u (union uni) (struct ((fref-this any-member) 42)))
  
  (return 0))
