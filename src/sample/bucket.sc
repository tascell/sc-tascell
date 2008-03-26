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

(c-exp "#include<stdio.h>")
(c-exp "#include<stdlib.h>")
(c-exp "#include<ctype.h>")

(%defconstant Y-MAX 50)
(%defconstant X-MAX 80)
(%defconstant ALPH-MAX 100)

(%defmacro defswap (fname texp)
   `(def (,fname a b) (fn void (ptr ,texp) (ptr ,texp))
       (def buffer ,texp)
       (= buffer (mref a))
       (= (mref a) (mref b))
       (= (mref b) buffer)
       (return)))


(defswap swap int)
(defswap swap_w (ptr char))

((struct bucket_part)
   (word (array (ptr char) ALPH-MAX))
   (num int))

(deftype bucket (array (struct bucket_part) 26))

((bucket_sort fp bucket) (fn void (ptr FILE) (ptr (struct bucket_part)))
 (let ((defs int i n p)
       (w (array char (+ X-MAX 1)))
       (defs char ch first))

   (for ((def i int 0) (<= i 25) (inc i))
     (= (fref (aref bucket i) num) 0))

   (= i 1)
   (loop
      (while (not (isalpha (= ch (getc fp))))
        (if (== ch #\Linefeed)
            (begin
               (inc i)
               (if (> i Y-MAX) (return))))
        (if (== ch EOF) (return)))

      (= first (tolower ch))
      (= n (- first #\a))

      (if (>= (fref (aref bucket n) num) ALPH-MAX)
         (begin
            (printf "more than %d words begining with %c" ALPH-MAX first)
            (fclose fp)
            (exit 1)))

      (= p 0)
      (do-while (and (!= (= ch (getc fp)) #\,)
                     (!= ch #\Space)
                     (!= ch #\.)
	             (!= ch #\Linefeed)
                     (!= ch EOF))
         (if (isalpha ch)
             (begin
               (= (mref (+ w p)) (tolower ch))
               (inc p))))

      (= (mref (+ w p)) 0)

      (= (aref (fref (aref bucket n) word) (fref (aref bucket n) num))
         (cast (ptr char) (malloc (+ (strlen w) 1))))
      (strcpy (aref (fref (aref bucket n) word) (fref (aref bucket n) num)) w)

      (inc (fref (aref bucket n) num))

      (if (== ch #\Linefeed)
          (begin
            (inc i)
            (if (> i Y-MAX) (return)))))

   (exit 0)
   (return)))

((bubble_sort_w bucket) (fn void (ptr (struct bucket_part)))
  (def ch int)

  (for ((= ch 0) (<= ch 25) (inc ch))
     (let ((defs int i j k p (lenmax 0))
           (len (array int ALPH-MAX))
           (c (array char 2)))
       
       (for ((= i 0) (< i (fref (aref bucket ch) num)) (inc i))
          (if (> (= (aref len i)
                    (strlen (aref (fref (aref bucket ch) word) i)))
                 lenmax)
              (= lenmax (aref len i))))

       (for ((= p (- lenmax 1)) (>= p 1) (dec p))
          (for ((= i (- (fref (aref bucket ch) num) 1)) (> i 0) (-- i))
             (for ((= j 0) (< j i) (inc j))
                (for ((= k 0) (<= k 1) (inc k))
                   (if (< (aref len (+ j k)) (+ p 1))
                       (= (aref c k) 0)
                       (= (aref c k) 
                          (aref (fref (aref bucket ch) word) (+ j k) p))))
                (if (> (aref c 0) (aref c 1))
                  (begin
                    (swap_w (ptr (aref (fref (aref bucket ch) word) j))
                            (ptr (aref (fref (aref bucket ch) word) (+ j 1))))
                    (swap (+ len j) (+ len j 1)))))))))

  (return))

((output_words bucket) (fn void (ptr (struct bucket_part)))
   (defs int ch i)
   (for ((= ch 0) (<= ch 25) (inc ch))
      (for ((= i 0) (< i (fref (aref bucket ch) num)) (++ i))
         (printf "%s~%" (aref (fref (aref bucket ch) word) i)))))

((main argc argv) (fn int int (array (ptr char)))
   (let ((mybucket bucket)
         (fp (ptr FILE)))
      
      (if (!= argc 2)
          (begin
             (printf "Usage:%s input_file~%" (aref argv 0))
             (exit 0)))

      (if (== (= fp (fopen (aref argv 1) "r")) NULL)
          (begin
             (printf "cannot open %s~%" (aref argv 1))
             (exit 0)))

      (bucket_sort fp mybucket)
      (fclose fp)

      (bubble_sort_w mybucket)
  
      (output_words mybucket))) 