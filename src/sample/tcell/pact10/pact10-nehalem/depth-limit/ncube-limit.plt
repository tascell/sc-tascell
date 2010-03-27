#!/usr/bin/env gnuplot
#set terminal tgif color "Times-Roman" 30 # monochrome
set term postscript eps enhanced color "Times-Roman" 8
# set terminal epslatex color #"Times-Roman" 30
# set output '%1%%2%.obj'
set output 'ncube-limit.eps'

set size 0.36,0.36                   # set size 1,0.75
set pointsize 0.7

# set multiplot
## common settings

set grid noxtics ytics
set key right bottom

set logscale y
set xrange [14:20]
set xtic 1
set yrange [0.0001:1]
# set ytic 10

# set lmargin 6

# set size 1.0,1.0                   # set size 1,0.75
# set origin 0,0                 # set origin 0,0.25
set xlabel 'problem size'
set ylabel 'elapsed time (sec)' # 'elapsed time (sec)'
# set xtics ("1x4" 4, "2x4" 8, "4x4" 16, "8x4" 32, "16x4" 64) # set xtics 4 # set format x ""
# set bmargin 0

# plot \
# "%1%-Cilk_cas%2%.dat"  using 1:2 title "Cilk\\_cas"     with linespoints lt 4 pt 1,\
# "%1%-Cilk_membar%2%.dat"  using 1:2 title "Cilk\\_membar"     with linespoints lt 4 pt 2,\
# "%1%-Cilk_R_cas%2%.dat"  using 1:2 title "Cilk\\_R\\_cas"     with linespoints lt 3 pt 1,\
# "%1%-Cilk_R_membar%2%.dat"  using 1:2 title "Cilk\\_R\\_membar"     with linespoints lt 3 pt 2,\
# "%1%-Tascell_cas%2%.dat"  using 1:2 title "Tascell\\_cas"     with linespoints lt -1 pt 1,\
# "%1%-Tascell_membar%2%.dat"  using 1:2 title "Tascell\\_membar"     with linespoints lt -1 pt 2

plot \
"ncube-cilk-1G.dat"  using 1:2 title "no depth limit"   with linespoints lt 4 pt 6,\
"ncube-cilk-30.dat"  using 1:2 title "depth-limiting (w/ short-term workspaces)" with linespoints lt -1 pt 6,\
"ncube-tascell-30.dat"  using 1:2 title "depth-limiting (w/ long-term workspaces)" with linespoints lt -1 pt 1

# "ncube-tascell-1G.dat"  using 1:2 title "Tascell\\_cas (w/o depth limit)"     with linespoints lt -1 pt 4

# set nomultiplot
