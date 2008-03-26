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

;;; Example of %defmacro, %ifdef, %error

(decl printf (fn int (const int) va-arg))

(%defmacro ntimes (n &body form)
    `(for ((def i int 0) (< i ,n) (inc i)) ,@form)) 

(main (fn int)
  (ntimes 10 
    (def str (array char 3 10) 
               (array "abcdefg~%" "hijklmn~%" "opqrstu~%"))
    (printf "~%")
    (ntimes 3
      (printf (aref str i)))))


(%defmacro defswap (fname texp)
   `(def (,fname a b) (fn void (ptr ,texp) (ptr ,texp))
       (def buffer ,texp)
       (= buffer (ptr a))
       (= (ptr a) (ptr b))
       (= (ptr b) buffer)
       (return)))


(defswap swap int)
(defswap swap-w (ptr char))

(%defmacro ggtype () '(ptr (fn (ptr void) int int)))

(decl (gg ff) (fn (ggtype) (ggtype)))
(deftype gg-t (fn (ggtype) (ggtype)))

(%ifdef ggtype
	((%error "ggtype defined"))
	((%error "ggtypee undefined")))
