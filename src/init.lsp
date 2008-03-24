;;; Printer settings
(setq *print-case* :downcase)
(setq *print-circle* nil)
(setq *print-right-margin* 120)
;; (setq *compile-print* t)
;; (setq *load-print* t)
#+allegro (progn (setq top-level:*print-level* nil)
                 (setq top-level:*print-length* nil))

;;; Avoid name conflict with rule:declaration
(shadow 'cl:declaration)

;;; Optimization settings: referred ACL reference manual
;; The most debuggable (and yet reasonably fast) code, use
(proclaim '(optimize (speed 2) (safety 1) (space 1) (debug 3)))
;; The fastest and least safe code
;; (proclaim '(optimize (speed 3) (safety 0) (space 1) (debug 3)))

;;; Implementation-dependent settings
#+cmu (setq ext:*gc-verbose* nil)
#+clisp (setq custom:*default-file-encoding* charset:euc-jp)
#+allegro (setq comp:*cltl1-compile-file-toplevel-compatibility-p* t)
#+(and allegro mswindows) (setq *locale* (find-locale "japan.EUC"))
;; #+(and composer allegro) (wt:start-composer)

;;; Compile and load the SC system.
(with-compilation-unit ()
  (load "sc.lsp"))

;;; Typical sequences of transformation rule-sets
(defconstant *mt-sc* '(:multithread-sc1 :multithread-type :multithread-temp :multithread :untype))
(defconstant *lw-sc* '(:nestfunc-sc1 :nestfunc-type :nestfunc-temp
               :nestfunc :untype))
(defconstant *cl-sc* '(:nestfunc-sc1 :nestfunc-type :nestfunc-temp
               (:nestfunc :all-in-estack t) :untype))
(defconstant *lw-mt-sc* '(:multithread-sc1  :rename :hoist :multithread-type :multithread-temp
              :multithread :untype :nestfunc-sc1 :rename :hoist 
              :nestfunc-type :temp :nestfunc :untype))
(defconstant *stack-scan* '(:sc1 :type-info :stack-scan :untype))

;;; For translation of Tacell or LW-SC programs
;; with lightweight nested functions
(defun sc2c-lw (filename &rest args)
  (apply #'sc-main:sc2c filename :predefinitions ~((%defconstant NF-TYPE LW-SC)) args))
;; with XC-cube lightweight nested functions
(defun sc2c-xcc (filename &rest args)
  (apply #'sc-main:sc2c filename :predefinitions ~((%defconstant NF-TYPE XCC)) args))
;; with GCC nested functions
(defun sc2c-gcc (filename &rest args)
  (apply #'sc-main:sc2c filename :predefinitions ~((%defconstant NF-TYPE GCC)) args))
;; WITHOUT nested functions (only for comparative evaluation!)
(defun sc2c-c (filename &rest args)
  (apply #'sc-main:sc2c filename :predefinitions ~((%defconstant NF-TYPE RAWC)) args))

;;; Settings for REPL

;; packages
#+clisp (unuse-package :ext)
(use-package "SC-MAIN")
(use-package "SC-TRANSFORMER")
(use-package "RULE")   ; == sct-user
(use-package "SC-MISC")
(use-package "C2SC")

;; exit command
#+(or allegro cmu sbcl clisp)
(setf (symbol-function 'bye)
      #+allegro #'exit
      #+(or cmu sbcl) #'quit
      #+clisp #'ext::bye)


;;; print SC code without "sc::" prefix
(defun scprint (x)
  (with-package sc-file:*code-package*
    (print x)
    nil))

;;; Settings for macroexpand-rec (defined in "sc-misc.lsp")
(sc-misc:recexpand 'sct::matching-exp 'sct::matching-exp-for-list 'sct::matching-exp-for-commaat 'rule::if-match
                   'rule::cond-match)
(sc-misc:recexpand-abbrev 'macrolet)