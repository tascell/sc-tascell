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

;;;; Utility functions and macros

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#+sc-system
(progn
  (provide "SC-MISC")
  (eval-when (:compile-toplevel :load-toplevel :execute)
    (require "SC-DECL" "sc-decl.lsp"))
  
  (in-package "SC-MISC")
  )

;;;;;

;;; 新しい変数名を作る（マクロ記述用）
(eval-when (:compile-toplevel :load-toplevel :execute)
  (defmacro with-fresh-variables (varlist &body body)
    `(let ,(mapcar
            #'(lambda (var) (declare (symbol var)) `(,var (gensym ,(symbol-name var))))
            (if (symbolp varlist) (list varlist) varlist))
       ,@body)))

;;; Anaphoric Variants (from "On Lisp")
(defmacro aif (test-form then-form &optional else-form)
  `(let ((it ,test-form))
     (declare (ignorable it))
     (if it ,then-form ,else-form)))

(defmacro acase (test-form &body body)
  `(let ((it ,test-form))
     (declare (ignorable it))
     (case it ,@body)))

(defmacro awhen (test-form &body body)
  `(aif ,test-form
       (progn ,@body)))

(defmacro aunless (test-form &body body)
  `(aif ,test-form
       it
     (progn ,@body)))

(defmacro awhile (expr &body body)
  `(do ((it ,expr ,expr)) ((not it))
     ,@body))

(defmacro aand (&rest args)
  (cond ((null args) t)
        ((null (cdr args)) (car args))
        (t `(aif ,(car args) (aand ,@(cdr args))))))

(defmacro acond (&rest clauses)
  (if (null clauses)
      nil
    (let ((cl1 (car clauses))
          (sym (gensym)))
      `(let ((,sym ,(car cl1)))
         (if ,sym
             (let ((it ,sym))
               (declare (ignorable it))
               ,@(cdr cl1))
           (acond ,@(cdr clauses)))))))

;;; コマンドラインの実行（実装依存部分を吸収）
#+(or allegro kcl ecl cmu clisp)
(defun command-line (command &key args verbose other-options)
  (declare (simple-string command) (list verbose) (list other-options))
  (let ((cat-string (strcat (cons command args) #\Space)))
    (prin1 cat-string verbose)
    (fresh-line verbose)
    #+allegro(apply #'excl:run-shell-command cat-string :wait t
                    other-options)
    #+kcl(apply #'system cat-string other-options)
    #+ecl(apply #'si::system cat-string other-options)
    #+(or cmu clisp)
    (apply #'ext:run-program command
           :arguments args
           :wait t
           other-options)
    ))

;;;;; ファイル操作

;;; *readtable* を一時的に切替え
(defmacro with-readtable (readtable &body forms)
  `(let ((*readtable* ,readtable))
     ,@forms))

;;; *package* を一時的に切替え
(defmacro with-package (package &body forms)
  (let ((package-var (gensym "package")))
    `(let ((,package-var (find-package ,package)))
       (unless ,package-var
         (error "Package ~S not found." ,package-var))
       (let ((*package* ,package-var))
         ,@forms)) ))

;;; pname のパッケージが見つからなければ新たに作って返す
(defun find-package2 (pname &rest args)
  (or (find-package pname)
      (apply #'make-package pname args)))

;;; ファイルをS式として読み込む
(defun read-file (file-name
                  &key
                  ((:package *package*) *package*)
                  ((:readtable *readtable*) *readtable*)
                  &aux ret)
  (unless (probe-file file-name)
    (error "Can't open ~S." file-name))
  (with-open-file (istream file-name :direction :input)
    (do ((y (read istream nil 'eof) (read istream nil 'eof)))
        ((eq y 'eof) (nreverse ret))
      (push y ret))))

;;; ファイルを文字列として読み込む
(defun read-file-as-string (file-name)
  (with-open-file (istream file-name :direction :input)
    (input-buffer-to-string istream)))

#+allegro (defparameter *eol-convention* :unix)
;;; ファイルにS式または文字列を書き込む
(defun write-file (file-name list 
                   &key
                   (overwrite nil) (write-string nil)
                   ((:print-case *print-case*) *print-case*)
                   ((:package *package*) *package*)
                   ((:readtable *readtable*) *readtable*)
                   #+allegro ((:eol-convention eol-conv) *eol-convention*)
                   )
  (when (probe-file file-name)
    (if overwrite
        (delete-file file-name)
      (error "~S already exists." file-name)))
  (with-open-file (ostream file-name :direction :output)
    #+allegro (setf (excl:eol-convention ostream) eol-conv)
    (if write-string
        (princ list ostream)
      (dolist (elm list t)
        (format ostream "~S" elm)
        (terpri ostream)))))

;; eofか?
(defun eof-p (stream)
  (let ((ch (read-char-no-hang stream nil t)))
    (cond
     ((eq t ch)
      t)
     ((characterp ch)
      (unread-char ch stream)
      nil)
     (t
      nil))))

;; ファイルの1行ごとの処理
(defmacro do-text-line ((line istream &optional ret) &body body)
  `(do ((,line (read-line ,istream nil) (read-line ,istream nil)))
       ((null ,line) ,ret)
     ,@body))

;; input-stream のバッファにたまっているものに対しての処理
(defmacro do-input-stream-buffer ((var istream &optional ret) &body body)
  (with-fresh-variables (is-var)
    `(let ((,is-var ,istream))
       (do ((,var (read-char-no-hang ,is-var nil nil)
                  (read-char-no-hang ,is-var nil nil)))
           ((null ,var) ,ret)
         ,@body))))

;; input-stream のバッファにたまっているものをoutput-streamに流す
(defun input-buffer-to-output (istream &rest ostreams &aux (cnt 0))
  (do-input-stream-buffer (ch istream cnt)
    (incf cnt)
    (dolist (ost ostreams (values))
      (write-char ch ost))
    ))

;; input-stream のバッファにたまっているものを文字列に変換
(defun input-buffer-to-string (istream)
  (with-output-to-string (os)
    (input-buffer-to-output istream os)))

;;; path-list および現在のディレクトリ(current-directory=tの時)
;;; からファイルを検索
(defun path-search (filespec path-list 
                    &optional 
                    (current-directory t)
                    (error-when-unfound nil))
  (setq path-list (mapcar #'pathname (mklist path-list)))
  (when current-directory
    (push (make-pathname :directory '(:relative)) path-list))
  (dolist (path path-list
            (when error-when-unfound
              (error "~S was not found in ~S" filespec path-list)))
    (let ((candidate
           (merge-pathnames path filespec)))
      (when (probe-file candidate)
        (return candidate)))))

;;; （ディレクトリを換えずに）ファイル名を変換
(defun change-filename (pathname newfilename)
  (unless (pathnamep pathname)
    (setq pathname (pathname pathname)))
  (make-pathname
   :name (pathname-name newfilename)
   :type (pathname-type newfilename)
   :directory (pathname-directory pathname)))

;;; ファイル名の拡張子変換, なければ追加
(defun change-extension (pathname newext)
  (unless (pathnamep pathname)
    (setq pathname (pathname pathname)))
  (unless (or (null newext) (stringp newext))
    (setq newext (string-downcase (string newext))))
  #-clisp (make-pathname
           :name (pathname-name pathname)
           :type newext
           :directory (pathname-directory pathname))
  #+clisp (let* ((pname-name (pathname-name pathname))
                 (dot-pos (position #\. pname-name))
                 (name (if (and dot-pos (> dot-pos 0))
                           (subseq pname-name 0 dot-pos)
                         pname-name)))
            (make-pathname
             :name (string+ name #\. newext)
             :type nil
             :directory (pathname-directory pathname))
            )
  )

;;; ファイル名の拡張子獲得
(defun get-extension (pathname)
  (let* ((pathstring (namestring pathname))
         (i (when pathstring (position #\. pathstring :test #'char=))))
    (if i
        (subseq pathstring (1+ i))
      "" )))

;;; ファイルの新旧比較
(defun file-newer (path1 path2)
  (let ((exists1-p (probe-file path1))
        (exists2-p (probe-file path2)))
    (cond
     ((and exists1-p (not exists2-p))
      t)
     ((and (not exists1-p) exists2-p)
      nil)
     (t
      (> (file-write-date path1)
         (file-write-date path2))))))

;;; CLTLにないnamestring系
(defun name-namestring (pathname)
  (namestring (make-pathname :name (pathname-name pathname))))

;;;;;; 文字列操作

;;; 大文字／小文字の逆転
(defun char-invertcase (char)
  (cond
   ((char<= #\a char #\z)
    (char-upcase char))
   ((char<= #\A char #\Z)
    (char-downcase char))
   (t
    char)))

(defun case-mixed-p (seq)
  (and (some #'upper-case-p seq)
       (some #'lower-case-p seq)))

(defun string-invertcase (str)
  (declare (simple-string str))
  (map 'string #'char-invertcase str))

;;; str2 が str1 で始まる文字列なら、その残りの文字列を返す
(defun string-begin-with (str1 str2)
  (declare (simple-string str1 str2))
  (let ((pos (search str1 str2)))
    (and (eql 0 pos)
         (string-left-ntrim str2 (length str1)))))

;;; character or string or symbol のリストを文字列として結合
(defun string+ (&rest strings)
  (declare (list strings))
  (apply #'concatenate 'string
         (mapcar #'string strings)))

;;; tree内にある全ての文字列を結合
(defun string+-rec (&rest trees)
  (declare (list trees))
  (with-output-to-string (s)
    (do-all-atoms #'(lambda (x) (when x (princ x s)))
      trees)))

;;; リスト内の文字列を結合
(defun strcat (string-list &optional (inter "") (prev "") (post ""))
  (declare (list string-list) (simple-string inter prev post))
  (apply #'string+
         (separate-list string-list inter prev post)))

;;; 文字列参照
(defun string-ref (str n)
  (declare (simple-string str) (fixnum n))
  (if (>= n (length str))
      nil
    (aref str n)))

;;; 文字列追加
(defmacro add-string (str &rest slist)
  (let ((newstr (nconc `(concatenate 'string ,str) slist)))
    `(setq ,str ,newstr)))

;;; 最初のn文字
(defun string-firstn (str n &optional (ellipsis "..."))
  (declare (simple-string str ellipsis) (fixnum n))
  (with-output-to-string (s-out)
    (with-input-from-string (s-in str)
      (loop for i from 1 to n
          do (aif (read-char s-in nil nil)
                 (write-char it s-out)
               (loop-finish))
          finally 
            (when (and ellipsis (read-char s-in nil nil))
              (write-string ellipsis s-out))))))

;;; n文字削除
(defun string-left-ntrim (str &optional (n 1))
  (declare (simple-string str) (fixnum n))
  (values (subseq str n)
          (subseq str 0 n)))

;;; 左から条件に合う文字を削除
(defun string-left-trim-if (str func)
  (declare (simple-string str) (function func))
  (if (or (string= "" str)
          (not (funcall func (aref str 0))))
      (values str "")
    (let ((str-list (concatenate 'list str)))
      (do ((prev str-list (cdr prev))
           (curr (cdr str-list) (cdr curr)))
          ((or (endp curr)
               (not (funcall func (car curr))))
           (rplacd prev nil)
           (values (concatenate 'string curr)
                   (concatenate 'string str-list)))))))

;;; 右から条件に合う文字を削除
(defun string-right-trim-if (str func)
  (declare (simple-string str) (function func))
  (let ((str2 (copy-seq str)))
    (nreverse (string-left-trim-if (nreverse str2) func))))

;;; 空白削除
(defun string-left-trim-space (str)
  (declare (simple-string str))
  (string-left-trim-if
   str
   #'(lambda (c) 
       (declare (character c))
       (member c '(#\Tab #\Newline #\Page #\Return #\Space)))))
(defun string-right-trim-space (str)
  (declare (simple-string str))
  (let ((str2 (copy-seq str)))
    (nreverse (string-left-trim-space (nreverse str2)))))

;;; 空白でないものを削除
(defun string-left-trim-notspace (str)
  (declare (simple-string str))
  (string-left-trim-if
   str
   #'(lambda (ch) 
       (declare (character ch))
       (not
        (member ch '(#\Tab #\Newline #\Page #\Return #\Space))))))
(defun string-right-trim-notspace (str)
  (declare (simple-string str))
  (let ((str2 (copy-seq str)))
    (nreverse (string-left-trim-notspace (nreverse str2)))))

;; chの部分をstrに置き換える
(defun substitute-string (newstr oldch str)
  (declare (simple-string newstr str) (character oldch))
  (with-output-to-string (ost)
    (map nil #'(lambda (ch)
                 (declare (character ch))
                 (if (char= ch oldch)
                     (write-string newstr ost)
                   (write-char ch ost)))
         str)))

;; 文字列strをcharsに含まれる文字で分割してリストにする
(defun split-string (str &optional (chars '(#\Tab #\Newline #\Page #\Return #\Space))
                     &aux (ret (list)))
  (declare (simple-string str))
  (setq chars (mklist chars))
  (with-input-from-string (s-in str)
    (loop
      (do ((ch (read-char s-in nil nil)
               (read-char s-in nil nil)))
          ((or (and (null ch)
                    (return-from split-string (nreverse ret)))
               (not (member ch chars)))
           (unread-char ch s-in)))
      (push (with-output-to-string (s-out)
              (do ((ch (read-char s-in nil nil)
                       (read-char s-in nil nil)))
                  ((or (null ch)
                       (member ch chars)))
                (write-char ch s-out)))
            ret))))

;; 文字列strを最初のcharsに含まれる文字で分割して2要素のリストで返す
(defun split-string-1 (str &optional (chars '(#\Tab #\Newline #\Page #\Return #\Space)))
  (declare (simple-string str))
  (setq chars (mklist chars))
  (with-input-from-string (s-in str)
    (let* ((ret1
            (with-output-to-string (s-out)
              (do ((ch (read-char s-in nil nil)
                       (read-char s-in nil nil)))
                  ((or (null ch)
                       (member ch chars)))
                (write-char ch s-out))))
           (ret2
            (with-output-to-string (s-out)
              (do ((ch (read-char s-in nil nil)
                       (read-char s-in nil nil)))
                  ((or (null ch)))
                (write-char ch s-out)))))
      (list ret1 ret2))))

;; caseの文字列版
(defmacro string-case (exp &body case-clauses)
  (with-fresh-variables (exp-var)
    `(let ((,exp-var ,exp))
       (declare (simple-string ,exp-var))
       (cond
        ,@(loop for clause in case-clauses 
              collect
                (cond 
                 ((eq 'cl:otherwise (car clause))
                  `(t ,@(cdr clause)))
                 (t
                  `((or ,@(mapcar
                           #'(lambda (str)
                               (assert (stringp str))
                               `(string= ,exp-var ,str))
                           (mklist (car clause))))
                    ,@(cdr clause)))))))))

;;;;;; multiple-value 関連
(defmacro nth-multiple-value (n form)
  `(nth ,n (multiple-value-list ,form)))

;;;;;; シンボル操作

;;; 値の交換
(defmacro swap (place1 place2)
  (let ((temp (gensym)))
    `(let ((,temp ,place1))
       (setf ,place1 ,place2
             ,place2 ,temp))))

;;; x 中の pkg2 に登録されているシンボル( inherited も含む )
;;; を pkg1 に登録し直したものを返す
(defun immigrate-package (x pkg1 &optional pkg2)
  (declare (package pkg1))
  (map-all-atoms 
      #'(lambda (xx)
          (if (and (symbolp xx)
                   (or (null pkg2)
                       (eq xx (find-symbol (symbol-name xx) pkg2))))
              (intern (symbol-name xx) pkg1)
            xx))
    x))

;;; concatenate symbols
(defun cat-symbol (sym1 sym2)
  (declare (symbol sym1 sym2))
  (unless (and (symbolp sym1) (symbolp sym2))
    (error "~s or ~s is not symbol" sym1 sym2))
  (make-symbol
   (concatenate 'string (symbol-name sym1) (symbol-name sym2))))

(defun symbol+ (sym1 &rest rest-syms)
  (declare (symbol sym1))
  (let ((package (symbol-package sym1))
        (symstr (apply #'string+ 
                       (symbol-name sym1)
                       (mapcar #'symbol-name rest-syms))))
    (if package
        (intern symstr package)
      (make-symbol symstr))))

;;; 登録されているpackgeに依らず，シンボル名が同じがどうかを判定
(defun symbol= (sym1 sym2)
  (declare (symbol sym1 sym2))
  (and
   (symbolp sym1)
   (symbolp sym2)
   (string= (symbol-name sym1) (symbol-name sym2))))

;;;;;; リスト操作

;;; list を `(,@prev ,el1 ,@inter ,el2 ,@inter ... ,eln ,@post)
(defun separate-list (elms separator
                      &optional (head nil head-p) (tail nil tail-p))
  (declare (list elms))
  (nconc (when head-p (list head))
         (when elms
           (cons (car elms)
                 (mapcan #'(lambda (x) (list separator x))
                         (cdr elms))))
         (when tail-p (list tail))))

;;; member-from-tail
(defun member-from-tail (item list &key (key #'identity) (test #'eql) test-not)
  (declare (list list) (function key test))
  (do ((cur (member item list :key key :test test)
            (member item (cdr cur) :key key :test test))
       (prev nil cur))
      ((endp cur)
       (if prev prev test-not))))

;;; リストの末尾に要素追加
(defmacro add-element (place &rest elist)
  `(setf ,place (nconc ,place ,`(list ,@elist)))) 

;;; 複数回のpushを一度に書く
(defmacro pushs (&rest args &aux (place (car (last args))))
  `(progn
     ,@(mapcar #'(lambda (x)
                   `(push ,x ,place))
               (butlast args))))

;; listのn番目の直前に xs をappendしたものを返す（n=0でappendと同じ）
(defun insert (xs list &optional (n 0))
  (declare (list xs list) (fixnum n))
  (multiple-value-bind (prev post)
      (list-until list (nthcdr n list))
    (nconc prev (append xs post))))

;; listのn番目の直前に x をconsしたものを返す（n=0でconsと同じ）
(defun insert1 (x list &optional (n 0))
  (declare (list list) (fixnum n))
  (insert (list x) list n))

;;; リストの先頭n要素の複製を返す
(defun firstn (xs &optional (n 1))
  (declare (list xs) (fixnum n))
  (loop repeat n as x in xs collect x))

;;; リストのn番目をnew に置き換えたのものを返す
;;; n-new-list == (n1 new1 n2 new2 ...)
;; > (substitute-n '(1 2 3 4) 1 nil 2 10)
;; (1 nil 10 4)
(defun substitute-n (list &rest n-new-list)
  (declare (list list))
  (loop
      for x in list
      as i upfrom 0
      collect
        (progn (loop for n-new on n-new-list by #'cddr
                   thereis (and (= i (car n-new))
                                (progn (setq x (cadr n-new))
                                       t)))
               x)))

;;; position-rec
(defun position-rec (x list &key (test #'eql) (key #'identity))
  (declare (list list) (function test key))
  (labels ((rec (focus acc)
             (declare (list focus acc))
             (cond
              ((funcall test x (funcall key focus))
               acc)
              ((atom focus)
               nil)
              (t
               (or (rec (car focus) (cons 'car acc))
                   (rec (cdr focus) (cons 'cdr acc)))))))
    (rec list nil)))

;;; リストの要素を :key の要素が :test の意味で等しいもの同士でまとめる
;; > (assort '((1 2) (1 3) (3 4) (2 5) (9 3) (3 2)) :key #'car)
;; (((9 3)) ((2 5)) ((3 2) (3 4)) ((1 3) (1 2)))
(defun assort (lst &key (test #'eql) (key #'identity))
  (declare (list lst) (function test key))
  (let ((retval '()))
    (dolist (elm lst (mapcar #'cdr retval))
      (let ((keyval (funcall key elm)))
        (aif (member keyval retval :test test :key #'car)
            (push elm (cdar it))
          (push (cons keyval (list elm)) retval))))))

;; リストの順番を保存するassort
;; > (stable-assort '((1 2) (1 3) (3 4) (2 5) (9 3) (3 2)) :key #'car)
;; (((1 2) (1 3)) ((3 4) (3 2)) ((2 5)) ((9 3)))
(defun stable-assort (lst &key (test #'eql) (key #'identity))
  (declare (list lst) (function test key))
  (nreverse (mapcar #'nreverse (assort lst :test test :key key))))


;; リストの中から与えられた基準での最高要素を見付ける
(defun find-max (lst &key (test #'>) (key #'identity))
  (declare (list lst) (function test key))
  (let* ((ret (car lst))
         (ret-key (funcall key ret)))
    (mapc #'(lambda (x)
              (let ((x-key (funcall key x)))
                (when (funcall test x-key ret-key)
                  (setq ret x
                        ret-key x-key))))
          (cdr lst))
    (values ret ret-key)))

;; リストの中から最初に条件に合ったものを除いて，除いたものを返す
(defmacro find-pop (place test &key (key #'identity))
  (with-fresh-variables (test-var key-var prev-var x-var)
    `(let ((,test-var ,test)
           (,key-var ,key))
       (declare (function ,test ,key))
       (if (funcall ,test-var (funcall ,key-var (car ,place)))
           (pop ,place)
         (loop 
             for ,prev-var on ,place
             as ,x-var = (cadr ,prev-var)
             when (funcall ,test-var (funcall ,key-var ,x-var))
             do (rplacd ,prev-var (cddr ,prev-var))
                (return ,x-var))))))

;; 中間値
(defun median (lst &key (test #'>) (key #'identity))
  (declare (list lst) (function test key))
  (nth (truncate (/ (length lst) 2))
       (sort (copy-list lst) test :key key)))

;;; listでない引数をリスト化
(defun mklist (obj)
  (if (listp obj) obj (list obj)))

;;; リストの末尾にobjを追加
(defun append1 (lst obj)
  (declare (list lst))
  (append lst (list obj)))

;;; リストの長さと整数を比較
(defun list-length>= (list n)
  (declare (list list) (fixnum n))
  (if (endp list) (<= n 0)
    (loop for x in list
        as i upfrom 1
        thereis (>= i n))))

(defun list-length= (list n)
  (declare (list list) (fixnum n))
  (if (endp list) (= n 0)
    (loop for rest on list
        as i upfrom 1
        thereis (and (= i n)
                     (return (endp (cdr rest)))))))


;;;;; 集合演算

;;; 巾集合
(defun power-set (list)
  (declare (list list))
  (if (endp list)
      '(())
    (let ((remain-power-set (power-set (cdr list))))
      (append remain-power-set
              (mapcar (prefixed-func #'cons (car list))
                      remain-power-set)))))

;;; list の p と同じ要素の直前までの部分のコピーと
;;; その要素以後のリストを返す。
;;; test = #'eq , key = #'identity のときは ldiff とほぼ同じ
(defun list-until (list p &key (test #'eq) (key #'identity))
  (declare (list list) (function test key))
  (labels ((l-u-tail (list acc)
             (declare (list list acc))
             (if (or (endp list)
                     (funcall test (funcall key list) p))
                 (values (nreverse acc) list)
               (l-u-tail (cdr list) (cons (car list) acc)))))
    (l-u-tail list nil)))

;;; 先頭の共通部分数だけ，第一リストからコピーして返す
;;; 第二返り値で各givenリストの共通部分の残りをリストにして返す
(defun head-intersection (test list &rest lists)
  (declare (function test) (list list))
  (if (not lists) (values (copy-list list) (list nil))
    (let ((ret nil))
      (do* ((cur-list list (cdr cur-list))
            (cur-lists lists (mapcar #'cdr cur-lists))
            (cur-elm (car list) (car cur-list))
            (cur-elms (mapcar #'car cur-lists) (mapcar #'car cur-lists)))
          ((or (endp list)
               (member nil cur-lists)
               (notevery #'(lambda (x) (funcall test x cur-elm))
                         cur-elms))
           (values (nreverse ret) (cons cur-list cur-lists)))
        (push (car cur-elms) ret)))))

;;; list のcd..dr を順に調べ，testを満たす直前までのコピーと
;;; その要素以後のリストを返す．
(defun list-until-if (test list &key (key #'identity))
  (declare (function test key) (list list))
  (list-until list t 
              :test #'(lambda (x y)
                        (declare (ignore y))
                        (funcall test x))
              :key key))

;;; (a b) (1 2) (x y z) => (a 1 x) (a 1 y) (a 1 z) (a 2 x) ... (b 2 z)
(defun make-all-comb (&rest lists)
  (if (endp lists)
      (list ())
    (let ((fst (car lists))
          (rem-comb (apply #'make-all-comb (cdr lists))))
      (mapcan #'(lambda (fst-x)
                  (mapcar #'(lambda (comb) (cons fst-x comb)) rem-comb))
              fst))))

;;; make an integer list from 'from' to 'end' by step 'step'
(defun integer-list (from end &optional (step 1))
  (declare (integer from end step))
  (labels ((nl-tail (from acc)
             (declare (integer from) (list acc))
             (if (or (and (> step 0) (> from end))
                     (and (< step 0) (< from end)))
                 acc
               (nl-tail (+ from step) (cons from acc)))))
    (nreverse (nl-tail from nil))))

;;; recursive member
(defun member-rec (item list &rest args-member-rest)
  (declare (list list))
  (let (ret)
    (if (setq ret (apply #'member item list args-member-rest))
        ret
      (dolist (el list nil)
        (when (and (listp el)
                   (setq ret (apply #'member-rec item el args-member-rest)))
          (return-from member-rec ret))))))

;;; ((a b c)(x y z)(m n o)...)->((a x m)(b y n)(c z o) ...)
(defun combine-each-nth (lst &key (rev t) (n (length (car lst))))
  "Args: (list)
LIST must be a list of list.
Returns a list whose Nth element is a list whose Mth element is Nth of Mth of list."
  (labels ((c-e-n-tail (lst acc)
             (if (endp lst)
                 acc
               (c-e-n-tail
                (cdr lst)
                (let ((ithcdr-el
                       (firstn (car lst) n)))
                  (mapcar #'(lambda (x)
                              (prog1
                                  (cons (car ithcdr-el) x)
                                (setq ithcdr-el (cdr ithcdr-el))))
                          acc))))))
    (mapcar
     (if rev #'reverse #'identity)
     (c-e-n-tail lst (make-list n)))))

;;; make ((a . 1) (b . 2)) from (a b) (1 2 3)
(defun cmpd-list (a b &aux ab)
  "Args: (x y)
Returns a list whose Nth element is (cons (nth x) (nth y))"
  (declare (list a b ab))
  (unless (and (listp a) (listp b))
    (error "~s or ~s is not list" a b))
  (do ((aa a (cdr aa)) (bb b (cdr bb)))
      ((endp aa) (setq ab (nreverse ab)))
    (push `(,(car aa) . ,(car bb)) ab)))

;;; Queue
(defun make-queue ()
  (cons nil nil))
(defun empty-queue-p (queue)
  (declare (list queue))
  (null (car queue)))
(defun front-queue (queue)
  (declare (list queue))
  (if (empty-queue-p queue) nil
    (caar queue)))
(defun queue-list (queue)
  (declare (list queue))
  (car queue))
(defun insert-queue (item queue)
  (declare (list queue))
  (let ((new-pair (cons item '())))
    (cond ((empty-queue-p queue)
           (rplaca queue new-pair)
           (rplacd queue new-pair))
          (t
           (rplacd (cdr queue) new-pair)
           (rplacd queue new-pair)))
    queue))
(defun delete-queue (queue)
  (declare (list queue))
  (if (empty-queue-p queue) nil
    (let* ((front (car queue))
           (item (car front)))
      (rplaca queue (cdr front))
      item)))

(defun find-delete-queue (queue test &key (key #'identity))
  (declare (list queue) (function test key))
  (let ((qlist (queue-list queue)))
    (if (funcall test (funcall key (car qlist)))
        (progn
          (rplaca queue (cdr qlist))
          (car qlist))
      (loop 
          for prev on qlist
          as x = (cadr prev)
          when (funcall test (funcall key x))
          do (when (eq (cdr queue) (cdr prev))
               (rplacd queue prev))
             (rplacd prev (cddr prev))
             (return x)))))

;;;;;; ハッシュテーブル
(defun list-to-hashtable (list &optional (default-value t)
                          &rest make-hash-table-args)
  (declare (list list))
  (let ((ret-hashtable (apply #'make-hash-table make-hash-table-args)))
    (declare (hash-table ret-hashtable))
    (loop for elm in list
        do (setf (gethash elm ret-hashtable)
             default-value))
    ret-hashtable))

(defun alist-to-hashtable (alist &rest make-hash-table-args)
  (declare (list alist))
  (let ((ret-hashtable (apply #'make-hash-table make-hash-table-args)))
    (declare (hash-table ret-hashtable))
    (loop for (key . val) in alist
        do (setf (gethash key ret-hashtable) val))
    ret-hashtable))

;;;;;; 高階関数ユーティリティ

;;; 定数を返す関数
(defun const-f (&rest values)
  #'(lambda (&rest dummy)
      (declare (ignore dummy))
      (values-list values)))

;;; 途中でnilがでたら中断してnilを返すmapcar
;;; multiple-value 対応
(defun check-mapcar (func list)
  (declare (function func) (list list))
  (catch 'suspend
    (apply 
     #'values
     (combine-each-nth
      (mapcar 
       #'(lambda (x &aux (fx (multiple-value-list (funcall func x))))
           (if (car fx)
               fx 
             (throw 'suspend nil)))
       list)))))

;;; recursive mapcar
(defun rmapcar (fn &rest args)
  (declare (function fn))
  (if (some #'atom args)
      (apply fn args)
    (apply #'mapcar 
           #'(lambda (&rest args)
               (apply #'rmapcar fn args))
           args)))

;;; treeの全てのatom要素にfnを適用する
;;; 返り値はarg自身
(defun do-all-atoms (fn tree)
  (declare (function fn) (list tree))
  (if (atom tree)
      (funcall fn tree)
    (progn
      (map-all-atoms fn (car tree))
      (map-all-atoms fn (cdr tree)))))

;;; treeの全てのatom要素を，fnを適用した結果に置き換えたtreeを生成して返す
(defun map-all-atoms (fn tree)
  (declare (function fn) (list tree))
  (if (atom tree)
      (funcall fn tree)
    (cons (map-all-atoms fn (car tree))
	  (map-all-atoms fn (cdr tree)))))

;;; 非破壊的なmapcan
(defun mappend (fn &rest lists)
  (declare (function fn))
  (mapcan #'copy-list (apply #'mapcar fn lists)))

;;; quoteされている式か
(defun quoted-p (exp)
  (and (consp exp)
       (eq 'quote (car exp))))

(defun tagged-p (tag exp &key (test #'eq))
  (declare (function test))
  (and (consp exp)
       (funcall test tag (car exp))))

;;; (funall (tagger 'x 'y) 1 2) -> (x y 1 2) 
(defun tagger (&rest tags)
  #'(lambda (&rest remains)
      (append tags remains)))

;;; (funall (tail-tagger 'x 'y) 1 2) -> (1 2 x y) 
(defun tail-tagger (&rest tags)
  #'(lambda (&rest remains)
      (append remains tags)))

;;; compose functions
(defun compose-rev (&rest fns)
  (if fns
      (let* ((fn1 (car fns))
             (fns (cdr fns)))
        #'(lambda (&rest args)
            (let ((ret (apply fn1 args)))
              (loop for fn of-type function in fns
                  do (setq ret (funcall fn ret)))
              ret)))
    #'identity))

(defun compose (&rest fns)
  (apply #'compose-rev (nreverse fns)))

(defun compose-n (n fn)
  (declare (fixnum n) (function fn))
  (cond
   ((= n 0)
    #'identity)
   ((> n 0)
    #'(lambda (&rest args)
        (let ((ret (apply fn args)))
          (loop for i from 2 to n
              do (setq ret (funcall fn ret)))
          ret)))
   (t
    (error "compose-n: n must be positive or zero."))
   ))

(defun f-and (&rest fns)
  #'(lambda (&rest args)
      (apply #'every fns args)))

(defun f-or (&rest fns)
  #'(lambda (&rest args)
      (some #'every fns args)))

;; fun: 関数生成オペレータ (from "On Lisp")
(defmacro fun (expr) `#',(rbuild expr))
(defun rbuild (expr)
  (if (or (atom expr) (eq (car expr) 'lambda))
      expr
    (if (eq (car expr) 'compose)
        (build-compose (cdr expr))
      (build-call (car expr) (cdr expr)))))
(defun build-call (op fns)
  (let ((g (gensym)))
    `(lambda (,g)
       (,op ,@(mapcar #'(lambda (f)
                          `(,(rbuild f) ,g))
                      fns)))))
(defun build-compose (fns)
  (let ((g (gensym)))
    `(lambda (,g)
       ,(labels ((rec (fns)
                   (if fns
                       `(,(rbuild (car fns))
                         ,(rec (cdr fns)))
                     g)))
          (rec fns)))))

;;;;;; iteration macros
(defmacro vwhile (test &body body)
  (let ((retval (gensym)))
    `(do ((,retval nil (progn ,@body)))
         ((not ,test) ,retval))))

(defmacro till (test &body body)
  (let ((retval (gensym)))
    `(do ((,retval nil (progn ,@body)))
         (,test ,retval))))

(defmacro for ((var start stop) &body body)
  (let ((gstop (gensym)))
    `(do ((,var ,start (1+ ,var))
          (,gstop ,stop))
         ((> ,var ,gstop))
       ,@body)))

;;; memoized function 
(defun memoize (fn &key
                   (test #'eql)
                   (size nil size-p)
                   (rehash-size nil rehash-size-p)
                   (rehash-threshold nil rehash-threshold-p)
                   (use-multiple-values nil)
                   )
  (declare (function fn test) (integer size rehash-size rehash-threshold)
           (boolean use-multiple-values))
  (let ((cache (apply #'make-hash-table :test test
                      (nconc (when size-p (list :size size))
                             (when rehash-size-p (list :rehash-size rehash-size))
                             (when rehash-threshold-p
                               (list :rehash-threshold rehash-threshold))))))
    (declare (hash-table cache))
    (if use-multiple-values
        #'(lambda (arg)
            (multiple-value-bind (val win) (gethash arg cache)
              (if win
                  (values-list val)
                (let ((rets (multiple-value-list (funcall fn arg))))
                  (setf (gethash arg cache) rets)
                  (values-list rets)))))
      #'(lambda (arg)
          (multiple-value-bind (val win) (gethash arg cache)
            (if win
                val
              (let ((ret (funcall fn arg)))
                (setf (gethash arg cache) ret)
                ret)))) )
    ))

;;; define memoized method
(defmacro defmethod-memo (name specialized-lambda-list keyarg &rest rest-args)
  (assert (symbolp name))
  (assert (consp specialized-lambda-list))
  (assert (symbolp keyarg))
  (with-fresh-variables (cache do-var)
    `(let ((,cache (make-hash-table :test #'eq)))
       (declare (hash-table ,cache))
       (defmethod ,name ,specialized-lambda-list
                  (flet ((,do-var () ,@rest-args))
                    (if (symbolp ,keyarg)
                        (multiple-value-bind (val win) (gethash ,keyarg ,cache)
                          (if win
                              (values-list val)
                            (let ((rets (multiple-value-list (,do-var))))
                              (setf (gethash ,keyarg ,cache) rets)
                              (values-list rets))))
                      (,do-var)))))
    ))

;;; argument fixed function
(defun prefixed-func (func &rest prefix-args)
  (declare (function func))
  #'(lambda (&rest args)
      (apply func (append prefix-args args))))

(defun suffixed-func (func &rest suffix-args)
  (declare (function func))
  #'(lambda (&rest args)
      (apply func (append args suffix-args))))

(defun argfixed-func (func &optional prefixes suffixes)
  (declare (function func))
  (apply #'prefixed-func
         (apply #'suffixed-func func suffixes)
         prefixes))

;;; メッセージを表示してユーザーの入力を求める
(defun query (message)
  (format *error-output* "~A? " message)
  (read))

;;; リストの中の要素を1つ、ユーザからの入力によって選ぶ
(defun query-select-list (list &key
                               (print-elm #'write-to-string) 
                               (message "Select number"))
  (declare (list list) (function print-elm))
  (assert (consp list))
  (let ((i 0)
        (user-input nil))
    (dolist (elm list (values))
      (incf i)
      (format *error-output* "~2d:~a~%" i (funcall print-elm elm)))
    (till (and (numberp user-input)
               (<= 1 user-input i))
          (setq user-input (query message)))
    (nth (1- user-input) list)))

;;;;;;; エラー表示

;; CL-USER> (error-indent ("In function ~S:" 'f)
;; 	   (iwarn "123")
;; 	   (error-indent ("In function ~S: 'g") 
;; 	     (iwarn "cde"))
;; 	   (iwarn "abc")
;; 	   (iwarn "ABC"))
;; In function f:
;;   Warning: 123
;;   In function g:
;;     Warning: cde
;;   Warning: abc
;;   Warning: ABC

(defconstant *error-indent-step* 2)
(defvar *error-indent* 0)
(defvar *error-indent-spaces* "")
(defvar *error-annotations* nil)

(defvar *no-iwarn* nil)

(declaim (inline ierror iwarn iformat))

(defmacro error-indent (message &body body)
  `(let* ((*error-indent* (+ *error-indent-step* *error-indent*))
          (*error-annotations*
           (cons (string+ *error-indent-spaces* ,message) *error-annotations*))
          (*error-indent-spaces*
           (make-string *error-indent* :initial-element #\Space)))
     ,@body))

(defun print-annotations ()
  (loop
      for annot in (reverse *error-annotations*)
      when annot
      do
        (fresh-line *error-output*)
        (format *error-output* "~A~%" annot))
  ;; 一度表示したannotation は表示しない．
  (mapl #'(lambda (sl) (rplaca sl nil)) *error-annotations*)
  (setq *error-annotations* nil))       ; 次のloopの手間を省くため

(defun ierror (&rest args)
  (print-annotations)
  (apply #'error args))

(defun iwarn (&rest args)
  (unless *no-iwarn*
    (print-annotations)
    (fresh-line *error-output*)
    (format *error-output* "~AWarning: ~?~%"
      *error-indent-spaces* (car args) (cdr args))))

(defun iformat (fstr &rest args)
  (print-annotations)
  (fresh-line *error-output*)
  (format *error-output* "~A~?"
    *error-indent-spaces* fstr args))

;;;;;;; デバッグ用ツール

;;; recexpandに登録されているシンボルが先頭のフォームについては
;;; 再帰的にmacroexpandする．
;;; 実際に評価されるフォームかの判定（データとの区別）はしない
;;; once が非nilなら最低一度はexpandする
(defvar *recexpand-symbols* (list))
(defvar *abbrev-symbols* (list))
(defun macroexpand-rec (form &optional (once t))
  (macroexpand-rec2 (funcall (if once #'macroexpand-1 #'identity)
                             form)))
(defun macroexpand-rec2 (form)
  (cond
   ((atom form) form)
   ((member (car form) *recexpand-symbols*)
    (macroexpand-rec2 (macroexpand-1 form)))
   ((member (car form) *abbrev-symbols*)
    (list (car form) '*))
   (t
    (loop for rest on form
        as cur = (car rest)
        collect (macroexpand-rec2 cur)))))

;;; *recexpand-symbols*への登録，削除
(defun recexpand (&rest symbols)
  (setq *recexpand-symbols*
    (delete-duplicates (nconc *recexpand-symbols* symbols))))
(defun unrecexpand (&rest symbols)
  (loop for sym in symbols
      do (setq *recexpand-symbols* (delete sym *recexpand-symbols*)))
  *recexpand-symbols*)
(defun recexpand-clean ()
  (setq *recexpand-symbols* nil))

;;; *abbrev-symbols*への登録，削除
(defun recexpand-abbrev (&rest symbols)
  (setq *abbrev-symbols*
    (delete-duplicates (nconc *abbrev-symbols* symbols))))
(defun recexpand-unabbrev (&rest symbols)
  (loop for sym in symbols
      do (setq *abbrev-symbols* (delete sym *abbrev-symbols*)))
  *abbrev-symbols*)
(defun recexpand-abbrev-clean ()
  (setq *abbrev-symbols* nil))
