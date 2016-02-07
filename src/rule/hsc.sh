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

;;; Header file for HSC, automatically included by hsc.rule

(%ifndef* NESTFN
  (%defconstant NESTFN fn))
(deftype sht (ptr (NESTFN void void)))

;;;
;; (deftype size-t long)
(deftype align-t double)

;;; (def d desc-t) の下位2ビットに埋め込む
(%defconstant TYPE-STRUCT 0)
(%defconstant TYPE-REF-ARRAY 1)
(%defconstant TYPE-NONREF-ARRAY 3)

;;; 構造体<sid>の構造（hsc.ruleより）
;; ~(def (struct ,data-sid) ,@sbody-ret) ; 元の構造体情報は <sid>_data
;; ~(def (struct ,sid)     ; <sid> --> {<type_info>, <ptr to descriptor>, <sid>_data}
;;   (def d desc-t)        ; (ptr (struct desc-struct))
;;   (def data (struct ,data-sid))

;; (deftype size-t unsigned-int)

(def (struct desc-struct)
  (decl size size-t)
  (decl asize size-t) 
  (decl fli-len int)
  (decl fli (ptr size-t)))
(deftype descriptor (struct desc-struct))
(deftype desc-t (ptr descriptor))

(def (struct array-object)
  (decl d desc-t)   ; 下位2bit以外はダミー
  (decl length int)
  (decl el-size size-t)
  (decl size size-t)
  (decl asize size-t)
  (decl body (array char 0)))
  
;;;
(extern-decl (getmem scan size) (fn (ptr void) sht size-t))
(extern-decl (csym::move vp) (csym::fn (ptr void) (ptr void)))
