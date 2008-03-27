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

;;; LW-SC example
(%ifndef* NF-TYPE
  (%defconstant NF-TYPE LW-SC)) ; one of (GCC LW-SC CL-SC XCC XCCCL)
(%include "rule/nestfunc-setrule.sh")

;;; goto による入れ子関数脱出例
;;; (複雑なケース)
;;; g -> f -> g1 -> g -> g1 ==goto==> g
(%rule (:nestfunc-sc1 :nestfunc-type :nestfunc-temp :nestfunc :untype))

(deftype size-t long)

(def (f pg) (fn int (ptr (lightweight void int)))
  (pg 0)
  (return 0))

;; 親関数への呼出しがあるときはプロトタイプ宣言が必須
(decl (g pg) (fn int (ptr (lightweight void int))))

(def (g pg) (fn int (ptr (lightweight void int)))
  (def L1 __label__)
  (def (g1 m) (lightweight void int)
    (if (> m 0)
        (goto L1)                       ; non-local exit
      (g g1)))
  
  (if (== pg 0)
      (return (f g1))
      (begin
       (g1 1)  ; goto先は後で呼び出されたg
       (return 0)))
  
  (label L1
   (if (== pg 0)
       (return 0)
       (pg 1)  ; goto先は最初に呼び出されたg
       )))
  
(def (main) (fn int)
  (return (g 0)))

