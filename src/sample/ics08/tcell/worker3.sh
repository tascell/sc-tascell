(%defconstant BUFSIZE 1280)             ; コマンド行の文字数の最大+1
(%defconstant MAXCMDC 5)                ; コマンドのargument数の最大（コマンド名自身含む）
(%defconstant CMD-QUEUE-LENGTH 512)     ; コマンド送受信各キューの長さ
(%defconstant ARG-SIZE-MAX 256)         ; コマンドの各引数の許される長さ
(%defconstant TASK-LIST-LENGTH 256)     ; スレッドごとのTASK, TASK-HOMEリストの長さ
(%defconstant TASK-MAX 256)             ; プログラマが定義できるタスクの最大数

(%defconstant clsc)

(%cinclude "sock.h" (:macro))           ; 通信関係

(def (enum node) OUTSIDE INSIDE ANY)

;; ワーカ間でやりとりするコマンド
;; task, rsltの本体はcmd-listにある
(def (struct cmd)
  (def c int)                           ; コマンドのargument数（コマンド名自身も含む）
  (def node (enum node))
  (def v (array (ptr char) MAXCMDC))    ; v[i]: i-th argument of the command
                                        ; 文字列の領域は別に確保
  )

(def (struct cmd-list)
  (def cmd (struct cmd))
  (def body (ptr void))              ; task, rsltの本体（タスク種類依存の構造体）
  (def task-no int)                  ; task番号（bodyの送信関数を決定するため）
  (def next (ptr (struct cmd-list)))
  )

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

(def (enum task-home-stat)
  TASK-HOME-ALLOCATED    ; 領域のみ，未初期化
  TASK-HOME-INITIALIZED  ; セットされている
  ;;  結果待ちは？ taskのほうでわかる？
  TASK-HOME-DONE)        ; 結果が求まっている

;; 計算するプロセッサにまで移動してきたタスク
(def (struct task)
  (def stat (enum task-stat))       ; タスクの状態
  (def next (ptr (struct task)))    ;; 双方向リスト
  (def prev (ptr (struct task)))    ;;
  (def task-no int)                 ; 実行するタスク番号（tcell追加）
  (def body (ptr void))             ; task-data構造体へのポインタ
  (def ndiv int)                    ; 何回分裂してできたタスク (task cell)か
  (def rslt-to (enum node))         ; 結果送信先の種別（別ノード or ノード内 or any）
  (def rslt-head (array char 256))) ; 結果送信先アドレス

;; 分割してできたタスク．遠隔移動元
(def (struct task-home)
  (def stat (enum task-home-stat))     ; 状態
  (def id int)                         ; 初期化時に割当てられるID（各計算ノードで一意）
  (def task-no int)                    ; 実行するタスク番号（tcell追加）
  (def req-from (enum node))           ; 仕事送信先の種別（別ノード or ノード内 or any）
  (def next (ptr (struct task-home)))  ; リンク（次の空きセル or スタックの1コ下）
  (def body (ptr void))                ; task-data構造体へのポインタ
  (def task-head (array char 256)))    ; タスクの送り先（取り返し先，rackの送り先）

(def (struct thread-data)
  (def req (ptr (struct task-home)))    ; 仕事の要求が来ているか？
  (def id int)        ; 初期化時に割当てられるID
  (def w-rack int)    ; （rsltを送信して）rack待ちの数
  (def w-none int)    ; none待ちの数
  (def ndiv int)      ; 仕事が分割された回数
  (def task-free (ptr (struct task)))      ; タスク確保用フリーリスト
  (def task-top (ptr (struct task)))    ; スレッドに与えられた仕事のリスト
  ;; treq-freeから始まるフリーリストから
  ;; treq-topスタックとsubスタックを確保している
  (def treq-free (ptr (struct task-home))) ; タスク作成待ちスタック確保用フリーリスト
  (def treq-top (ptr (struct task-home)))  ; 上記スタックのトップ
  (def sub (ptr (struct task-home)))       ; サブタスク(initialized)スタック．
  (def mut pthread-mutex-t)           ; mutex
  (def rack-mut pthread-mutex-t)      ; rack mutex
  (def cond pthread-cond-t)           ; task, none待ちで眠らせるときの条件変数
  (def cond-r pthread-cond-t)         ; rslt待ちで眠らせるときの条件変数
  (def ndiv-buf (array char 32))
  (def tno-buf (array char 8))        ; task-no送信用文字列バッファ（tcell追加）
  (def id-str (array char 32))        ; id の文字列表現
  (def buf (array char ARG-SIZE-MAX)))  ; thread local バッファ

(extern divisibility-flag int) ;★ 消してしまいたい
(decl (csym::send-divisible) (csym::fn void void))
(decl (csym::make-and-send-task thr task-no body)
      (csym::fn void (ptr (struct thread-data)) int (ptr void)))
(decl (wait-rslt thr) (fn (ptr void) (ptr (struct thread-data))))

;; Cのプミリミティブ型のSender, Reciever
(decl (csym::send-int n) (csym::fn void int))
(decl (csym::recv-int) (csym::fn int void))
