(setq *print-case* :downcase)
(setq *print-circle* nil)


(load "sc.lsp")

(defconstant *mt-sc* '("basic" "type" "tmp" "multithread" "untype"))
(defconstant *lw-sc* '("basic" "type" "tmp" "nestfunc" "untype"))
(defconstant *lw-mt-sc* '("basic"  "rename" "hoist" "type" "tmp"
			  "multithread" "untype" "basic" "rename" "hoist" 
			  "type" "tmp" "nestfunc" "untype"))

;;;
(load "c2sc.lsp")
