;;;; Compile-time options

;;; Debug print
(%defconstant VERBOSE 1)                ; non-nil: Debug print ON
(%if* VERBOSE
 (%defmacro DEBUG-STMTS (n &rest stmts) `(if (>= option.verbose ,n) (begin ,@stmts)))
 (%defmacro DEBUG-PRINT (n &rest args)
  `(if (>= option.verbose ,n) (csym::fprintf stderr ,@args)))
 %else
 (%defmacro DEBUG-STMTS (n &rest stmts) `(begin))
 (%defmacro DEBUG-PRINT (n &rest args) `(begin)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(%defconstant BUFSIZE 1280)             ; コマンド行の文字数の最大+1
(%defconstant MAXCMDC 4)                ; コマンドのargument数の最大（コマンド名自身含む）
(%defconstant ARG-SIZE-MAX 16)          ; コマンドの各引数の許される長さ
(%defconstant TASK-LIST-LENGTH 1024)    ; スレッドごとのTASK, TASK-HOMEリストの長さ
(%defconstant TASK-MAX 256)             ; プログラマが定義できるタスクの最大数
(%defconstant DUMMY-SIZE 1000)          ; false-sharing防止のためのpaddingサイズ

(%defconstant DELAY-MAX (* 1 1000 1000 1000))          ; none時->treq再送信までの時間の上限 [nsec]
; (%defconstant BUSYWAIT)                 ; ワーカがtreqの返事をbusywaitで待つならuncomment

(%cinclude "sock.h" (:macro))           ; 通信関係

;;;; Declarations

;; 0以上の数はthread-idに相当するので，enum idには負の数を割り当てる
(def (enum node) (OUTSIDE -1) (INSIDE -2) (ANY -3) (PARENT -4) (FORWARD -5) (TERM -99))
(def (enum command)
    TASK RSLT TREQ NONE BACK RACK DREQ DATA
    STAT VERB EXIT WRNG)
(extern-decl cmd-strings (array (ptr char))) ; ↑に対応する文字列．cmd-serial.scで定義．

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

(decl (struct task))
(decl (struct thread-data))

;;; ☆ do_task_body 以外は，thread_data 引数が不要では?
;;; Tascellプログラム側で（自動）定義するtask objectのsender/receiver
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

;;; Tascellプログラム側で定義する要求時取得データのallocator/sender/receiver．
;; 引数はデータのサイズ＝data-flagの数
;; -init-data が data-flagsの初期化後に呼出す
(decl (csym::data-allocate) (csym::fn void int))
;; 引数の整数は送受信するデータの範囲（data-flagsの添字に対応する整数）
(decl (csym::data-send) (csym::fn void int int))
(decl (csym::data-receive) (csym::fn void int int))

;;; worker local storage の構造体および初期化関数（定義はユーザプログラムで）
(decl (struct worker-data))
(decl (csym::worker-init) (csym::fn void (ptr (struct thread-data))))

;;; Tascellプログラマに提供する機能 (worker.scで定義)
;;; （Tascellでは最初の 'csym::-' を除いた名前．request-dataの先頭引数thrはtcell.ruleが追加）
;; データ領域の確保およびフラグを初期化（引数はデータのサイズ＝data-flagの数）
;; 複数回呼出しても一度しか実行されない
(decl (csym::-setup-data) (csym::fn void int))
;; 親タスクに指定された範囲のデータのdreqを発行する
(decl (csym::-request-data) (csym::fn void (ptr (struct thread-data)) int int))
;; 指定された範囲のデータが揃うまで待つ
(decl (csym::-wait-data) (csym::fn void int int))
;; 指定された範囲のdata-flagsをDATA-EXISTにする（仕事開始ノード用）
(decl (csym::-set-exist-flag) (csym::fn void int int))

(def (enum task-stat)
  TASK-ALLOCATED   ; 領域のみ．未初期化
  TASK-INITIALIZED ; セットされている
  TASK-STARTED     ; 実行開始済
  TASK-DONE        ; 完了済み
  TASK-NONE        ; ALLOC後treqしたがnoneが返ってきた
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

;; 計算するworkerにまで移動してきたタスク
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
  (def id int)                          ; 初期化時に割当てられるID（各ワーカで一意）
  (def owner (ptr (struct task)))       ; このサブタスクをspawnしたタスク
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
  (def wdptr (ptr void))                ; worker local storage構造体（ユーザが定義）へのポインタ
  (def dummy (array char DUMMY-SIZE)) ; false sharing防止のpadding
  )

;;;; 必要時データ要求関連

;; 存在フラグの種類
(def (enum DATA-FLAG) DATA-NONE DATA-REQUESTING DATA-EXIST)

;; dreq処理関数に渡す引数
(def (struct dhandler-arg)
  (def data-to (enum node))                   ; データのrequester (INSIDE|OUTSIDE)
  (def head (array (enum node) ARG-SIZE-MAX)) ; データのrequester
  (def dreq-cmd (struct cmd))           ; さらに親にdreqを投げる際の雛形 (for DATA-NONE)
  (def dreq-cmd-fwd (struct cmd))       ; さらに親にdreqを投げる際の雛形 (for DATA-REQUESTING)
  (def start int)                       ; データの要求範囲
  (def end int)                         ; データの要求範囲
  )


;;;; worker.sc の関数プロトタイプ宣言
(decl (csym::make-and-send-task thr task-no body)
      (csym::fn void (ptr (struct thread-data)) int (ptr void)))
(decl (wait-rslt thr) (fn (ptr void) (ptr (struct thread-data))))

(decl (csym::proto-error str pcmd) (csym::fn void (ptr (const char)) (ptr (struct cmd))))
(decl (csym::read-to-eol) (csym::fn void void))

(decl (csym::init-data-flag) (csym::fn void int))

(decl (csym::recv-rslt) (csym::fn void (ptr (struct cmd)) (ptr void)))
(decl (csym::recv-task) (csym::fn void (ptr (struct cmd)) (ptr void)))
(decl (csym::recv-treq) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-rack) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-dreq) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-data) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-none) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-back) (csym::fn void (ptr (struct cmd))))
(decl (csym::print-task-list task-top name) (csym::fn void (ptr (struct task)) (ptr char)))
(decl (csym::print-task-home-list treq-top name) (csym::fn void (ptr (struct task-home)) (ptr char)))
(decl (csym::print-thread-status thr) (csym::fn void (ptr (struct thread-data))))
(decl (csym::print-status) (csym::fn void (ptr (struct cmd))))
(decl (csym::set-verbose-level) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-exit) (csym::fn void (ptr (struct cmd))))

;;;; cmd-serial.sc の関数プロトタイプ宣言
(decl (csym::serialize-cmdname buf w) (fn int (ptr char) (enum command)))
(decl (csym::deserialize-cmdname buf str) (fn int (ptr (enum command)) (ptr char)))
(decl (csym::serialize-arg buf arg) (fn int (ptr char) (ptr (enum node))))
(decl (csym::deserialize-node str) (fn (enum node) (ptr char)))
(decl (csym::deserialize-arg buf str) (fn int (ptr (enum node)) (ptr char)))
(decl (csym::serialize-cmd buf pcmd) (fn int (ptr char) (ptr (struct cmd))))
(decl (csym::deserialize-cmd pcmd str) (fn int (ptr (struct cmd)) (ptr char)))
(decl (csym::copy-address dst src) (fn int (ptr (enum node)) (ptr (enum node))))
(decl (csym::address-equal adr1 adr2) (fn int (ptr (enum node)) (ptr (enum node))))

;;;; Command line options
(%defconstant HOSTNAME-MAXSIZE 256)
(def (struct runtime-option)
  (def num-thrs int)                    ; worker数
  (def sv-hostname (array char HOSTNAME-MAXSIZE))
                                        ; Tascellサーバのホスト名．""ならstdout
  (def port unsigned-short)             ; Tascellサーバへの接続ポート番号
  (def initial-task (ptr char))         ; 自動的に最初に投入するタスクパラメータ
  (def auto-exit int)                   ; 外部に最初のrsltを送ったら自動終了
  (def affinity int)                    ; use sched_setaffinity
  (def prefetch int)                    ; 投機的に外部へtreq
  (def verbose int)                     ; verbose level
  )
(extern-decl option (struct runtime-option))
