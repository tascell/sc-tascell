(defpackage "QUEUE"
  (:use "CL")
  (:export "QUEUE" "SHARED-QUEUE" "ADD-QUEUE" "EMPTY-QUEUE-P"
           "DELETE-QUEUE" "FIND-DELETE-QUEUE"
           "QUEUE-BODY" "SQ-LOCK" "SQ-GATE")
  ;; sc-misc.lsp のキュー関連の関数との衝突を防ぐ
  (:shadow #:empty-queue-p #:delete-queue #:find-delete-queue))
(in-package "QUEUE")

;;;; queueクラスの定義
(defclass queue ()
  ((body :accessor queue-body :type list :initform (misc:make-queue))))

(defclass shared-queue (queue)
  ((lock :accessor sq-lock :type mp:process-lock :initform (mp:make-process-lock))
   (gate :accessor sq-gate :initform (mp:make-gate nil)))) ; to notify addition


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; queueへの追加
(defgeneric add-queue (elm q))
(defmethod add-queue (elm (q queue))
  (misc:insert-queue elm (queue-body q)))

(defmethod add-queue :around (elm (sq shared-queue))
  (declare (ignorable elm))
  (mp:with-process-lock ((sq-lock sq))
    (prog1 (call-next-method)
      (mp:open-gate (sq-gate sq)))))

;; 空queue
(defgeneric empty-queue-p (q))
(defmethod empty-queue-p ((q queue))
  (misc:empty-queue-p (queue-body q)))

;; queueから取り出し
(defgeneric delete-queue (q))
(defmethod delete-queue ((q queue))
  (misc:delete-queue (queue-body q)))

(defmethod delete-queue :around ((sq shared-queue))
  (mp:with-process-lock ((sq-lock sq))
    (prog1 (call-next-method)
      (when (empty-queue-p sq)
        (mp:close-gate (sq-gate sq))))))

;; queueから検索して取り出し
(defgeneric find-delete-queue (q test &key key))
(defmethod find-delete-queue ((q queue) test &key (key #'identity))
  (misc:find-delete-queue (queue-body q) test :key key))

(defmethod find-delete-queue :around ((sq shared-queue) test
                                       &key (key #'identity)
                                            (wait nil))
  (declare (ignore test key))
  (block :end
    (tagbody
      :retry
      (mp:with-process-lock ((sq-lock sq))
        (let ((item (call-next-method)))
          (when item
            (when (empty-queue-p sq)
              (mp:close-gate (sq-gate sq)))
            (return-from :end item))
          (unless wait
            (return-from :end nil))
          (mp:close-gate (sq-gate sq))))
      (mp:process-wait "FIND-DELETE-QUEUE-WAIT"
                       #'mp:gate-open-p (sq-gate sq))
      (go :retry))))
