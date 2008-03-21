(%ifndef* NF-TYPE
  (%defconstant NF-TYPE GCC)) ; one of (GCC LW-SC CL-SC XCC XCCCL RAWC)

(%include "rule/nestfunc-setrule.sh")

(%defconstant RULE-HSC
              (:hsc-sc1 :hsc-type (:hsc :nestfunc-tag NESTFN) :untype))

(%rule RULE-HSC RULE)

(%error "RULE: ~S" RULE)
