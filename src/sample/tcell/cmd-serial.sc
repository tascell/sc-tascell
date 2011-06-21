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

;;;; コマンドのシリアライズ・デシリアライズ関数
(c-exp "#include <pthread.h>")
(%include "rule/nestfunc-setrule.sh")

(c-exp "#include<stdio.h>")
(c-exp "#include<stdlib.h>")
(c-exp "#include<string.h>")
(%include "worker.sh")


;; 文字列の最初の空白以外の位置を返す
(def (csym::skip-whitespace str) (fn (ptr char) (ptr char))
  (def ch char)
  (= ch (mref str))
  (for ((or (== ch #\Space) (== ch #\Newline))
        (= ch (mref (++ str)))))
  (return str))

;; 文字列の最初の空白の位置を返す
(def (csym::skip-notwhitespace str) (fn (ptr char) (ptr char))
  (def ch char)
  (= ch (mref str))
  (for ((not (or (== ch #\Space) (== ch #\Newline)))
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
;; (enum command)の値が配列添字に対応
;; ここを変えると deserilalize-cmdname の「ゆるい文字列比較」も変える必要があるので注意
(def cmd-strings (array (ptr char))
  (array "task" "rslt" "treq" "none" "back" "rack" "dreq" "data"
         "bcst" "bcak" "stat" "verb" "exit" "leav" "lack" "abrt" "cncl" "wrng" 0))

;; コマンド->文字列 (returns 書いた文字数)
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
  ;; ゆるい文字列比較ver.
  (begin
   (switch (mref (inc p))
     (case #\t)
     (switch (mref (inc p))
       (case #\a) (= (mref buf) TASK) (break)
       (case #\r) (= (mref buf) TREQ) (break)
       (default) (= (mref buf) WRNG) (return 0))
     (break)

     (case #\d)
     (switch (mref (inc p))
       (case #\r) (= (mref buf) DREQ) (break)
       (case #\a) (= (mref buf) DATA) (break)
       (default) (= (mref buf) WRNG) (return 0))
     (break)
     
     (case #\r)
     (switch (mref (inc p))
       (case #\s) (= (mref buf) RSLT) (break)
       (case #\a) (= (mref buf) RACK) (break)
       (default) (= (mref buf) WRNG) (return 0))
     (break)
     
     (case #\b)
     (switch (mref (inc p))
       (case #\a) (= (mref buf) BACK) (break)
       (case #\c)
       (switch (mref (inc p))
         (case #\s) (= (mref buf) BCST) (break)
         (case #\a) (= (mref buf) BCAK) (break)
         (default) (= (mref buf) WRNG) (return 0))
       (break)
       (default) (= (mref buf) WRNG) (return 0))
     (break)
     
     (case #\n) (= (mref buf) NONE) (break)
     (case #\s) (= (mref buf) STAT) (break)
     (case #\v) (= (mref buf) VERB) (break)
     (case #\e) (= (mref buf) EXIT) (break)
     (case #\a) (= (mref buf) ABRT) (break)
     (case #\c) (= (mref buf) CNCL) (break)
  
     (case #\l)
     (switch (mref (inc p))
       (case #\e) (= (mref buf) LEAV) (break)
       (case #\a) (= (mref buf) LACK) (break)
       (default) (= (mref buf) WRNG) (return 0))
     (break)
          
     (default) (= (mref buf) WRNG) (return 0))
   (= p (csym::skip-notwhitespace p))
   (= p (csym::skip-whitespace p))
   (return (- p str)))
  #+comment                             ; 厳密に文字列比較ver.
  (begin
   (for ((= i 0) (= cmdstr (aref cmd-strings i)) (inc i))
     (if (== 0 (csym::strncmp p cmdstr 4))
         (begin
          (= (mref buf) i)              ; コマンドをセット
          (+= p 4)
          (= p (csym::skip-whitespace p))
          (return (- p str))
          (return i))))
   (= (mref buf) WRNG)
   (return 0))
  )

;;;;;;;;;;
;; [整数|ノード]列->文字列 (returns 書いた文字数)
(def (csym::serialize-arg buf arg) (fn int (ptr char) (ptr (enum addr)))
  (def p (ptr char) buf)
  (def addr (enum addr))
  (def i int)
  (for ((= i 0) (!= TERM (= addr (aref arg i))) (inc i))
       (cond
        ((== ANY addr)
         (csym::strcpy p "any")
         (+= p 3))
        ((== PARENT addr)
         (= (mref (inc p)) #\p))
        ((== FORWARD addr)
         (= (mref (inc p)) #\f))
        (else
         (+= p (csym::sprintf p "%d" addr))))
       (= (mref (inc p)) #\:))
  (if (== i 0) (inc p))
  (= (mref (-- p)) #\NULL)
  (return (- p buf)))

;; 文字列->ノード
(def (csym::deserialize-addr str) (fn (enum addr) (ptr char))
  (cond
   ((== #\p (aref str 0))
    (return PARENT))
   ((== #\f (aref str 0))
    (return FORWARD))
   ((== 0 (csym::strncmp str "any" 3))
    (return ANY))
   (else
    (return (csym::atoi str)))))

;; 文字列->[整数|ノード]列 (returns 読んだ文字数)
(def (csym::deserialize-arg buf str) (fn int (ptr (enum addr)) (ptr char))
  (defs (ptr char) p0 p1)
  (def ch int)
  (def paddr (ptr (enum addr)))
  (= p0 str) (= p1 str)
  (= paddr buf)
  (for (1 (inc p1))
    (= ch (mref p1))
    (cond 
     ((or (== ch #\:) (== ch #\Space) (== ch #\Newline) (== ch #\NULL))
      (= (mref p1) #\NULL)
      (= (mref (inc paddr)) (csym::deserialize-addr p0)) ; p0--p1までdeserialize
      (= (mref p1) ch)
      (if (!= ch #\:)                   ; NULL, Space, Newline
          (break))
      (= p0 (+ 1 p1)))))
  (= (mref paddr) TERM)  ; 番兵
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
(def (csym::copy-address dst src) (fn int (ptr (enum addr)) (ptr (enum addr)))
  (def i int)
  (for ((= i 0) (!= TERM (aref src i)) (inc i))
    (= (aref dst i) (aref src i)))
  (= (aref dst i) TERM)
  (return i))

;; ノード列比較 (同一なら非0)
(def (csym::address-equal adr1 adr2) (fn int (ptr (enum addr)) (ptr (enum addr)))
  (def i int)
  (for ((= i 0) (< i MAXCMDC) (inc i))
    (if (!= (aref adr1 i) (aref adr2 i)) (return 0))
    (if (== TERM (aref adr1 i)) (return 1)))
  (return 1))
