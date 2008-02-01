;; NF-TYPEを%defconstantで定義したうえでこれを%includeすると
;; ruleを自動設定，出力ファイル名変更

(%ifndef* NF-TYPE
  (%defconstant NF-TYPE GCC)) ; one of (GCC LW-SC CL-SC XCC XCCCL RAWC)

(%defconstant RULE-LWSC
              (:nestfunc-sc1 :nestfunc-rename :nestfunc-type :nestfunc-temp
                             (:nestfunc :all-in-estack ALL-IN-ESTACK)
                             :untype))

;; (%defconstant lw)
(%if* (eq 'NF-TYPE 'GCC)
  (%defconstant NESTFN fn)
  (%output-file INPUTFILE-NAME "-gcc")
%else
(%if* (eq 'NF-TYPE 'LW-SC)
  (%defconstant NESTFN lightweight)
  (%defconstant ALL-IN-ESTACK nil)
  (%rule RULE-LWSC)
  (%output-file INPUTFILE-NAME "-lw")
%else
(%if* (eq 'NF-TYPE 'CL-SC)
  (%defconstant NESTFN lightweight)
  (%defconstant ALL-IN-ESTACK t)
  (%rule RULE-LWSC)
  (%output-file INPUTFILE-NAME "-clos")
%else
(%if* (eq 'NF-TYPE 'XCC)
  (%defconstant NESTFN lightweight)
  (%sc2c (:sxcc2c :closure-keyword "lightweight"))
  (%output-file INPUTFILE-NAME "-xcc")
%else
(%if* (eq 'NF-TYPE 'XCCCL)
  (%defconstant NESTFN lightweight)
  (%sc2c (:sxcc2c :closure-keyword "lightweight"))
  (%output-file INPUTFILE-NAME "-xcccl")
%else
  (%error "Unknown NF-TYPE!")
  (%defconstant NESTFN fn)
  (%output-file INPUTFILE-NAME "-gcc")
  )))))

(%error "RULES: ~S" RULE)
