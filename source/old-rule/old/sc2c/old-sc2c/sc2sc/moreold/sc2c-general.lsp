;;; リスト内の文字列を結合
(defun string+ (x &optional (intv "") (prev "") (post "") &aux ret)
  (if (listp x)
      (if (null x)
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
  `(setq ,x (nconc ,x ,(append '(list) elist)))) 

;;; 文字列追加
(defmacro add-string (str &rest slist)
  (let ((newstr (nconc `(concatenate 'string ,str) slist)))
    `(setq ,str ,newstr)))

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

;;; Cの宣言文を生成
(defun make-declstr 
    (texp &optional idlist initlist bitstr &aux (ret "") initstr)
  (setq texp1 (first texp) texp2 (second texp) texp3 (third texp))
  (if (not (and (stringp texp1) (stringp texp2) (stringp texp3)))
      (error "texp is not type-expression"))
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

;;; リストの末尾n要素を抽出
(defun lastn (x &optional (n 1) &aux (ret x))
  (if (or (not (numberp n)) (not (listp x)))
      ret
      (dotimes (i (- (length x) n) ret)
	(setq ret (cdr ret)))))
