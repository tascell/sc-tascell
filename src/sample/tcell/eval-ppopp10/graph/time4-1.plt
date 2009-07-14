#!/usr/bin/env gnuplot
set terminal tgif monochrome "Times-Roman" 30
set output 'time4-1.obj' # 4x1--4x16 for small problem

set multiplot
## common settings

set grid noxtics ytics
set key left top

set xrange [4:64]
set xtic 2
set ytic 2
set logscale xy

# set lmargin 6

## LU
# set size 1,0.25
# set origin 0,0
# set xlabel 'number of workers'
# set ylabel ""
# set xtics ("1x4" 4, "2x4" 8, "4x4" 16, "8x4" 32, "16x4" 64)
# set tmargin 0

# set yrange [0.125:4]
set ytic 4
# タイトルは後で表示
# plot\
# "lu2000-4.dat" using 1:2 notitle with linespoints linetype 7 pointtype 7

## exceput LU
set size 1,1                   # set size 1,0.75
set origin 0,0                 # set origin 0,0.25
set xlabel '(# of nodes)x(# of workers per node)' # set xlabel ""
set ylabel 'speedup' # 'elapsed time (sec)'
set xtics ("1x4" 4, "2x4" 8, "4x4" 16, "8x4" 32, "16x4" 64) # set xtics 4 # set format x ""
# set bmargin 0

# see *.dat for yposition
set label "   0.534s (Fib)"       at 64,11.8 font "Times-Roman,30"
set label "   2.41s (Nqueens)"    at 64,30.8 font "Times-Roman,30"
set label "   0.772s (Pentomino)" at 64,17.0 font "Times-Roman,30"
set label "   0.580s (Comp)"      at 64,7.43 font "Times-Roman,30"
set label "   0.377s (Grav)"      at 64,13.2 font "Times-Roman,30"
set label "   36.1s (LU) "        at 64,0.24  font "Times-Roman,30"

set ytic 2
set yrange [0.21:50]
plot \
"fib44-4.dat"  using 1:2 title "Fib(44)"     with linespoints,\
"nq16-4.dat"   using 1:2 title "Nqueens(16)"      with linespoints,\
"pen13-4.dat"   using 1:2 title "Pentomino(13)"      with linespoints,\
"lu2000-4.dat" using 1:2 title "LU(2000)"  with linespoints,\
"comp30000-4.dat" using 1:2 title "Comp(30000)" with linespoints,\
"grav200-4.dat" using 1:2 title "Grav(200)" with linespoints
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
