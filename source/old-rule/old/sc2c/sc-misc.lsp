;;;; ユーティリティ関数・マクロ
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(provide 'sc-misc)
(unless (find-package "SC-MISC")
  (make-package "SC-MISC" :use '("LISP")))
(in-package "SC-MISC")

(export '(find-package2 immigrate-package swap
	  read-file write-file path-search change-extension string+ strcat
	  member-from-tail add-element string-ref add-string 
	  string-left-ntrim string-left-trim-space
	  string-left-trim-if cat-symbol symbol+ symbol=
	  dprint bprint abbrev-print btrace-switch btrace unbtrace
	  pushs firstn lastn butlastn mklist append1 list-until integer-list
	  make-all-comb
	  member-rec combine-each-nth cmpd-list check-mapcar rmapcar
	  do-all-atoms mappend 
	  while till for 
	  it aif awhen aunless awhile aand acond
	  memoize compose f-and f-or prefixed-func suffixed-func))

;;;;;

;;;;; ファイル操作


;;; <symbol>のパッケージが見つからなければ新たに作って返す
(defun find-package2 (pname)
  (or (find-package pname)
      (make-package pname)))

;;; ファイルからS式を読み込む
(defun read-file (file-name &key ((:package *package*) *package*) &aux ret)
  (unless (probe-file file-name)
    (error "Can't open ~S." file-name))
  (with-open-file (istream file-name :direction :input)
    (do ((y (read istream nil 'eof) (read istream nil 'eof)))
	((eq y 'eof) (setq x (nreverse ret)))
      (push y ret))))

;;; ファイルにS式または文字列を書き込む
(defun write-file (file-name list 
		   &key
		   (overwrite nil) (write-string nil)
		   ((:package *package*) *package*))
  (when (and (not overwrite)
	     (probe-file file-name))
    (error "~S already exists." file-name))
  (with-open-file (ostream file-name :direction :output)
    (if write-string
	(princ list ostream)
	(dolist (elm list t)
	  (write elm :stream ostream :escape t)
	  (terpri ostream)))))

;;; path-list からファイルを検索
(defun path-search (filespec path-list 
                    &optional 
                    (current-directory t)
                    (error-when-unfound nil))
  (setq path-list (mklist path-list))
  (let ((file-name (namestring filespec))
        (dir-list (if current-directory
		      (cons '(:current) path-list)
		      path-list)))
    (dolist (dir dir-list
             (when error-when-unfound
               (error "~s is unfound in ~s" file-name dir-list)))
      (let ((dir-file (make-pathname :name file-name
				     :directory dir)))
	(when (probe-file dir-file)
	  (return dir-file))))))  

;;; ファイル名の拡張子変換, なければ追加
(defun change-extension (pathname newext &aux headstr)
  (unless (pathnamep pathname)
    (setq pathname (pathname pathname)))
  (unless (stringp newext)
    (setq newext (string-downcase (string newext))))
  (let* ((directory (pathname-directory pathname))
	 (file-name (pathname-name pathname))
	 (file-name-list (concatenate 'list file-name))
	 (head-name-list (ldiff 
			  file-name-list
			  (member-from-tail
			   #\. file-name-list :test #'char=))))
    (make-pathname
     :name
     (format nil "~A.~A"
	     (concatenate 'string head-name-list)
	     newext)
     :directory
     directory)))

;;;;;; 文字列操作


;;; 文字列の結合
(defun string+ (&rest string-list)
  (apply #'format nil
	 (apply #'concatenate 'string 
		(make-list (length string-list)
			   :initial-element "~a"))
	 string-list))

;;; リスト内の文字列を結合
(defun strcat (x &optional (inter "") (prev "") (post "") &aux ret)
  (if (listp x)
      (if (null (setq x (remove nil x)))
	  (setq ret "")
	  (progn
	    (setq ret (string (car x)))
	    (setq inter (string inter))
	    (dolist (a (cdr x) ret)
	      (setq ret (concatenate 'string ret inter (string a))))))
      (setq ret (string x)))
  (setq prev (string prev))
  (setq post (string post))
  (concatenate 'string prev ret post))

;;; 文字列参照
(defun string-ref (str n)
  (check-type str string)
  (if (>= n (length str))
      nil
      (aref str n)))

;;; 文字列追加
(defmacro add-string (str &rest slist)
  (let ((newstr (nconc `(concatenate 'string ,str) slist)))
    `(setq ,str ,newstr)))

;;; n文字削除
(defun string-left-ntrim (str &optional (n 1))
  (if (= n 0)
      (values str "")
      (let ((str-list (concatenate 'list str))
            (post nil))
        (setq post (nthcdr n str-list))
        (rplacd (nthcdr (1- n) str-list) nil)
        (values (concatenate 'string post)
                (concatenate 'string str-list)))))
  
;;; 条件削除
(defun string-left-trim-if (str func)
  (if (not (funcall func (aref str 0)))
      (values str "")
      (let ((str-list (concatenate 'list str)))
	(do ((prev str-list (cdr prev))
	     (curr (cdr str-list) (cdr curr)))
	    ((or (endp curr)
		 (not (funcall func (car curr))))
	     (rplacd prev nil)
	     (values (concatenate 'string curr)
		     (concatenate 'string str-list)))))))

;;; 空白削除
(defun string-left-trim-space (string)
  (string-left-trim #(#\Tab #\Newline #\Page #\Return #\Space)
		    string))

;;;;;; シンボル操作

;;; 変数の交換
(defmacro swap (sym1 sym2)
  (let ((temp (gensym)))
    `(setq
      ,temp ,sym1
      ,sym1 ,sym2
      ,sym2 ,temp)))

;;; x 中の pkg2 に登録されているシンボル( inherited も含む )
;;; を pkg1 に登録し直したものを返す
(defun immigrate-package (x pkg1 &optional pkg2)
  (do-all-atoms 
      #'(lambda (xx)
	  (if (and (symbolp xx)
		   (or (null pkg2)
		       (eq xx (find-symbol (symbol-name xx) pkg2))))
	      (intern (symbol-name xx) pkg1)
	      xx))
    x))

;;; concatenate symbols
(defun cat-symbol (sym1 sym2)
  (unless (and (symbolp sym1) (symbolp sym2))
    (error "~s or ~s is not symbol" sym1 sym2))
  (make-symbol
    (concatenate 'string (symbol-name sym1) (symbol-name sym2))))

(defun symbol+ (sym1 &rest rest-syms)
  (let ((package (symbol-package sym1))
        (symstr (apply #'string+ 
                       (symbol-name sym1)
                       (mapcar #'symbol-name rest-syms))))
    (if package
        (intern symstr package)
        (make-symbol symstr))))

;;; 登録されているpackgeに依らず，シンボル名が同じがどうかを判定
(defun symbol= (sym1 sym2)
  (and
   (symbolp sym1)
   (symbolp sym2)
   (string= (symbol-name sym1) (symbol-name sym2))))

;;;;;; リスト操作

;;; member-from-tail
(defun member-from-tail (item list &key (key #'identity) (test #'eql) test-not)
  (do ((cur (member item list :key key :test test)
	    (member item (cdr cur) :key key :test test))
       (prev nil cur))
      ((endp cur)
       (if prev prev test-not))))

;;; リストの末尾に要素追加
(defmacro add-element (x &rest elist)
  `(setq ,x (nconc ,x ,`(list ,@elist)))) 


;;; 複数回のpushを一度に書く
(defmacro pushs (&rest args &aux (place (car (last args))))
  `(progn
     ,@(mapcar #'(lambda (x)
		   `(push ,x ,place))
	       (butlast args))))

;;; リストの先頭n要素を抽出
(defun firstn (x &optional (n 1) &key (func #'nreverse))
  (labels ((firstn-tail (x n acc)
	     (if (or (= n 0) (null x))
		 acc
		 (firstn-tail (cdr x) (1- n) (cons (car x) acc)))))
    (funcall func (firstn-tail x n nil))))

;;; リストの末尾n要素を抽出
(defun lastn (x &optional (n 1) &key (func #'identity))
  (funcall func (firstn (reverse x) n :func #'identity)))

;;; リストの末尾n要素を除く
(defun butlastn (x &optional (n 1))
  (firstn x (- (length x) n)))

;;; listでない引数をリスト化
(defun mklist (obj)
  (if (listp obj) obj (list obj)))

;;; リストの末尾にobjを追加
(defun append1 (lst obj)
  (append lst (list obj)))

;;; list の p と同じ要素の直前までの部分のコピー
;;; test = #'eq のときは ldiff と同じ
(defun list-until (list p &key (test #'eq))
  (labels ((l-u-tail (list acc)
	     (if (or (endp list)
		     (funcall test list p))
		 acc
		 (l-u-tail (cdr list) (cons (car list) acc)))))
    (reverse (l-u-tail list nil))))

;;; (a b) (1 2) (x y z) => (a 1 x) (a 1 y) (a 1 z) (a 2 x) ... (b 2 z)
(defun make-all-comb (&rest lists)
  (if (endp lists)
      '( () )
      (let ((fst (car lists))
	    (rem-comb (apply #'make-all-comb (cdr lists))))
	(mapcan #'(lambda (fst-x)
		    (mapcar #'(lambda (comb) (cons fst-x comb)) rem-comb))
		fst))))

;;; make an integer list from 'from' to 'end' by step 'step'
(defun integer-list (from end &optional (step 1))

  (labels ((nl-tail (from acc)
	     (if (or (and (> step 0) (> from end))
		     (and (< step 0) (< from end)))
		 acc
		 (nl-tail (+ from step) (cons from acc)))))
    (reverse (nl-tail from nil))))

;;; recursive member
(defun member-rec (item list &rest args-member-rest)
  (let (ret)
    (if (setq ret (apply #'member item list args-member-rest))
	ret
      (dolist (el list nil)
	(when (and (listp el)
		   (setq ret (apply #'member-rec item el args-member-rest)))
	  (return-from member-rec ret))))))


;((a b c)(x y z)(m n o)...)->((a x m)(b y n)(m n o) ...) 
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
  (unless (and (listp a) (listp b))
    (error "~s or ~s is not list" a b))
  (do ((aa a (cdr aa)) (bb b (cdr bb)))
      ((endp aa) (setq ab (nreverse ab)))
    (push `(,(car aa) . ,(car bb)) ab)))      

;;;;;; デバッグ用ユーティリティ

;;; debug-print
(defmacro dprint (&rest sym-list)
  `(print
    (list ,@(mapcan #'(lambda (x)
			(list `',x x))
		    sym-list))))

;;; print-and-break
(defmacro bprint (&rest print-arg)
  `(prog1
       (print ,@print-arg)
     (break)))

;;; 長さn以上のリストは省略してprint
(defun abbrev-print (object &optional (n 5) (stream *standard-output*))
  (print (abbrev-list object n) stream))

;;; ↑の省略形を得る
(defun abbrev-list (object &optional (n 5))
  (if (listp object)
      (progn
	(when (< n (length object))
	  (setq object (append (firstn object n) '(<...>))))
	(mapcar #'abbrev-list object))
      object))

;;; 関数呼び出しの直前でbreakしてくれるtrace

;; (<symbol> <orig-func> <btraced-func>) list
(defvar *break-trace-list* (list))
(defvar *break-before-call* t)
(defvar *break-after-return* nil)

(defun btrace-switch (&optional (before t) (after nil))
  (setq *break-before-call* before)
  (setq *break-after-return* after)
  (list before after))
  
(defun btrace1 (fsymbol)
  (let ((orig-func (symbol-function fsymbol)))
    (unless (aand (find fsymbol *break-trace-list* :key #'first)
              (eq orig-func (third it)))
      (setf (symbol-function fsymbol)
            #'(lambda (&rest args)
                (format *error-output* "> ~s~%" (cons fsymbol args))
		(prog2
		    (when *break-before-call* (break))
		    (let ((retlist (multiple-value-list
				    (apply orig-func args))))
		      (format *error-output* "< ~s~%" 
			      (cons fsymbol retlist))
		      (values-list retlist))
		  (when *break-after-return* (break)))))
      (setq *break-trace-list* 
            (remove fsymbol *break-trace-list* :key #'first))
      (push (list fsymbol orig-func (symbol-function fsymbol))
            *break-trace-list*))))

(defun unbtrace1 (fsymbol)
  (let ((tr-hist (find fsymbol *break-trace-list* :key #'first)))
    (when (and tr-hist
               (eq (symbol-function fsymbol) (third tr-hist)))
      (setf (symbol-function fsymbol) (second tr-hist))))
  (setq *break-trace-list*
        (remove fsymbol *break-trace-list* :key #'first))
  fsymbol)

(defmacro btrace (&rest fsymbols)
  `(dolist (fsymbol ',fsymbols (mapcar #'first *break-trace-list*))
    (btrace1 fsymbol)))

(defmacro unbtrace (&rest fsymbols)
  `(dolist (fsymbol ',fsymbols (mapcar #'first *break-trace-list*))
    (unbtrace1 fsymbol)))

;;;;;; 高階関数ユーティリティ

;;; 途中でnilがでたら中断してnilを返すmapcar
;;; multiple-value 対応
(defun check-mapcar (func list)
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
  (if (some #'atom args)
      (apply fn args)
      (apply #'mapcar 
	     #'(lambda (&rest args)
		 (apply #'rmapcar fn args))
	     args)))

;;; rmapcarに似ているが、ドットペア, ドットリストにも対応
(defun do-all-atoms (fn arg &aux ret dotlast)
  (if (atom arg)
      (funcall fn arg)
      (progn
	(do ((x arg (cdr x)))
	    ((null x))
	  (if (atom x)
	      (return (setq dotlast (funcall fn x)))
	      (push (do-all-atoms fn (car x)) ret)))
	(setq ret (reverse ret))
	(rplacd (last ret) dotlast)
	ret)))

;;; 非破壊的なmapcan
(defun mappend (fn &rest lists)
  (mapcan #'copy-list (apply #'mapcar fn lists)))

;;; compose functions
(defun compose (&rest fns)
  (if fns
      (let ((fn1 (car (last fns)))
	    (fns (butlast fns)))
	#'(lambda (&rest args)
	    (reduce #'funcall fns
		    :from-end t
		    :initial-value (apply fn1 args))))
    #'identity))

(defun f-and (&rest fns)
  (if fns 
      #'(lambda (&rest args)
	  (and (apply (car fns) args)
	       (apply (apply #'f-and (cdr fns)) args)))
      #'(lambda (&rest args) t)))

(defun f-or (&rest fns)
  (if fns
      #'(lambda (&rest args)
	  (or (apply (car fns) args)
	      (apply (apply #'f-or (cdr fns)) args)))
      #'(lambda (&rest args) nil)))

;;;;;; iteration macros
(defmacro while (test &body body)
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

;;;; Anaphoric Variants
(defmacro aif (test-form then-form &optional else-form)
  `(let ((it ,test-form))
    (if it ,then-form ,else-form)))

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
	      (let ((it ,sym)) ,@(cdr cl1))
	      (acond ,@(cdr clauses)))))))

;;; memoized function 
(defun memoize (fn)
  (let ((cache (make-hash-table :test #'equal)))
    #'(lambda (&rest args)
	(multiple-value-bind (val win) (gethash args cache)
	  (if win
	      (values-list val)
              (values-list
               (setf (gethash args cache)
                     (multiple-value-list (apply fn args)))))))))

;;; argument fixed function
(defun prefixed-func (func &rest prefix-args)
  #'(lambda (&rest args)
      (apply func (append prefix-args args))))

(defun suffixed-func (func &rest suffix-args)
  #'(lambda (&rest args)
      (apply func (append args suffix-args))))

