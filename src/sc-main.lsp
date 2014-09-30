;;; Copyright (c) 2008-2010 Tasuku Hiraishi <tasuku@media.kyoto-u.ac.jp>
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

;;; The SC language sytem main
(provide "SC-MAIN")
(eval-when (:compile-toplevel :load-toplevel :execute)
  (require "SC-DECL" "sc-decl.lsp"))

(scr:require "SC-MISC")
(scr:require "SCT")
(scr:require "SC-FILE")
(scr:require "SCPP")

(in-package "SC-MAIN")

;;;;;
;; (defconstant *scmain-package* *package*)

;;;;;
(defconstant *rule-list-default* '())   ;'(:sc1))
(defparameter *indent-command* "indent")
(defparameter *indent-options* '("-i2" "-nut"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun c-indent (c-file &optional (command *indent-command*)
                                  (option *indent-options*))
  (setq c-file (namestring c-file))
  (command-line command
                :args (cons c-file option)
                :verbose nil)
  c-file)

(defun cc (&rest args)
  (setq args
    (mapcar #'(lambda (arg)
                (if (pathnamep arg)
                    (namestring arg)
                  arg))
            args))
  (command-line "gcc" :args args :verbose nil))

;;; Predefined macros
(defun make-system-predefined-macros (&key input-file rule-list)
  (when input-file
    (list ~(%defconstant INPUTFILE ,(namestring input-file))
          ~(%defconstant INPUTFILE-NAME ,(name-namestring input-file))
          ~(%defconstant INPUTFILE-DIRECTORY ,(directory-namestring input-file))
          ~(%defconstant RULE ,rule-list)
          )))

;;; ��֥ե�������ϡܥ�å�����
(defun write-intermediate-file (pathname x)
  (format *error-output*
      "~&Writing an intermeidate file ~S...~%" (namestring pathname))
  (sc-file:write-sc-file pathname x))

;;; ���ϥե�����̾
(defun make-output-filename (output-file input-file
                             &optional (default-type "c"))
  (if (not input-file) (make-pathname :directory '(:relative)
                                      :name "sc2c-output"
                                      :type default-type)
    (let ((directory (aif (and output-file (pathname-directory output-file))
                         it
                       (pathname-directory input-file)))
          (name (if output-file
                    (pathname-name output-file)
                  (pathname-name input-file)))
          (type (aif (and output-file (pathname-type output-file))
                    it
                  default-type)))
      (make-pathname :name name :directory directory :type type))))

(defun sc2c (x
             &key
             ((:rule rule-list) *rule-list-default* rulelist-specified) ;rule-set�Υꥹ��
                                        ; ���ꤹ��� %rule ���ѹ��Բ�
             (output-file nil outputfile-specified) ; ����ʤ����ȼ�ư����
                                        ; ���ꤹ��� %output-file ���ѹ��Բ�
             (sc2c-rule :sc2c sc2c-specified) ; ɬ�פ˱������ѹ���nil����C���������ʤ�
                                        ; ���ꤹ��� %sc2c ���ѹ��Բ�
             (intermediate nil)         ; ��֥ե��������Ϥ��뤫��
             (predefinitions '())       ; �ץ�������Ƭ���ɲä��륳����
             (indent *indent-command*)  ; C�����ɤΥ���ǥ�Ȥ�·���볰�����ޥ��
             &aux
             (input-file nil)
             (input-file-path "./"))
  "Args: (x &key (rule nil)
                 (intermediate nil)
                 (output-file nil)
                 (indent *indent-command*) ...)
Compiles SC-program specified by X into C-program.
X is an S-expression or a filespec."
  ;;;;; ����S�� => x
  ;; ���ϥե�������Υ���ܥ���ϡ�*code-package*����Ͽ����롣
  (when (or (pathnamep x) (stringp x))
    (unless (setq input-file (or (and (probe-file x) x)
                                 (let ((sc-file (change-extension x "sc")))
                                   (and (probe-file sc-file) sc-file))))
      (error "Input file ~S not found." x))
    (setq input-file-path
      (make-pathname :directory (pathname-directory input-file)))
    (setq x (sc-file:read-sc-file input-file)))
  ;; ���ϥե�����̾
  (setq output-file (make-output-filename output-file input-file))

  ;; append user's predefinitions
  (setq x (append (make-system-predefined-macros
                   :input-file input-file :rule-list rule-list)
                  predefinitions x))

  ;; �Ѵ��¹�
  (rule:with-setup-generate-id          ; prepare a table of used variables
      ;; sc2c
      (let* ((include-path (list input-file-path scr:*sc-system-path*))
             (rule-modifier (if rulelist-specified ; ���ޥ�ɥ饤��ǻ��ꤵ��Ƥ�����
                                #'identity ; �ѹ���ǧ��ʤ�
                              #'(lambda (rlist) (setq rule-list rlist))) )
             (sc2c-modifier (if sc2c-specified ; ���ޥ�ɥ饤��ǻ��ꤵ��Ƥ�����
                                #'identity ; �ѹ���ǧ��ʤ�
                              #'(lambda (new-sc2c) (setq sc2c-rule new-sc2c))))
             (ofile-modifier (if outputfile-specified ; ���ޥ�ɥ饤��ǻ��ꤵ��Ƥ�����
                                 #'identity ; �ѹ���ǧ��ʤ�
                               #'(lambda (newo)
                                   (setq output-file
                                     (make-output-filename newo input-file)))))
             (common-scpp-args (list :input-file-path input-file-path
                                     :include-path include-path
                                     :sc2c-modifier sc2c-modifier
                                     :ofile-modifier ofile-modifier)))
        ;; apply scpp (%rule ��Ŭ�Ѥ����)
        ;;(format *error-output* "~&>>> Applying SCPP...~%")
        ;;(force-output *error-output*)
        (setq x (apply #'scpp:scpp x :rule-modifier rule-modifier
                       common-scpp-args))
        ;; write to intermediate file if requried
        (when intermediate
          (write-intermediate-file (change-extension output-file "0p.init.sc")
                                   x))
        (loop for i upfrom 1
            as remrule on rule-list
            as rule = (car remrule)
            as rule-name = (car (mklist rule))
            as rule-prms = (cdr (mklist rule))
            do
              (format *error-output* "~&>>> Applying ~A ~:S...~%"
                rule-name rule-prms)
              (force-output *error-output*)
              ;; apply rule
              (setq x
                (apply #'sct-user:apply-rule
                       x rule-name rule-prms))
              ;; write to intermediate file if requried
              (when intermediate
                (write-intermediate-file 
                 (change-extension output-file (format nil "~D.~A.sc" i rule-name))
                 x))
              ;; apply scpp
              ;;(format *error-output* "~&>>> Applying SCPP...~%")
              ;;(force-output *error-output*)
              (setq x (apply #'scpp:scpp (append predefinitions x) common-scpp-args))
              ;; write to intermediate file (preprocessed) if requried
              (when intermediate
                (write-intermediate-file
                 (change-extension output-file (format nil "~Dp.~A.sc" i rule-name))
                 x))
              ;; #+allegro (excl:gc t)
              ;; #+clisp   (ext:gc)
              ))
    ;; sc2c
    (if (not sc2c-rule) (with-package "SC" (print x) t)
      (let ((rule-name (car (mklist sc2c-rule)))
            (rule-prms (cdr (mklist sc2c-rule))))
        (format *error-output* "~&>>> Applying ~A ~:S...~%"
          rule-name rule-prms)
        (force-output *error-output*)
        (setq x (apply #'sct-user:apply-rule x rule-name rule-prms))
        ;; output C file
        (if output-file
            (progn
              (write-file output-file x :overwrite t :write-string t)
              (when indent (c-indent output-file indent))
              output-file)
          x)) )
    )
  )
