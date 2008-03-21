(%ifndef @nestfunc-tag
  ((%defconstant @nestfunc-tag fn))) 


(def (main argc argv) (fn int int (ptr (ptr char)))
     (def @MAIN-RET --label--)
     (def ln int 0)
     (def msc-main-ret int)
     (def (main-c cp rsn) (@nestfunc-tag (ptr void) thst-ptr reason)
	  (switch rsn
		  (case rsn-cont)
		  (switch ln
			  (case 0) (goto @L0)
			  (case 1) (goto @L1))
		  (return)
		  (case rsn-retval)
		  (return (cast (ptr void) (ptr msc-main-ret))))
	  (return)
	  (label @L0 (= ln 1))
	  (= msc-main-ret (msc-main main-c argc argv))
	  (label @L1
		 (goto @MAIN-RET)))
     (= (fref (mref thst-top) c) main-c)
     (= (fref (mref thst-top) stat) thr-runnable)
     (inc thst-top)
     (do-while 1 (scheduling))
     (label @MAIN-RET
	    (return msc-main-ret)))

