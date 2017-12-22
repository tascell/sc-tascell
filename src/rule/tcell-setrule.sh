;;; Copyright (c) 2008-2017 Tasuku Hiraishi <tasuku@media.kyoto-u.ac.jp>
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

(%ifndef* NF-TYPE
  (%defconstant NF-TYPE GCC))           ; one of (GCC LW-SC CL-SC XCC XCCCL RAWC)

(%ifndef* NO-EXCEPTION                  ; Invalidate exception support if non-zero
  (%defconstant NO-EXCEPTION 0))
(%if* NO-EXCEPTION
  (%defconstant _NO-EXCEPTION-P (%splice :no-exception t))
  %else
  (%defconstant _NO-EXCEPTION-P (%splice :no-exception nil))
  )
                

(%if* (eq 'NF-TYPE 'RAWC)
  (%defconstant RULE-TCELL
                (:tcell-sc1 :tcell-type (:tcell :no-nestfunc t _NO-EXCEPTION-P) :untype))
  (%output-file INPUTFILE-NAME "-c")
%else
  (%include "rule/nestfunc-setrule.sh")
  (%defconstant RULE-TCELL
                (:tcell-sc1 :tcell-type (:tcell :nestfunc-tag NESTFN _NO-EXCEPTION-P) :untype)))

(%rule RULE-TCELL RULE)
(%error "RULE: ~S" RULE)
