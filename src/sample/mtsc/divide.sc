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
  (%defconstant NF-TYPE GCC))           ; one of (GCC LW-SC CL-SC XCC XCCCL)
(%include "rule/multithread-setrule.sh")

(def (struct sync)
    (def n int)
  (def c cont))

(def (f-notify s) (fn void (ptr (struct sync)))
  (inc (fref s -> n))
  (if (<= (fref s -> n) 0)
      (thread-resume (fref s -> c))))

(def (f-wait s) (fn void (ptr (struct sync)))
  (dec (fref s -> n))
  (if (< (fref s -> n) 0)
      (thread-suspend c0
                      (= (fref s -> c) c0))))

(def (g x y) (fn int int int)
  (return (/ (+ x y) 2)))

(def (fw1 n1 n2 tab1 tab2 s1 s2)
    (fn void int int (ptr int) (ptr int) (ptr (struct sync)) (ptr (struct sync)))
  (let ((i int n1))
    (while (< i n2)
      (= (aref tab2 i)
         (g (aref tab1 i) (aref tab1 (- i 1))))
      (inc i)))
  (f-notify s1)
  (f-wait s2)
  (let ((i2 int n1))
    (while (< i2 n2)
      (= (aref tab1 i2)
         (g (aref tab1 (+ i2 1))
            (aref tab2 i2)))
      (inc i2)))
  (f-notify s1)
  (f-wait s2))

(def (divide-work n m) (fn void int int)
  (let ((tab1 (array int 1024))
        (tab2 (array int 1024))
        (s1 (struct sync) (struct 0 0))
        (s2 (struct sync) (struct 0 0))
        (i int 0))
    (while (< i m)
      (thread-create
       (fw1 1 (- (/ n 2) 1) tab1 tab2 (ptr s1) (ptr s2)))
      (fw1 (/ n 2) (- n 2) tab1 tab2 (ptr s2) (ptr s1))
      (inc i))))

(def (msc-main argc argv) (fn int int (ptr (ptr char)))
  (def n int 100)
  (def m int 1)
  (divide-work n m)
  (return 0))
