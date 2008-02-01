(%ifndef* NF-TYPE
  (%defconstant NF-TYPE GCC)) ; one of (GCC LW-SC CL-SC XCC XCCCL RAWC)

(%defconstant RULE-FULL
	      (:tcell-sc1 :tcell-type (:tcell :nestfunc-tag lightweight) :untype
   	       :nestfunc-sc1 :nestfunc-rename :nestfunc-type :nestfunc-temp
	       (:nestfunc :all-in-estack ALL-IN-ESTACK) :untype))
(%defconstant RULE-GCC (:tcell-sc1 :tcell-type
          	        (:tcell :nestfunc-tag NFTAG :no-nestfunc NONESTFUNC)
			:untype))

(%if* (eq 'NF-TYPE 'GCC)
  (%defconstant NFTAG fn)
  (%defconstant NONESTFUNC nil)
  (%rule RULE-GCC)
  (%output-file INPUTFILE-NAME "-gcc")
%else
(%if* (eq 'NF-TYPE 'LW-SC)
  (%defconstant ALL-IN-ESTACK nil)
  (%rule RULE-FULL)
  (%output-file INPUTFILE-NAME "-lw")
%else
(%if* (eq 'NF-TYPE 'CL-SC)
  (%defconstant ALL-IN-ESTACK t)
  (%rule RULE-FULL)
  (%output-file INPUTFILE-NAME "-clos")
%else
(%if* (eq 'NF-TYPE 'XCC)
  (%defconstant NFTAG lightweight)
  (%defconstant NONESTFUNC nil)
  (%rule RULE-GCC)
  (%sc2c (:sxcc2c :closure-keyword "lightweight"))
  (%output-file INPUTFILE-NAME "-xcc")
%else
(%if* (eq 'NF-TYPE 'XCCCL)
  (%defconstant NFTAG lightweight)
  (%defconstant NONESTFUNC nil)
  (%rule RULE-GCC)
  (%sc2c (:sxcc2c :closure-keyword "closure"))
  (%output-file INPUTFILE-NAME "-xcccl")
%else
(%if* (eq 'NF-TYPE 'RAWC)		; only for comperative evaluation
  (%defconstant NFTAG fn)
  (%defconstant NONESTFUNC t)
  (%rule RULE-GCC)
  (%output-file INPUTFILE-NAME "-c")
%else
  (%error "Unknown NF-TYPE!")
  (%rule RULE-GCC)
))))))
