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

;;; Library for LW-SC (SC version of nestfunc.c)

(c-exp "#include<stdlib.h>")
(c-exp "#include<pthread.h>")

;;; 追加する型、マクロ定義
(deftype nestfn-t (ptr (fn (ptr char) (ptr char) (ptr void))))

(deftype closure-t struct
  (def fun nestfn-t)
  (def fr (ptr void)))
(deftype Align-t double)

(%defmacro mref-t (tp p)
  `(mref (cast (ptr ,tp) ,p)))

;;; nonlocalexit-goto に対応
(def (lw-call esp) (fn (ptr char) (ptr char))
  (def clos (ptr closure-t) (mref-t (ptr closure-t) esp))
  ;; 入れ子関数を呼び出す。
  ;; goto で脱出してきた場合は、脱出先のefp
  ;; (stackのreconstructionをefpのところのフレームで止める)
  ;; そうでない場合は、0 が返り値となる。 
  (return ((fref clos -> fun) esp (fref clos -> fr))))

;; for handling pthread
(def (struct func-arg)
    (def func (ptr (fn (ptr void) (ptr char) (ptr void))))
  (def arg (ptr void)))

(def (struct tho-frame)
    (def tmp-esp (ptr char)))

(def (thread-origin farg) (fn (ptr void) (ptr void))
  (def efp (ptr (struct tho-frame)))
  (def x (ptr (struct func-arg)) farg)
  (def func (ptr (fn (ptr void) (ptr char) (ptr void))) (fref x -> func))
  (def arg (ptr void) (fref x -> arg))
  (def ret (ptr void))
  (def new-esp (ptr char))
  (def estack (array char 65536))
  (def esp (ptr char) estack)

  (pthread-cleanup-push free farg)
  (= efp (cast (ptr (struct tho-frame)) esp))
  (= esp (cast (ptr char)
           (+ (cast (ptr Align-t) esp)
              (/ (- (+ (sizeof (struct tho-frame)) (sizeof Align-t)) 1)
                 (sizeof Align-t)))))
  (= (mref-t (ptr char) esp) 0)
  (= new-esp esp)
  (while (and (== (= ret (func new-esp arg)) (- (cast (ptr void) 0) 1))
              (!= (= (fref (mref efp) tmp-esp)
                     (mref-t (ptr char) esp))
                  0))
    (lw-call (fref (mref efp) tmp-esp)) (= new-esp (+ esp 1)))
  (pthread-cleanup-pop 1)
  (return ret)
  )
