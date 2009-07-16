#!/usr/bin/env gnuplot
set terminal tgif color "Times-Roman" 30 # monochrome
# set terminal epslatex color #"Times-Roman" 30
set output 'Bintree.obj' # 4x1--4x16 for small problem

set multiplot
## common settings

set grid noxtics ytics
set key right bottom

unset logscale xy
set xrange [1:8]
set xtic 1
set yrange [0:4]
set ytic 1

# set lmargin 6

set size 1,1                   # set size 1,0.75
set origin 0,0                 # set origin 0,0.25
set xlabel '# of cores'
set ylabel 'speedup' # 'elapsed time (sec)'
# set xtics ("1x4" 4, "2x4" 8, "4x4" 16, "8x4" 32, "16x4" 64) # set xtics 4 # set format x ""
# set bmargin 0

# see *.dat for yposition
# set label "   0.534s (Fib)"       at 64,11.8 font "Times-Roman,30"
# set label "   2.41s (Nqueens)"    at 64,30.8 font "Times-Roman,30"
# set label "   0.772s (Pentomino)" at 64,17.0 font "Times-Roman,30"
# set label "   0.580s (Comp)"      at 64,7.43 font "Times-Roman,30"
# set label "   0.377s (Grav)"      at 64,13.2 font "Times-Roman,30"
# set label "   36.1s (LU) "        at 64,0.24  font "Times-Roman,30"


plot \
"Bintree-Cilk_cas.dat"  using 1:2 title "Cilk_cas"     with linespoints lt 1 pt 6,\
"Bintree-Cilk_membar.dat"  using 1:2 title "Cilk_membar"     with linespoints lt 1 pt 2,\
"Bintree-Cilk_R_cas.dat"  using 1:2 title "Cilk_R_cas"     with linespoints lt 3 pt 6,\
"Bintree-Cilk_R_membar.dat"  using 1:2 title "Cilk_R_membar"     with linespoints lt 3 pt 2,\
"Bintree-Tascell_cas.dat"  using 1:2 title "Tascell_cas"     with linespoints lt 4 pt 6,\
"Bintree-Tascell_membar.dat"  using 1:2 title "Tascell_membar"     with linespoints lt 4 pt 2

# "Bintree-serial_call.dat" using 1:2 title "serial_call"  with linespoints,\
# "Bintree-serial_call_cas.dat" using 1:2 title "serial_call_cas" with linespoints,\
# "Bintree-serial_call_membar.dat" using 1:2 title "serial_call_membar" with linespoints,\

# 0 title "LU(2000)" with linespoints linetype 7 pointtype 7

# "ideal64-4.dat"  using 1:2 title "ideal"     with linespoints,\
# "fib40.dat"  using 1:2 title "Fib(40)"     with linespoints,\
# "fib42.dat"  using 1:2 title "Fib(42)"     with linespoints,\
# "fib44.dat"  using 1:2 title "Fib(44)"     with linespoints,\
# "nq15.dat"   using 1:2 title "Nqueens(15)"      with linespoints,\
# "pen12.dat"   using 1:2 title "Pentomino(12)"      with linespoints,\
# "comp30000.dat" using 1:2 title "Comp(32768)" with linespoints,\
# "lu2048.dat"    using 1:2 title "LU(2048)"    with linespoints

set nomultiplot
