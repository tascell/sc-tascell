(let ((directory (pathname-directory *load-pathname*)))
  (loop for prog in '("fib" "lu" "pen" "pen6x17" "cmp")
      collect (loop for mode in ~(GCC LW-SC CL-SC XCC XCCCL RAWC)
                  as path = (make-pathname
                                 :directory directory
                                 :name prog
                                 :type "tcell")
                  as pred = ~((%defconstant NF-TYPE ,mode))
                  collect (progn
                            (format *error-output*
                                "~&~S~&" `(sc2c ,path :predefinitions ',pred))
                            (sc2c path :predefinitions pred)))))
