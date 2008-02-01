(%defconstant BUFSIZE 1024)
(%defconstant MAXCMDC 4)

(%defconstant FN fn)  ;; fn or lightweight

(def (enum node) OUTSIDE INSIDE ANY)

(def (struct cmd)
  (def c int)                           ; コマンドのargument数（コマンド名自身も含む）
  (def node (enum node))
  (def v (array (ptr char) MAXCMDC))
  )

(def (struct cmd-list)
  (def cmd (struct cmd))
  (def body (ptr void))
  (def next (ptr (struct cmd-list)))
  )

(decl (csym::recv-rslt) (fn void (struct cmd) (ptr void)))
(decl (csym::recv-task) (fn void (struct cmd) (ptr void)))
(decl (csym::recv-treq) (fn void (struct cmd)))
(decl (csym::recv-rack) (fn void (struct cmd)))
(decl (recv-none) (fn void (struct cmd)))

(decl (struct task))
(decl (struct thread-data))

(decl (do-task-body)
  (fn void (ptr (struct thread-data)) (ptr void)))
(decl (send-task-body)
  (fn void (ptr (struct thread-data)) (ptr void)))
(decl (csym::recv-task-body)
  (fn (ptr void) (ptr (struct thread-data))))
(decl (send-rslt-body)
  (fn void (ptr (struct thread-data)) (ptr void)))
(decl (recv-rslt-body)
  (fn void (ptr (struct thread-data)) (ptr void)))

(def (enum task-stat)
  TASK-ALLOCATED TASK-INITIALIZED TASK-STARTED TASK-DONE
  TASK-NONE TASK-SUSPENDED)

(def (enum task-home-stat)
  TASK-HOME-ALLOCATED TASK-HOME-INITIALIZED
  ;;  結果待ちは？ taskのほうでわかる？
  TASK-HOME-DONE)

(def (struct task)
  (def stat (enum task-stat))       ; タスクの状態
  (def next (ptr (struct task)))    ;; 双方向リスト
  (def prev (ptr (struct task)))    ;;
  (def body (ptr void))             ; task-data構造体へのポインタ
  (def ndiv int)                    ; 分割回数
  (def rslt-to (enum node))         ; 結果送信先の種別
  (def rslt-head (array char 256))) ; 結果送信先アドレス

;; タスク作成，結果待ちスタックの要素
(def (struct task-home)
  (def stat (enum task-home-stat))
  (def id int)                         ; 初期化時に割当てられるID 
  (def req-from (enum node))
  (def next (ptr (struct task-home)))
  (def body (ptr void))
  (def task-head (array char 256)))    ; タスクを要求してる人のアドレス

(def (struct thread-data)
  (def req (volatile (ptr (struct task-home)))) ; 仕事の要求が来ているか？
  (def id int)        ; 初期化時に割当てられるID
  (def w-rack int)    ; （rsltを送信して）rack待ちの数
  (def w-none int)    ; 
  (def ndiv int)      ; 仕事が分割された回数
  (def task-free (ptr (struct task)))      ; タスク確保用フリーリスト
  (def task-top (ptr (struct task)))       ; スレッドに与えられた仕事のリスト
  (def treq-free (ptr (struct task-home))) ; タスク作成待ちスタック確保用フリーリスト
  (def treq-top (ptr (struct task-home)))  ; 上記スタックのトップ
  (def sub (ptr (struct task-home)))       ; 結果待ちのタスク情報スタックのトップ
  (def mut pthread-mutex-t)
  (def rack-mut pthread-mutex-t)
  (def cond pthread-cond-t)           ; task, none待ちで眠らせるときの条件変数
  (def cond-r pthread-cond-t)         ; rslt待ちで眠らせるときの条件変数
  (def ndiv-buf (array char 32))
  (def id-str (array char 32))
  (def buf (array char BUFSIZE)))

(extern divisibility-flag int)

;; Cのプミリミティブ型のSender, Reciever
(decl (csym::send-int n) (fn void int))
(decl (csym::recv-int n) (fn void (ptr int)))

