(c-exp "#include<stdio.h>")
(c-exp "#include<stdlib.h>")

(decl (struct _thstelm))

; 継続用入れ子関数の呼び出し理由
(deftype reason enum rsn-cont rsn-retval)

; 継続用入れ子関数のポインタ
(deftype cont (ptr (fn (ptr void) (ptr (struct _thstelm)) reason)))

; スレッド管理用スタックの要素
(def (struct _thstelm)
     (def c cont)
     (def (enum _stat)
	  ; 停止中で，暗黙的継続も有効
	  thr-new-suspended
	  ; 実行可能で、暗黙的継続も有効
	  thr-new-runnable
	  ; 停止中で、明示的継続のみ有効
	  thr-suspended
	  ; 実行可能で、明示的継続のみ有効
	  thr-runnable
	  ; 明示的継続(cont c)は無効
	  thr-scheduled)
     (def stat (enum _stat)))

(deftype thstelm (struct _thstelm))

(deftype thst_ptr (ptr (struct _thstelm)))

; スレッド管理用スタック
(def thst (array thstelm 65536))

; スレッド管理用スタックのトップ
(def thst-top thst_ptr thst)

(deftype schdexit (ptr (fn void)))
; スケジューラの非局所脱出先
(def cur-schd-exit schdexit 0)
(def cur-schd-thst-top thst-ptr thst)

(def scheduling (fn void)
     (def @L0 --label--)
     ;元のスケジューラの情報
     (def prev-exit schdexit cur-schd-exit)
     (def prev-thst-top thst-ptr cur-schd-thst-top)
     ;このスケジュラの情報
     (def mythst-top thst-ptr thst-top)
     (def nonlocalexit (fn void) (goto @L0))

     (label @L0 nil)
     (= cur-schd-exit nonlocalexit)
     (= cur-schd-thst-top (= thst-top mythst-top))
     (while 1
       (let ((cp thst-ptr))
	 ;ここで元のスケジューラへの非局所脱出を試みる
	 (for ((= cp prev-thst-top)
	       (< cp mythst-top)
	       (inc cp))
	    (if (!= (fref cp -> stat) thr-scheduled) (break))
	    ; 間が全て thr-scheduled なら
	    (if (== cp mythst-top) (if prev-exit (prev-exit)))))

       ; runnableなスレッドを探す
       (let ((cp thst-ptr)
	     (cc cont))
	 (for ((= cp (- thst-top 1))
	       (>= cp thst)
	       (dec cp))
	      (if (or (== (fref cp -> stat) thr-runnable)
		      (== (fref cp -> stat) thr-new-runnable))
		  (break)))
	 (if (< cp thst)
	     (begin
	      ; 見つからなかったときは他のプロセッサからの要求を処理すべし
	      ; 今回は何もしない
	      (c-exp "fprintf(~a, ~a);" stderr "No Active thread!\\n")
	      (c-exp "exit(~a);" 1)))
	 (do-while (== (fref cp -> stat) thr-runnable)
	    (= cc (fref cp -> c))
	    (= (fref cp -> c) 0)
	    (= (fref cp -> stat) thr_scheduled)
		   (cc cp rsn-cont)))
       ;直下がnew-runnableならpopし、そちらに制御を移す
       (if (and (> thst-top thst)
		(== (fref (- thst-top 1) -> stat) thr-new-runnable))
	   (begin (dec thst-top) (break))))

     ; 元のスケジューラの情報を戻す
     (= cur-schd-exit prev-exit)
     (= cur-schd-thst-top prev-thst-top))

; thread-resume
(def (thr-resume cp) (fn void thst-ptr)
     (if (== (fref cp -> stat) thr-suspended)
	 (= (fref cp -> stat) thr-runnable)
       (if (== (fref cp -> stat) thr-new-suspended)
	   (= (fref cp -> stat) thr-new-runnable))))

(def (struct sync)
     (def n int)
     (def c thst-ptr))

(def (f-notify c-p s) (fn void cont (ptr (struct sync)))
     (inc (fref s -> n))
     (if (<= (fref s -> n) 0)
	 (thr-resume (fref s -> c))))

(def (f-wait c-p s) (fn void cont (ptr (struct sync)))
     (def ln int 0)
     (def (f-wait-c cp rsn) (fn (ptr void) thst-ptr reason)
	  (switch rsn
		  (case rsn-cont)
		  (switch ln (case 1) (goto @L1))
		  (return)
		  (case rsn-retval)
		  (switch ln) (return))
	  (return)
	  (begin
	   (dec (fref s -> n))
	   (if (< (fref s -> n) 0)
	       ; thread-suspend
	       (begin
		(def c0 thst-ptr cp)
		(= (fref c0 -> c) f-wait-c)
		(= (fref c0 -> stat) thr-suspended)
		(= (fref s -> c) c0)
		(= ln 1)
		(return))))
	  (label @L1 nil)
	  (= (fref cp -> c) c-p)
	  (= (fref cp -> stat) thr-runnable)
	  (return))

     (begin
      (dec (fref s -> n))
      (if (< (fref s -> n) 0)
	  (begin
	   ; thread-suspend
	   (def c0 thst-ptr (inc thst-top))
	   (= (fref c0 -> c) f-wait-c)
	   (= (fref c0 -> stat) thr-new-suspended)
	   (= (fref s -> c) c0)
	   (= ln 1)
	   (scheduling)))))

(def (g c-p x y) (fn int cont int int)
     (return (/ (+ x y) 2)))

(def (fw1 c-p n1 n2 tab1 tab2 s1 s2)
     (fn void cont int int (ptr int) (ptr int) (ptr (struct sync)) (ptr (struct sync)))
     (let ((ln int 0)
	   (i int n1) (i2 int)
	   (t1 int) (t4 int))
       (def (fw1-c cp rsn) (fn void thst-ptr reason)
	    (switch rsn
		    (case rsn-cont)
		    (switch ln
			    (case 1) (goto @L1)   (case 2) (goto @L2)
			    (case 3) (goto @L3)   (case 4) (goto @L4)
			    (case 5) (goto @L5)   (case 6) (goto @L6))
		    (return)
		    (case rsn-retval)
		    (switch ln
			    (case 1) (return (cast (ptr void) (ptr t1)))
			    (case 4) (return (cast (ptr void) (ptr t4))))
		    (return))
	    (return)
	    (while (< i n2)
	      (= ln 1)
	      (= t1 (g fw1-c (aref tab1 i) (aref tab1 (- i 1))))
	      (label @l1 nil)
	      (= (aref tab2 i) t1)
	      (inc i))
	    (= ln 2)
	    (f-notify fw1-c s1)
	    (label @L2 nil)
	    (= ln 3)
	    (f-wait fw1-c s2)
	    (label @L3 nil)
	    (= i2 n1)
	    (while (< i2 n2)
	      (= ln 4)
	      (= t4 (g fw1-c (aref tab2 (+ i 1)) (aref tab2 i)))
	      (label @L4 nil)
	      (= (aref tab1 i) t4)
	      (inc i2))
	    (= ln 5)
	    (f-notify fw1-c s1)
	    (label @L5 nil)
	    (= ln 6)
	    (f-wait fw1-c s2)
	    (label @L6 nil)
	    (= (fref cp -> c) c-p)
	    (= (fref cp -> stat) thr-runnable)
	    (return))

       (while (< i n2)
	 (= ln 1)
	 (= (aref tab2 i) (g fw1-c (aref tab1 i) (aref tab1 (- i 1))))
	 (inc i))
       (= ln 2)
       (f-notify fw1-c s1)
       (= ln 3)
       (f-wait fw1-c s2)
       (= i2 n1)
       (while (< i2 n2)
	 (= ln 4)
	 (= (aref tab1 i) (g fw1-c (aref tab2 (+ i 1)) (aref tab2 i)))
	 (inc i2))
       (= ln 5)
       (f-notify fw1-c s1)
       (= ln 6)
       (f-wait fw1-c s2)
       (return)))

(def (divide-work c-p n m) (fn void cont int int)
     (let ((ln int)
	   (tab1 (array int n))
	   (tab2 (array int n))
	   (s1 (struct sync) (struct 0 0))
	   (s2 (struct sync) (struct 0 0))
	   (i int 0))
       (def (divide-work-c cp rsn) (fn (ptr void) thst-ptr reason)
	    (switch rsn
		    (case rsn-cont)
		    (switch ln
			    (case 1) (goto @L1)   (case 2) (goto @L2))
		    (return)
		    (case rsn-retval)
		    (switch ln)
		    (return))
	    (return)
	    (while (< i m)
	      (begin
	       ; thread-create
	       (= ln 1)
	       (= (fref thst-top -> c) divide-work-c)
	       (= (fref thst-top -> stat) thr-new-runnable)
	       (inc thst-top)
	       (begin
		(def ln int 0)
		(def (nthr-c cp rsn) (fn (ptr void) thst-ptr reason)
		     (switch rsn
			     (case rsn-cont)
			     ; continue
			     (switch ln (case 1) (goto @L1))
			     (return)
			     (case rsn-retval)
			     ; retrun return value location
			     (switch ln)
			     (return))
		     (return)
		     (fw1 nthr-c 1 (- (/ n 2) 1) tab1 tab2 (ptr s1) (ptr s2))
		     (label @L1 nil)
		     (return))
		(= ln 1)
		(fw1 nthr-c 1 (- (/ n 2) 1) tab1 tab2 (ptr s1) (ptr s2)))
	       (if (!= (fref (- thst-top 1) -> stat) thr-new-runnable)
		   (scheduling)
		 (dec thst-top)))
	      (label @L1 nil)
	      (= ln 2)
	      (fw1 divide-work-c (/ n 2) (- n 2) tab1 tab2 (ptr s2) (ptr s1))
	      (label @L2 (inc i)))
	    (= (fref cp -> c) c-p)
	    (= (fref cp -> stat) thr-runnable)
	    (return))
       (while (< i m)
	 ; thread-create
	 (begin
	  (= ln 1)
	  (= (fref thst-top -> c)  divide-work-c)
	  (= (fref thst-top -> stat) thr-new-runnable)
	  (inc thst-top)
	  (begin
	   (def ln int 0)
	   (def (nthr-c cp rsn) (fn (ptr void) thst-ptr reason)
		(switch rsn
			(case rsn-cont)
			; continue
			(switch ln (case 1) (goto @L1))
			(return)
			(case rsn-retval)
			; return return value location
			(switch ln)
			(return))
		(return)
		(fw1 nthr-c 1 (- (/ n 2) 1) tab1 tab2 (ptr s1) (ptr s2))
		(label @L1 (return)))
	   (= ln 1)
	   (fw1 nthr-c 1 (- (/ n 2) 1) tab1 tab2 (ptr s1) (ptr s2)))
	  (if (!= (fref (- thst-top 1) -> stat) thr-new-runnable)
	      (scheduling)
	    (dec thst-top)))
	 (= ln 2)
	 (fw1 divide-work-c (/ n 2) (- n 2) tab1 tab2 (ptr s2) (ptr s1))
	 (inc i))))

						 
	   
