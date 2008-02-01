(defvar *keyword-list*
  '(fn inline va-arg :attr deftype enum struct union :bit array ptr
    def decl extern extern-def extern-decl static static-def
    auto auto-def register register-def
    defs extern-defs static-defs auto-defs register-defs
    void char signed-char unsigned-char short signed-short unsigned-short
    int signed-int unsigned-int long signed-long unsigned-long
    long-long signed-long-long unsigned-long-long float double long-double
    const restrict volatile
    begin let label case default if switch while do-while for loop
    goto continue break return
    aref fref inc dec sizeof cast if-exp exps
    bit-xor bit-and bit-or and or bit-and= bit-xor= bit-or=
    ptr mref bit-not not))

;;; 返値:identifier文字列
(defun identifier (x &aux sym)
  (if (and (symbolp x) (not (keywd x)) (not (null x)))
      (progn 
	(setq sym (symbol-name x))
	(if (let ((s1 (aref sym 0)))
	      (or (alpha-char-p s1) (eq s1 #\_)))
	    (dotimes (n (1- (length sym)) (string-downcase sym))
	      (unless (let ((sn (aref sym (1+ n))))
			(or (alphanumericp sn) (eq sn #\_)))
		(return nil)))
	    nil))
      nil))

;;; 返値:identifierの返値のリスト
(defun identifier-list (x &aux (ret1 nil) ret2)
  (if (listp x)
      (dolist (a x ret1)
	(if (setq ret2 (identifier a))
	    (setq ret1 (nconc ret1 (list ret2)))
	    (return nil)))))

;;; 返値: t or nil
(defun keywd (x)
  (member x *keyword-list*))

;;; 暫定
(defun constant (x)
  (cond
    ( (integerp x) (write-to-string x) )
    ( (floatp x) (write-to-string x) )
    ( (characterp x) (concatenate 'string "'" (char-sc2c x) "'") )
    ( t (identifier x) ))) ;;;==enumeration-constant

;;; 暫定<--文字列走査の必要
(defun string-literal (x)
  (if (stringp x)
      (concatenate 'string "\"" (string-sc2c x) "\"")
      nil))

;;; Lisp文字->C文字
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
    (othewise (string ch))))

;;; Lisp文字列->C文字列
(defun string-sc2c (str &aux (retstr ""))
  (if (not (stringp str))
      (error "str is not string"))
  (dotimes (i (length str) retstr)
    (add-string retstr (string (aref str i)))))
  