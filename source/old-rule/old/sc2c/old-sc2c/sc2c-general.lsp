(provide 'sc2c-general)

;;; リスト内の文字列を結合
(defun string+ (x &optional (intv "") (prev "") (post "") &aux ret)
  (if (listp x)
      (if (null (setq x (remove nil x)))
	  (setq ret "")
	  (progn
	    (setq ret (string (car x)))
	    (setq intv (string intv))
	    (dolist (a (cdr x) ret)
	      (setq ret (concatenate 'string ret intv (string a))))))
      (setq ret (string x)))
  (setq prev (string prev))
  (setq post (string post))
  (concatenate 'string prev ret post))

;;; リストの末尾に要素追加
(defmacro add-element (x &rest elist)
  `(setq ,x (nconc ,x ,`(list ,@elist)))) 

;;; 文字列追加
(defmacro add-string (str &rest slist)
  (let ((newstr (nconc `(concatenate 'string ,str) slist)))
    `(setq ,str ,newstr)))

;;; かっこをつける
(defun add-paren (str)
  (format nil "(~A)" str))

;;; ファイル名の拡張子変換
(defun change-extension (file-name newext &aux headstr)
  (if (not (stringp file-name))
      (error "file-name is not string"))
  (if (not (stringp newext))
      (error "newext is not string"))
  (setq headstr
	(do ((i (- (length file-name) 1) (- i 1)))
	    ((< i 0) file-name)
	  (if (eq #\. (aref file-name i))
	      (return (subseq file-name 0 i)))))
  (add-string headstr "." newext))

;;; concatenate symbols
(defun cat-symbol (sym1 sym2)
  (unless (and (symbolp sym1) (symbolp sym2))
    (error "~s or ~s is not symbol" sym1 sym2))
  (make-symbol
    (concatenate 'string (symbol-name sym1) (symbol-name sym2))))

;;; キーワードかどうかを判定
(defun keywd (x)
  (member x *keyword-list*))

;;; Cの宣言文を生成
(defun make-declstr 
    (texp1 texp2 texp3 &optional idlist initlist bitstr &aux (ret "") initstr)
  (if (not (listp idlist)) (setq idlist (list idlist)))
  (if (not (listp initlist)) (setq initlist (list initlist))) 
  (if (stringp bitstr)
      (setq bitstr (concatenate 'string " :" bitstr))
      (setq bitstr ""))
  (add-string ret texp1 " ") 
  (if (null idlist)  
      (add-string ret texp2 texp3)
      (progn
	(setq initstr
	      (if (stringp (first initlist)) 
		  (concatenate 'string "=" (first initlist))
		  ""))
	(add-string ret texp2 (first idlist) texp3 initstr bitstr)
	(do  ((id (cdr idlist) (cdr id)) (in (cdr initlist) (cdr in)))
	     ((endp id) ret)
	  (setq initstr
		(if (stringp (car in))
		    (concatenate 'string "=" (car in))
		    ""))
	  (add-string ret ", " texp2 (car id) texp3 initstr bitstr)))))

;;; SC文字->C文字
;;; 返値はstring
(defun char-sc2c (ch)
  (if (not (characterp ch)) 
      (error "ch is not character."))
  (case ch
    (#\Space " ")
    (#\Tab "\\t")
    (#\BackSpace "\\b")
    (#\Rubout "\\f")
    (#\Return "\\r")
    (#\Linefeed "\\n")
    (#\Page "\\f")
    (#\NewLine "\\n")
    (#\\ "\\\\")
    (#\' "\\'")
    (#\" "\\\"")
    (otherwise (string ch))))

;;; SC文字列->C文字列
(defun string-sc2c (str &aux (retstr ""))
  (unless (stringp str)
    (error "str is not string"))
  (let ((f-str (format nil str)))
    (dotimes (i (length f-str) retstr)
      (add-string retstr (char-sc2c (aref f-str i))))))

;;; SC-0演算子->C演算子
(defun operator-sc2c (op)
  (case op
    ((inc) "++")
    ((dec) "--")
    ((++ --) (string op))
    ((ptr) "&")
    ((mref) "*")
    ((bit-not) "~")
    ((not) "!")
    ((* / % + - << >>) (string op))
    ((bit-xor) "^")
    ((bit-and) "&")
    ((bit-or) "|")
    ((and) "&&")
    ((or) "||")
    ((< > <= >= == !=) (string op))
    ((= *= /= %= += -= <<= >>=) (string op))
    ((bit-and=) "&=")
    ((bit-xor=) "^=")
    ((bit-or=) "|=")
    ((exps) ",")
    (otherwise (error "unexpected operator ~S" op))))

