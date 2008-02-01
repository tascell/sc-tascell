(pushnew :sc-system *features*)

(eval-when (:compile-toplevel :load-toplevel :execute)
  (require "SC-DECL" "sc-decl.lsp"))


(scr:require "SC-MAIN")
(scr:require "C2SC")
;;(sct:require-rule :sc2c)

(defconstant *sc-source-files*
  '("c2sc-lex.lsp"
    "c2sc-parser.lsp"
    "c2sc.lsp"
    ;; "init.lsp"
    "sc-main.lsp"
    "sc-file.lsp"
    "sc-misc.lsp"
    ;; "sc-util.lsp"
    "scpp.lsp"
    "sct.lsp"
    "sct-lib.lsp"    
    "sct-file.lsp"
    "sct-macch.lsp"
    "sct-rule2lsp.lsp"
    "sct-defrule.lsp"
    "sct.lsp"
    ))

(defun compile-sc-sources (&key
                           (src *sc-source-files*)
                           (path scr:*sc-system-path*))
  (setq src (if (atom src) (list src) src))
  (mapcar #'compile-file
          (mapcar #'(lambda (x)
                      (merge-pathnames path x))
                  src)))
