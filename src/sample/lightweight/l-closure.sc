;;; Copyright (c) 2008-2018 Tasuku Hiraishi <tasuku@media.kyoto-u.ac.jp>
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

;;; LW-SC example

(%ifndef* NF-TYPE
  (%defconstant NF-TYPE CL-SC2)) ; one of (GCC LW-SC CL-SC CL-SC2 XCC XCCCL)
(%include "rule/nestfunc-setrule.sh")

(deftype size-t long)

(def (h i g) (fn int int (ptr (NESTFN int int)))
  (return (g (g i))))

(def (foo a b) (fn int int int)
  (def x int 0)
  (def y int 0)
  (def (g1 b) (NESTFN int int)
    (inc x)
    (csym::printf "g1_in_foo: a=%d g1_in_foo:b=%d x=%d y=%d~%" a b x y)
    (return (+ a b)))
  (csym::printf "foo: a=%d foo:b=%d x=%d y=%d~%" a b x y)
  (= y (h 10 g1))
  (csym::printf "foo: a=%d foo:b=%d x=%d y=%d~%" a b x y)
  (return (+ x y))) 

(def (main) (fn int void)
  (return (foo 1 3)))
