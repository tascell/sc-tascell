(%ifndef* NF-TYPE
  (%defconstant NF-TYPE GCC))           ; one of (GCC LW-SC CL-SC XCC XCCCL RAWC)

(%if* (eq 'NF-TYPE 'RAWC)
  (%defconstant RULE-TCELL
                (:tcell-sc1 :tcell-type (:tcell :no-nestfunc t) :untype))
  (%output-file INPUTFILE-NAME "-c")
%else
  (%include "rule/nestfunc-setrule.sh")
  (%defconstant RULE-TCELL
                (:tcell-sc1 :tcell-type (:tcell :nestfunc-tag NESTFN) :untype)))

(%rule RULE-TCELL RULE)
(%error "RULE: ~S" RULE)
