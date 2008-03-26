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

;;; Utilities for hsc.rule

(defpackage "HIGHLEVEL-SC"
  (:nicknames "HSC")
  (:export :*finfo* :make-finfo :local-variables :add-local-variable :add-tmp-local-variable
           :function-head-p
           :main-p :make-add-decls :make-nestfunc :make-align
           :*sinfo* :*sinfo-list* :make-sinfo :add-sinfo :find-sinfo
           :add-member :make-struct-map :make-descriptor
           :sinfo-hsc-sid :sinfo-sdesc-id :sinfo-asize
           :*nestfunc-tag*
           )
  (:shadow cl:declaration)
  (:use "RULE" "CL" "SC-MISC"))

(in-package "HSC")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Dynamic variables & parameters
(defvar *finfo* nil)
(defvar *sinfo* nil)
(defvar *sinfo-list* (list))

(defparameter *nestfunc-tag* ~fn)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; function information
(defstruct finfo
  variables                             ; list of (<id> . <type>)
  tmp-variables                         ; 追加された一時変数 list of (<id> . <type>)
  (function-head t)                     ; 関数の先頭のブロック通過時にnilにする
  main-p
  )

(defun local-variables (&optional (finfo *finfo*))
  (finfo-variables finfo))

(defun get-id (cid-or-id)
  (if (symbolp cid-or-id)
      cid-or-id
    (rule:generate-id cid-or-id)))

(defun add-local-variable (cid-or-id type &optional (finfo *finfo*))
  (let ((id (get-id cid-or-id)))
    (push (cons id type)
          (finfo-variables finfo))
    id))
(defun add-tmp-local-variable (cid-or-id type &optional (finfo *finfo*))
  (let ((id (get-id cid-or-id)))
    (push (cons id type)
          (finfo-tmp-variables finfo))
    id))

(defun main-p (&optional (finfo *finfo*))
  (finfo-main-p finfo))

(defun make-add-decls (&optional (finfo *finfo*))
  (loop for (var . type) in (finfo-tmp-variables finfo)
      collect ~(def ,var ,type 0)))

(defun function-head-p (&optional (finfo *finfo*))
  (if (finfo-function-head finfo)
      (progn
        (setf (finfo-function-head finfo) nil)
        t)
    nil))

(defun make-nestfunc (&optional (finfo *finfo*))
  ~(def (scan1) (,*nestfunc-tag* void void)
        ,@(loop for (var . type) in (finfo-variables finfo)
              when (rule::ref-type-p type)
              collect ~(= ,var (move ,var)))
        (scan0)
        (return)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; struct information
(defstruct sinfo
  id                                    ; 元の構造体名
  members                               ; list of (<id> . <type>) 
  hsc-sid                               ; 変換後の構造体名
  data-sid                              ; データ部の構造体名（今の実装ではidと同一）
  smap-id                               ; マップ配列の変数名
  sdesc-id                              ; 構造体ごとのdesc情報を保持する変数名
  )

(defun sinfo-asize (&optional (sinfo *sinfo*))
  (make-align ~(sizeof (struct ,(sinfo-hsc-sid sinfo)))
              ~(sizeof align-t)) )
(defun make-align (exp align-size)
  ~(* (/ (+ ,exp ,align-size -1)
	 ,align-size)
      ,align-size))

(defun sinfo-map-length (&optional (sinfo *sinfo*))
  ~(/ (sizeof ,(sinfo-smap-id sinfo)) (sizeof size-t)) )

;; make expression for aligned size

;; sinfo追加
(defun add-sinfo (&optional (sinfo *sinfo*))
  (push sinfo *sinfo-list*))

;; sinfoをidで検索
(defun find-sinfo (id)
  (find id *sinfo-list* :key #'sinfo-id))

(defun add-member (id type &optional (sinfo *sinfo*))
  (push (cons id type) (sinfo-members sinfo)))

;; オブジェクト内の参照の位置を示す配列式
(defun make-struct-map (&optional (sinfo *sinfo*))
  (let ((vars (sinfo-members sinfo))
        (hsc-sid (sinfo-hsc-sid sinfo))
        (data-sid (sinfo-data-sid sinfo)))
    (cons ~array
          (loop for (id . type) in vars
              when (rule::ref-type-p type)
              collect ~(+ ,(make-offsetof hsc-sid ~data)
                          ,(make-offsetof data-sid id))))
    ))

(defun make-offsetof (struct-id member-id)
  ~(cast size-t
         (ptr
          (fref (mref (cast (ptr (struct ,struct-id))
                            0))
                ,member-id)))
  )

;; オブジェクト情報構造体式を作る
(defun make-descriptor (&optional (sinfo *sinfo*))
  (let ((hsc-sid (sinfo-hsc-sid sinfo))
        (smap-id (sinfo-smap-id sinfo)))
    ~(struct
      (sizeof (struct ,hsc-sid))        ; size
      ,(sinfo-asize sinfo)              ; aligned-size
      ,(sinfo-map-length sinfo)         ; length of map
      ,smap-id)                         ; map
    ))

