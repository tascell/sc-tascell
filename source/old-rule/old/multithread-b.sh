(decl (struct _thstelm))

(DEFTYPE REASON ENUM RSN_CONT RSN-RETVAL)

(DEFTYPE CONT-f (PTR (fn (PTR VOID) (PTR (STRUCT _THSTELM)) REASON)))

(DEF (enum T-STAT)
  THR_NEW_SUSPENDED
  THR_NEW_RUNNABLE
  THR_SUSPENDED
  THR_RUNNABLE
  THR_SCHEDULED)

(DEF (struct _THSTELM) (DEF C CONT-f) (DEF STAT (enum T-STAT)))

(DEFTYPE THSTELM (STRUCT _THSTELM))

(DEFTYPE THST_PTR (PTR THSTELM))
(deftype cont thst-ptr)

(DEF THST (ARRAY THSTELM 65536))

(DEF THST_TOP THST_PTR THST)

(DEFTYPE SCHDEXIT (PTR (fn VOID)))

(DEF CUR_SCHD_EXIT SCHDEXIT 0)

(DEF CUR_SCHD_THST_TOP THST_PTR THST)

(DEF (SCHEDULING) (FN VOID) (DEF @L0 __LABEL__)
     (DEF PREV_EXIT SCHDEXIT CUR_SCHD_EXIT)
     (DEF PREV_THST_TOP THST_PTR CUR_SCHD_THST_TOP)
     (DEF MYTHST_TOP THST_PTR THST_TOP)
     (DEF (NONLOCALEXIT) (fn VOID) (GOTO @L0)) (LABEL @L0 NIL)
     (= CUR_SCHD_EXIT NONLOCALEXIT) (= CUR_SCHD_THST_TOP THST_TOP)
     (= CUR_SCHD_THST_TOP MYTHST_TOP)
     (DO-WHILE 1
         (BEGIN (DEF CP THST_PTR)
                (BEGIN (= CP PREV_THST_TOP)
                       (IF (< CP MYTHST_TOP)
                           (DO-WHILE (EXPS (INC CP) (< CP MYTHST_TOP))
                               (IF (!= (FREF (MREF CP) STAT)
                                    THR_SCHEDULED)
                                   (BREAK)))))
                (IF (== CP MYTHST_TOP) (IF PREV_EXIT (PREV_EXIT))))
         (BEGIN (DEF CP THST_PTR) (DEF CC CONT-f)
                (BEGIN (= CP (- THST_TOP 1))
                       (IF (>= CP THST)
                           (DO-WHILE (EXPS (DEC CP) (>= CP THST))
                               (IF (OR
                                    (== (FREF (MREF CP) STAT)
                                     THR_RUNNABLE)
                                    (== (FREF (MREF CP) STAT)
                                     THR_NEW_RUNNABLE))
                                   (BREAK)))))
                (IF (< CP THST) (BEGIN 
		    ;(FPRINTF STDERR "No active thread!\\n")
                    ;(EXIT 1)
		))
                (DO-WHILE (== (FREF (MREF CP) STAT) THR_RUNNABLE)
                    (= CC (FREF (MREF CP) C)) (= (FREF (MREF CP) C) 0)
                    (= (FREF (MREF CP) STAT) THR_SCHEDULED)
                    (CC CP RSN_CONT)))
         (IF (AND (> THST_TOP THST)
                 (== (FREF (MREF (- THST_TOP 1)) STAT)
                      THR_NEW_RUNNABLE))
             (BEGIN (DEC THST_TOP) (BREAK))))
     (= CUR_SCHD_EXIT PREV_EXIT) (= CUR_SCHD_THST_TOP PREV_THST_TOP))

(DEF (THR_RESUME CP) (FN VOID THST_PTR)
     (IF (== (FREF (MREF CP) STAT) THR_SUSPENDED)
         (= (FREF (MREF CP) STAT) THR_RUNNABLE)
         (IF (== (FREF (MREF CP) STAT) THR_NEW_SUSPENDED)
             (= (FREF (MREF CP) STAT) THR_NEW_RUNNABLE))))