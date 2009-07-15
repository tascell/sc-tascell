#/bin/sh
graphs="1 2 3 4"
procs="1 8"
time=1

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

for g in $graphs; do
    for p in $procs; do
#   *  Cilk_cas
#   *  Cilk_membar
        
#   *  Tascell_cas    (list->array はどうしましょうか....)
#        ntimes $time ./spanning-lw-cas -n $p -a -i "4 $g 3 0 0"

#   *  Tascell_membar
        ntimes $time ./spanning-lw-membar-statistics -n $p -a -i "4 $g 3 0 0"

#   *  Tascell_gcc_cas
#        ntimes $time ./spanning-gcc-cas -n $p -a -i "4 $g 3 0 0"

#   *  Tascell_gcc_membar
#        ntimes $time ./spanning-gcc-membar -n $p -a -i "4 $g 3 0 0"
    done
done


