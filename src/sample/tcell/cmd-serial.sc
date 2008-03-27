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

;;;; worker4.sc以降より
;;;; コマンドのシリアライズ・デシリアライズ関数
(c-exp "#include <pthread.h>")
(%include "rule/nestfunc-setrule.sh")

(c-exp "#include<stdio.h>")
(c-exp "#include<stdlib.h>")
(%include "worker4.sh")


;; (enum node)の値が配列添字に対応
(def cmd-strings (array (ptr char))
  (array "task" "rslt" "treq" "none" "rack" "exit" 0))

;; 文字列の最初の空白以外の位置を返す
(def (csym::skip-whitespace str) (fn (ptr char) (ptr char))
  (def ch char)
  (= ch (mref str))
  (for ((or (== ch #\Space) (== ch #\Newline))
        (= ch (mref (++ str)))))
  (return str))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; (struct cmd)構造体表示
(def (csym::print-cmd pcmd) (fn void (ptr (struct cmd)))
  (defs int i j)
  (csym::fprintf stderr "cmd.w: %d~%" pcmd->w)
  (csym::fprintf stderr "cmd.c: %d~%" pcmd->c)
  (csym::fprintf stderr "cmd.node: %d~%" pcmd->node)
  (for ((= i 0) (< i MAXCMDC) (inc i))
       (csym::fprintf stderr "cmd.v[%d]:" i)
       (for ((= j 0) (< j ARG-SIZE-MAX) (inc j))
            (csym::fprintf stderr "%3d " (aref pcmd->v i j)))
       (csym::fprintf stderr "~%"))
  )


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;
;; コマンド->文字列 (retruns 書いた文字数)
(def (csym::serialize-cmdname buf w) (fn int (ptr char) (enum command))
  (def p (ptr char) buf)
  (if (and (>= w 0) (< w WRNG))
      (begin
       (csym::strcpy p (aref cmd-strings w))
       (+= p (csym::strlen (aref cmd-strings w)))
       (return (- p buf)))
    (begin
     (= (mref p) #\NULL)
     (return 0))))

;; 文字列->コマンド (returns 読んだ文字数)
(def (csym::deserialize-cmdname buf str) (fn int (ptr (enum command)) (ptr char))
  (def i int)
  (def p (ptr char))
  (def cmdstr (ptr char))
  (= p str)
  (for ((= i 0) (= cmdstr (aref cmd-strings i)) (inc i))
       (if (== 0 (csym::strncmp p cmdstr 4))
           (begin
            (= (mref buf) i)            ; コマンドをセット
            (+= p 4)
            (= p (csym::skip-whitespace p))
            (return (- p str))
            (return i))))
  (= (mref buf) WRNG)
  (return 0))

;;;;;;;;;;
;; [整数|ノード]列->文字列 (returns 書いた文字数)
(def (csym::serialize-arg buf arg) (fn int (ptr char) (ptr (enum node)))
  (def p (ptr char) buf)
  (def node (enum node))
  (def i int)
  (for ((= i 0) (!= TERM (= node (aref arg i))) (inc i))
       (cond
        ((== ANY node)
         (csym::strcpy p "any")
         (+= p 3))
        ((== PARENT node)
         (= (mref (inc p)) #\p))
        (else
         (+= p (csym::sprintf p "%d" node))))
       (= (mref (inc p)) #\:))
  (= (mref (-- p)) #\NULL)
  (return (- p buf)))

;; 文字列->ノード
(def (csym::deserialize-node str) (fn (enum node) (ptr char))
  (cond
   ((== #\p (aref str 0))
    (return PARENT))
   ((== 0 (csym::strncmp str "any" 3))
    (return ANY))
   (else
    (return (csym::atoi str)))))

;; 文字列->[整数|ノード]列 (returns 読んだ文字数)
(def (csym::deserialize-arg buf str) (fn int (ptr (enum node)) (ptr char))
  (defs (ptr char) p0 p1)
  (def ch int)
  (def pnode (ptr (enum node)))
  (= p0 str) (= p1 str)
  (= pnode buf)
  (for (1 (inc p1))
    (= ch (mref p1))
    (cond 
     ((or (== ch #\:) (== ch #\Space) (== ch #\Newline) (== ch #\NULL))
      (= (mref p1) #\NULL)
      (= (mref (inc pnode)) (csym::deserialize-node p0)) ; p0--p1までdeserialize
      (= (mref p1) ch)
      (if (!= ch #\:)                   ; NULL, Space, Newline
          (break))
      (= p0 (+ 1 p1)))))
  (= (mref pnode) TERM)  ; 番兵
  (= p1 (csym::skip-whitespace p1))
  (return (- p1 str)))

;;;;;;;;;;
;; コマンド->文字列 (return 書いた文字数)
(def (csym::serialize-cmd buf pcmd) (fn int (ptr char) (ptr (struct cmd)))
  (def p (ptr char))
  (def ret int)
  (def i int)
  (= p buf)
  ;; コマンド名
  (if (not (= ret (csym::serialize-cmdname p pcmd->w)))
      (begin
       (csym::fprintf stderr "Serialize-cmd failed.~%")
       (csym::print-cmd pcmd)
       (csym::exit 1)))
  (+= p ret) (= (mref (inc p)) #\Space)
  ;; 引数
  (for ((= i 0) (< i pcmd->c) (inc i))
    (+= p (csym::serialize-arg p (aref pcmd->v i)))
    (= (mref (inc p)) #\Space))
  ;;
  (= (mref (-- p)) #\NULL)
  (return (- p buf)))

;; 文字列->コマンド (return 読んだ文字数)
(def (csym::deserialize-cmd pcmd str) (fn int (ptr (struct cmd)) (ptr char))
  (def p (ptr char))
  (def i int)
  (= p str)
  ;; コマンド名
  (+= p (csym::deserialize-cmdname (ptr pcmd->w) p))
  (if (== pcmd->w WRNG) (return (- p str)))
  ;; 引数
  (for ((= i 0) (and (mref p) (< i MAXCMDC)) (inc i))
       (+= p (csym::deserialize-arg (aref pcmd->v i) p)))
  ;; 引数の数
  (= pcmd->c i)
  (return (- p str)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; ノード列コピー (returns TERMを除くノード数)
(def (csym::copy-address dst src) (fn int (ptr (enum node)) (ptr (enum node)))
  (def i int)
  (for ((= i 0) (!= TERM (aref src i)) (inc i))
    (= (aref dst i) (aref src i)))
  (= (aref dst i) TERM)
  (return i))
