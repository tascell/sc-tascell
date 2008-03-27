;;; Copyright (c) 2008 Tasuku Hiraishi <hiraisi@kuis.kyoto-u.ac.jp>
;;; All rights reserved.

;;; Redistribution and use in source and binary forms, with or without
;;; modification, are permitted provided that the following conditions
;;; are met:
;;; 1. Redistributions of source code must retain the above copyright
;;;    notice, this list of conditions and the following disclaimer.
;;; 2. Redistributions in binary form must reproduce the above copyright
;;;    notice, this list of conditions and the following disclaimer in the
;;;    documentation and/or other materials provided with the distribution.

;;; THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
;;; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;;; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;;; ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
;;; FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;;; DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
;;; OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
;;; HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
;;; LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
;;; OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
;;; SUCH DAMAGE.

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
  (%sc2c (:sxcc2c :closure-keyword "closure"))
  (%output-file INPUTFILE-NAME "-xcccl")
%else
  (%error "Unknown NF-TYPE!")
  (%defconstant NESTFN fn)
  (%output-file INPUTFILE-NAME "-gcc")
  )))))

(%error "RULES: ~S" RULE)
