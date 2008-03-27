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

(decl (csym::printf) (fn int (ptr (const char)) va-arg))
(deftype size-t unsigned-int)

;;;; 1^2 + 2^2 + ... + n^2 の計算
;;;; 結果がlimitを超えたら -1 を返す。
(def (square-sum n pflag) (fn int int (ptr int))
  (def O-FLOW __label__)  ; この宣言はgccでも必須、2パス解析すればはずせる?
  (def limit int 8192)

  (def (square-sum-t n acc) (lightweight int int int)
    (def (square-t n acc) (lightweight int int int)
      (if (== n 0)
    (return acc)
    (if (> acc limit) 
        (goto O-FLOW)                   ; non-local exit
        (return (square-t (- n 1) (+ acc -1 (* 2 n)))))))

    (if (== n 0)
  (return acc)
  (if (> acc limit)
      (goto O-FLOW)
      (return
       (square-sum-t (- n 1) (+ acc (square-t n 0)))))))

  (= (mref pflag) 0)
  (return (square-sum-t n 0))
     
  (label @O-FLOW
    (begin
     (= (mref pflag) 1)
     (return -1))) )

(def (main) (fn int)
  (def flag int)
  (csym::printf "1^2 + ... + 100^2 =%d~%"
                (square-sum 100 (ptr flag)))
  (return flag) )
