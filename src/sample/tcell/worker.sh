(%defconstant BUFSIZE 1280)             ; コマンド行の文字数の最大+1
(%defconstant MAXCMDC 4)                ; コマンドのargument数の最大（コマンド名自身含む）
(%defconstant ARG-SIZE-MAX 16)          ; コマンドの各引数の許される長さ
(%defconstant TASK-LIST-LENGTH 1024)    ; スレッドごとのTASK, TASK-HOMEリストの長さ
(%defconstant TASK-MAX 256)             ; プログラマが定義できるタスクの最大数

(%defconstant clsc)

(%cinclude "sock.h" (:macro))           ; 通信関係

(def (enum node) (OUTSIDE -1) (INSIDE -2) (ANY -3) (PARENT -4) (TERM -5))
(def (enum command) TASK RSLT TREQ NONE RACK EXIT WRNG)
(extern-decl cmd-strings (array (ptr char))) ; ↑に順序が対応

;; treq any の相手選択方法
(def (enum choose) CHS-RANDOM CHS-ORDER)
(%defconstant NKIND-CHOOSE 2)           ; chooseの種類数

;; ワーカ間でやりとりするコマンド
;; task, rsltの本体はcmd-listにある
(def (struct cmd)
  (def w (enum command))                ; コマンドの種類
  (def c int)                           ; コマンドのargument数（コマンド名自身も含む）
  (def node (enum node))                ; どこに送るメッセージか INSIDE|OUTSIDE|ANY
  (def v (array (enum node) MAXCMDC ARG-SIZE-MAX)) ; v[i]: i-th argument of the command
                                        ; TERMでおわる[enum定数|0以上の整数]の配列
  )

(def (struct cmd-list)
  (def cmd (struct cmd))
  (def body (ptr void))              ; task, rsltの本体（タスク種類依存の構造体）
  (def task-no int)                  ; task番号（bodyの送信関数を決定するため）
  (def next (ptr (struct cmd-list)))
  )

(decl (csym::proto-error str pcmd) (csym::fn void (ptr (const char)) (ptr (struct cmd))))

(decl (csym::read-to-eol) (csym::fn void void))

(decl (csym::recv-rslt) (csym::fn void (ptr (struct cmd)) (ptr void)))
(decl (csym::recv-task) (csym::fn void (ptr (struct cmd)) (ptr void)))
(decl (csym::recv-treq) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-rack) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-none) (csym::fn void (ptr (struct cmd))))

(decl (struct task))
(decl (struct thread-data))

;; (decl (do-task-body)
;;   (fn void (ptr (struct thread-data)) (ptr void)))
;; (decl (send-task-body)
;;   (fn void (ptr (struct thread-data)) (ptr void)))
;; (decl (csym::recv-task-body)
;;   (fn (ptr void) (ptr (struct thread-data))))
;; (decl (send-rslt-body)
;;   (fn void (ptr (struct thread-data)) (ptr void)))
;; (decl (recv-rslt-body)
;;   (fn void (ptr (struct thread-data)) (ptr void)))
;;; ☆ do_task_body 以外は，thread_data 引数が不要では?
(decl task-doers
      (array (ptr (fn void (ptr (struct thread-data)) (ptr void))) TASK-MAX))
(decl task-senders
      (array (ptr (csym::fn void (ptr void))) TASK-MAX))
(decl task-receivers
      (array (ptr (csym::fn (ptr void))) TASK-MAX))
(decl rslt-senders
      (array (ptr (csym::fn void (ptr void))) TASK-MAX))
(decl rslt-receivers
      (array (ptr (csym::fn void (ptr void))) TASK-MAX))

(def (enum task-stat)
  TASK-ALLOCATED   ; 領域のみ．未初期化
  TASK-INITIALIZED ; セットされている
  TASK-STARTED     ; 実行開始済
  TASK-DONE        ; 完了済み
  TASK-NONE        ; 空っぽ？ TASK-ALLOCATEDとは違うの？
  TASK-SUSPENDED)  ; 待ち状態
;; -(treq送信)-> ALLOCATED --(task受け取り)--> INITIALIZED --> STARTED --> DONE -->
;;                 |  |none受け取り                               |
;;           再挑戦|  |                                           |
;;                 NONE                                        SUSPENDED
                  
(def (enum task-home-stat)
  TASK-HOME-ALLOCATED    ; 領域のみ，未初期化
  TASK-HOME-INITIALIZED  ; セットされている
  ;;  結果待ちは？ taskのほうでわかる？
  TASK-HOME-DONE)        ; 結果が求まっている

;; 計算するプロセッサにまで移動してきたタスク
(def (struct task)
  (def stat (enum task-stat))       ; タスクの状態
  (def next (ptr (struct task)))    ; 双方向リスト......
  (def prev (ptr (struct task)))    ; ..........のリンク
  (def task-no int)                 ; 実行するタスク番号（tcell追加）
  (def body (ptr void))             ; task object構造体へのポインタ
  (def ndiv int)                    ; 何回分裂してできたタスク (task cell)か
  (def rslt-to (enum node))         ; 結果送信先の種別（別ノード or ノード内 or any）
  (def rslt-head (array (enum node) ARG-SIZE-MAX))) ; 結果送信先アドレス

;; Workerが作ったサブタスク管理情報
(def (struct task-home)
  (def stat (enum task-home-stat))      ; 状態
  (def id int)                          ; 初期化時に割当てられるID（各計算ノードで一意）
  (def task-no int)                     ; 実行するタスク番号（tcell追加）
  (def req-from (enum node))            ; 仕事送信先の種別（別ノード or ノード内 or any）
  (def next (ptr (struct task-home)))   ; リンク（次の空きセル or スタックの1コ下）
  (def body (ptr void))                 ; task-data構造体へのポインタ
  (def task-head (array (enum node) ARG-SIZE-MAX))
                                        ; タスクの送り先（取り返し先，rackの送り先）
  )

(def (struct thread-data)
  (def req (ptr (struct task-home)))    ; 仕事の要求が来ているか？
  (def id int)                          ; 初期化時に割当てられるID
  (def w-rack int)                      ; （rsltを送信して）rack待ちの数
  (def w-none int)                      ; none待ちの数
  (def ndiv int)                        ; 今やってる仕事の分割された回数
  (def last-treq int)                   ; 内部へのtreq anyで，最後にtreqした相手
  (def last-choose (enum choose))       ; 内部へのtreq anyで，最後に採用した選択方法
  (def random-seed1 double)             ; 乱数の種 treq anyで使用・更新
  (def random-seed2 double)             ; 乱数の種 treq anyで使用・更新
  (def task-free (ptr (struct task)))   ; タスク確保用フリーリスト
  (def task-top (ptr (struct task)))    ; スレッドに与えられた仕事のリスト
  ;; treq-freeから始まるフリーリストから
  ;; 2つのスタック（treq-topスタックとsubスタック）を確保している
  (def treq-free (ptr (struct task-home))) ; タスク作成待ちスタック確保用フリーリスト
  (def treq-top (ptr (struct task-home))) ; 上記スタックのトップ
  (def sub (ptr (struct task-home)))    ; サブタスク(initialized)スタック．
  (def mut pthread-mutex-t)             ; mutex
  (def rack-mut pthread-mutex-t)        ; rack mutex
  (def cond pthread-cond-t)             ; task, none待ちで眠らせるときの条件変数
  (def cond-r pthread-cond-t)           ; rslt待ちで眠らせるときの条件変数
  )

(decl (csym::make-and-send-task thr task-no body)
      (csym::fn void (ptr (struct thread-data)) int (ptr void)))
(decl (wait-rslt thr) (fn (ptr void) (ptr (struct thread-data))))


;;;; cmd-serial.sc
(decl (csym::serialize-cmdname buf w) (fn int (ptr char) (enum command)))
(decl (csym::deserialize-cmdname buf str) (fn int (ptr (enum command)) (ptr char)))
(decl (csym::serialize-arg buf arg) (fn int (ptr char) (ptr (enum node))))
(decl (csym::deserialize-node str) (fn (enum node) (ptr char)))
(decl (csym::deserialize-arg buf str) (fn int (ptr (enum node)) (ptr char)))
(decl (csym::serialize-cmd buf pcmd) (fn int (ptr char) (ptr (struct cmd))))
(decl (csym::deserialize-cmd pcmd str) (fn int (ptr (struct cmd)) (ptr char)))
(decl (csym::copy-address dst src) (fn int (ptr (enum node)) (ptr (enum node))))
