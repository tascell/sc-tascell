#/bin/sh
# Nehalem
graphs="11 5 6 4" # 1:random(4M) 2:2dt(2000) 3:ncube(20) 4: bintree(24) 5: ncube(21)
                  # 6:2dt(4000) 11:random4000000.graph
                  # 20000 + n (Nehalem Cilk or Tascell only): 2dt(n)
                  # 30000 + n (Nehalem Cilk or Tascell only): ncube(n)
# to overwrite default
graphs="20016 20032 20064 20128 20256 20512 21024 22048 24096"
# graphs="30012 30013 30014 30015 30016 30017 30018 30019 30020"

procs="1 2 3 4 5 6 7 8"
# to overwrite default
procs="4"

time=3
cilk_dir=cilk
tcell_dir=../pact10-nehalem-tcell

## PACT10 comments
# 2010/3/26: 2dt wo 2000 -> 4000 ni henkou
# 2010/3/26: Cilk_R mo noseru
# 2010/3/27: PACT10 deha CAS only (faster one)

# ・評価 + xyグラフなどをふくめて，平石君中心でお願いできますでしょうか?
#   1,2,3,4,5,6,7,8 コアすべてで評価すればよいかと思います．
#   できたらできるだけ文章も? 

# ・組み合わせは
#    PARALLEL_SEARCH2 は使わない
#    PARALLEL_SEARCH と PARALLEL_SEARCH3 を使う．

for g in $graphs; do
# #   *  serial (逐次Cで，単に頂点スタックのみ search_s2)
    # ntimes $time ./st-serial-call $g 1

# *  serial_call (逐次Cで，呼び出しと頂点スタックを使う，
#          TascellやCilkでの並列化はされていない．
#          cas_int, membar (XXX_to_start_readなど) などは使わない．)
    # ntimes $time ./st-serial-call $g 2

#   *  serial_call_cas  (逐次Cで，呼び出しと頂点スタックを使う，
#          TascellやCilkでの並列化はされていない．
#          cas_int (PARALLEL_SEARCH )を使う．)
    # ntimes $time ./st-serial-call-cas $g 2

#   *  serial_call_membar  (逐次Cで，呼び出しと頂点スタックを使う，
#          TascellやCilkでの並列化はされていない．
#          membar (PARALLEL_SEARCH3) を使う．)
    # ntimes $time ./st-serial-call-membar $g 2

#   CALL_BOUND = 800
    # ntimes $time ./st-serial-call-800 $g 2
    # ntimes $time ./st-serial-call-cas-800 $g 2
    # ntimes $time ./st-serial-call-membar-800 $g 2
    
    for p in $procs; do
        echo
#   *  SYNCHED を使わない Cilk があってもよいかな...
        ntimes $time $cilk_dir/affinity $p $cilk_dir/st-par-cilk-s --nproc $p $g 2
        # ntimes $time $cilk_dir/affinity $p $cilk_dir/st-par3-cilk-s --nproc $p $g 2
#   *  Cilk_cas
#   *  Cilk_membar
        # ntimes $time $cilk_dir/affinity $p $cilk_dir/st-par-cilk --nproc $p $g 2
        # ntimes $time $cilk_dir/affinity $p $cilk_dir/st-par3-cilk --nproc $p $g 2
#   CALL_BOUND = 800
        # ntimes $time $cilk_dir/affinity $p $cilk_dir/st-par-cilk-s-800 --nproc $p $g 2
        # ntimes $time $cilk_dir/affinity $p $cilk_dir/st-par3-cilk-s-800 --nproc $p $g 2
        # ntimes $time $cilk_dir/affinity $p $cilk_dir/st-par-cilk-800 --nproc $p $g 2
        # ntimes $time $cilk_dir/affinity $p $cilk_dir/st-par3-cilk-800 --nproc $p $g 2
#   CALL_BOUND = 1G
        ntimes $time $cilk_dir/affinity $p $cilk_dir/st-par-cilk-s-1G --nproc $p $g 2
        # ntimes $time $cilk_dir/affinity $p $cilk_dir/st-par3-cilk-s-1G --nproc $p $g 2
        # ntimes $time $cilk_dir/affinity $p $cilk_dir/st-par-cilk-1G --nproc $p $g 2
        # ntimes $time $cilk_dir/affinity $p $cilk_dir/st-par3-cilk-1G --nproc $p $g 2
        
#   *  Tascell_cas    (list->array はどうしましょうか....)
#   *  Tascell_membar
        # ntimes $time $tcell_dir/spanning-clos-cas-30 -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-clos-membar-30 -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-lw-cas-30 -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-lw-membar-30 -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-xcc-cas-30 -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-xcc-membar-30 -n $p -a -i "4 $g 3 0 0"
        ntimes $time $tcell_dir/spanning-xcccl-cas-30 -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-xcccl-membar-30 -n $p -a -i "4 $g 3 0 0"
#   CALL_BOUND = 800
        # ntimes $time $tcell_dir/spanning-clos-cas-800 -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-clos-membar-800 -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-lw-cas-800 -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-lw-membar-800 -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-xcc-cas-800 -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-xcc-membar-800 -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-xcccl-cas-800 -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-xcccl-membar-800 -n $p -a -i "4 $g 3 0 0"
#   CALL_BOUND = 1G
        # ntimes $time $tcell_dir/spanning-clos-cas-1G -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-clos-membar-1G -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-lw-cas-1G -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-lw-membar-1G -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-xcc-cas-1G -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-xcc-membar-1G -n $p -a -i "4 $g 3 0 0"
        ntimes $time $tcell_dir/spanning-xcccl-cas-1G -n $p -a -i "4 $g 3 0 0"
        # ntimes $time $tcell_dir/spanning-xcccl-membar-1G -n $p -a -i "4 $g 3 0 0"
        
# #   *  Tascell_gcc_cas
#         ntimes $time ./spanning-gcc-cas -n $p -a -i "4 $g 3 0 0"

# #   *  Tascell_gcc_membar
#         ntimes $time ./spanning-gcc-membar -n $p -a -i "4 $g 3 0 0"
    done
done


# ・グラフにあと，tree を足してもらえませんか?
#   先週は冗長な木という話をしましたが，単なる木のほうがよいと思い
#   直しました．頂点 i が 頂点 (i-1)/2 と枝を作ればよいです．
#   (我々の方法ではとくに問題ないですが，
#    既存研究の batching がうまくいかないのは自明なので)

# ・性能評価とは別に，search
#      (= list (search1 v0))
#      (do-while (no-empty list) 
#         (= mylist list) (= list nil)
#         (for each v in mylist (+= list (search v))))
#   と考えたときの
#    * do-while の繰り返し回数
#    * 各回の list 長
#    * 各回で visit 済の頂点数 (全頂点を走査すればよいです．
#          キャッシュの状態に影響は与えそうですが仕方ないです．
#          ちなみに，PARALLEL_SEARCH3 では，複数のworker が一つの頂点を
#          getしたつもりが，あるworkerの書き込みのみがうまく残るというの
#          がありますので，その部分では正確にカウントできません)
#   を測定してもらえないでしょうか?
#   こうしてみると do-while ではなく while とすべきですね．

# -- 
# 八杉

