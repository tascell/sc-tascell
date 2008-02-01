;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; copygc.c

(%ifndef TOSIZE
  ((%defconstant TOSIZE (* 5 1024 1024))) )

(%ifndef ROOTSIZEMAX
  ((%defconstant ROOTSIZEMAX (* 8 1024))) )

(%ifndef GC_STACK_SIZE
  ((%defconstant GC_STACK_SIZE (/ (fref params tosize) (sizeof double)))) )

(%ifndef GC_LIMITED_STACK_SIZE
  ((%defconstant GC_LIMITED_STACK_SIZE 256)) )

(%ifndef GC_LIMITED_STACK_MAX
  ((%defconstant GC_LIMITED_STACK_MAX 32)) )

(%defmacro IN_FROM (p)
   `(< (cast unsigned-int (- (cast (ptr char) ,p) old-memory))
       (fref params tosize)) ) 
     
(%defmacro IN_TOSP (p)
   `(< (cast unsigned-int (- (cast (ptr char) ,p) new-memory))
       (fref params tosize)) ) 

(%defmacro FWPTR (p)
   `(mref (cast (ptr (ptr void)) ,p)))

(def (csym::MEMCPY d s sz) (fn (ptr void) (ptr void) (ptr (const void)) size_t)
  (def _des (ptr long) (cast (ptr long) d))
  (def _src (ptr long) (cast (ptr long) s))
  (def _til (ptr long) (cast (ptr long) (+ (cast (ptr char) _src) sz)) )
  (do-while (< _src _til)
    (= (mref (inc _des)) (mref (inc _src))) ))


;; util
(decl (csym::exit) (fn void int))

(static-def (csym::error s) (fn void (ptr char))
  (csym::printf "ERROR: %s~%" s)
  (csym::exit 1))

(static-def (csym::myalloc size) (fn (ptr void) size_t)
  (def p (ptr void))
  (= p (csym::malloc size))
  (if (== p NULL)
      (csym::error "Not enough memory."))
  (return p))

(static-def (csym::myrealloc p size) (fn (ptr void) (ptr void) size_t)
  (def q (ptr void))
  (= q (csym::realloc p size))
  (if (== q NULL)
      (csym::error "Not enough memory."))
  (return q))


;; /* root + heap area */

(def params gc_params)

(static-def allocated_size int)
(static-def old_memory (ptr char))
(static-def new_memory (ptr char))
(static-def old_memory_end (ptr char))
(static-def new_memory_end (ptr char))

;; /* gc */
(static-def b (ptr char))


;; /* *link = move(*link); */
(def (csym::move vp) (fn (ptr void) (ptr void))
  (def p (ptr char) vp)
  (def d desc_t)
  (def np (ptr char))
  (def nb (ptr char))

  (if (not (IN_FROM p))
      (return vp))
  (if (IN_TOSP (FWPTR p))
      (return (FWPTR p)))
  
  (= d (mref (cast (ptr desc_t) p)))
  (= np b)
  (= nb (+ np (fref (mref d) asize)))
  (if (>= nb new-memory-end)
      (csym::error "b overrun."))
  (csym::MEMCPY np p (fref (mref d) size))
  (= b nb)
  (= (FWPTR p) np)
  (return np))


;; /* BREADTH_FIRST_GC */
(def (gc_breadth_first scan) (fn void sht)
  (def i int)
  (def tmp (ptr char))
  (def d desc_t)
  (def p (ptr char))
  (def link (ptr (ptr void)))
  (def s (ptr char))

  (if (fref params gcv)
      (csym::printf "BREADTH_FIRST_GC start~%"))
  (= b new-memory) (= s b)
  (scan)
  (while (< s b)
    (= d (mref (cast (ptr desc_t) s)))
    (= p (cast (ptr char) s))
    (for ((= i 0) (< i (fref (mref d) fli_len)) (++ i))
      (= link (cast (ptr (ptr void)) (+ p (aref (fref (mref d) fli) i))))
      (= (mref link) (csym::move (mref link))))
    (+= s (fref (mref d) asize)))
  (= allocated_size (- b new_memory))
  (= tmp new_memory)
  (= new_memory old_memory)
  (= old_memory tmp)
  (= tmp new_memory_end)
  (= new_memory_end old_memory_end)
  (= old_memory_end tmp)
  (if (fref params gcv)
      (csym::printf "GC complete (%d)~%" allocated_size))
  )

;; /* */
(def gc-ttime double)

(def tp1 (struct timeval))
(def tp2 (struct timeval))

(def (gc scan) (fn void sht)
  (csym::gettimeofday (ptr tp1) 0)
  (switch (fref params gctype)
     (case 0)
     (gc-breadth-first scan)
     (break)
     )
  (csym::gettimeofday (ptr tp2) 0)
  (+= gc-ttime
      (+    (- (fref tp2 tv_sec) (fref tp1 tv_sec))
         (* (- (fref tp2 tv_usec) (fref tp1 tv_usec)) 0.000001)))
  )

;; /* memory allocation interface */

(def (csym::getmem-init p) (fn void gc-params)
  (def mp (ptr char))
  (static-def called int 0)

  (if called (return))
  
  (= called 1)
  (= gc-ttime 0.0)
  (= params p)
  (if (== (fref params tosize) 0) (= (fref params tosize) TOSIZE))
  (+= (fref params tosize) 3)
  (-= (fref params tosize) (bit-and (fref params tosize) 3))
  (if (== (fref params stack_size) 0) 
      (= (fref params stack_size) GC_STACK_SIZE))
  (if (== (fref params limited_stack_max) 0)
      (= (fref params limited_stack_max) GC_LIMITED_STACK_MAX))
  (csym::printf "tosize=%d, stack=%d, limit=%d~%"
                (fref params tosize)
                (fref params stack_size)
                (fref params limited_stack_max) )
  (= old_memory (csym::myalloc (fref params tosize)))
  (= old_memory_end (+ old_memory (fref params tosize)))
  (= new_memory (csym::myalloc (fref params tosize)))
  (= new_memory_end (+ new_memory (fref params tosize)))
  (= allocated_size 0)
  (for ((= mp new_memory) (< mp new_memory_end) (+= mp 4096)) (= (mref mp) 1) )
  (for ((= mp old_memory) (< mp old_memory_end) (+= mp 4096)) (= (mref mp) 1) )
  )

(def (csym::try_getmem d) (fn (ptr void) desc_t)
  (def size size_t)
  (def p (ptr char))
  (= size (fref (mref d) asize))
  (if (> (+ allocated_size size) (fref params tosize)) (return 0))
  (= p (+ old_memory allocated_size))
  (+= allocated_size size)
  (csym::memset p 0 size)
  (= (mref (cast (ptr desc_t) p)) d)
  (return p))

(def (getmem scan d) (fn (ptr void) sht desc_t)
  (def p (ptr void))
  (= p (csym::try_getmem d))
  (if (== p 0)
    (begin
      (gc scan)
      (= p (csym::try_getmem d))
      (if (== p 0)
          (csym::error "getmem: Not enough memory."))
      ))
  (return p))

