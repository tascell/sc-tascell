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

;;; Header file for LW-SC
(c-exp "#include<stdio.h>")
(c-exp "#include<pthread.h>")

;;; 追加する型、マクロ定義
(deftype nestfn-t (ptr (fn (ptr char) (ptr char) (ptr void))))

(deftype closure-t struct
  (def fun nestfn-t)
  (def fr (ptr void)))

(%defmacro SPECIAL (tp)
  `(- (cast ,tp 0) 1))

;; (deftype size-t unsigned-int)
(deftype Align-t double)

(%defmacro aligned-add (base size)
  `(cast (ptr char)
     (+ (cast (ptr Align-t) ,base)
        (/ (+ ,size (sizeof Align-t) -1) (sizeof Align-t)))))

(%defmacro aligned-sub (base size)
  `(cast (ptr char)
     (- (cast (ptr Align-t) ,base)
        (/ (+ ,size (sizeof Align-t) -1) (sizeof Align-t)))))

(%defmacro mref-t (tp p)
  `(mref (cast (ptr ,tp) ,p)))

(%defmacro push-arg (tp v argp)
  `(exps (= (mref-t ,tp ,argp) ,v)
         (= ,argp (aligned-add ,argp (sizeof ,tp)))))

(%defmacro pop-arg (tp argp)
  `(exps (= ,argp (aligned-sub ,argp (sizeof ,tp)))
         (mref-t ,tp ,argp)))

#+obsolete                              ; old version (without goto support)
(def (lw-call esp) (fn void (ptr char))
  (def tmp-esp (ptr char))
  (def clos (ptr closure-t) (mref-t (ptr closure-t) esp))
  (def new-esp (ptr char) esp)
  (while (= tmp-esp ((fref clos -> fun) new-esp (fref clos -> fr)))
    (lw-call tmp-esp)
    (= new-esp (+ esp 1))))

(decl (lw-call esp) (fn (ptr char) (ptr char)))

;;; nonlocalexit-goto に対応させてみる．
#+obsolete                              ; moved to nestfunc.sc
(def (lw-call esp) (fn (ptr char) (ptr char))
  (def clos (ptr closure-t) (mref-t (ptr closure-t) esp))
  ;; 入れ子関数を呼び出す。
  ;; goto で脱出してきた場合は、脱出先のefp,
  ;; そうでない場合は、0 が返り値となる。 
  (return ((fref clos -> fun) esp (fref clos -> fr))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; pthread-create 対応用
(def (struct func-arg)
    (def func (ptr (fn (ptr void) (ptr char) (ptr void))))
  (def arg (ptr void)))

(decl (thread-origin farg) (fn (ptr void) (ptr void)))
