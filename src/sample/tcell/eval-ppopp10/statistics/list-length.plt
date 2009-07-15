#!/usr/bin/env gnuplot
set terminal tgif monochrome "Times-Roman" 30
set output 'list-length.obj' # 4x1--4x16 for small problem

# set multiplot
## common settings

set grid noxtics ytics
set key left top

set xrange [1:102]
set xtic 20
set yrange [0:80]
set ytic 20
# set logscale xy

# set bmargin 0
# set lmargin 6

set size 1,1                   # set size 1,0.75
set origin 0,0                 # set origin 0,0.25
set xlabel 'iterations'
set ylabel 'length' # 'elapsed time (sec)'
# set xtics ("1x4" 4, "2x4" 8, "4x4" 16, "8x4" 32, "16x4" 64) # set xtics 4 # set format x ""

# see *.dat for yposition
# set label "   0.534s (Fib)"       at 64,11.8 font "Times-Roman,30"

plot \
"length-2dtorus-1core.dat"  using 1:2 title "serial"     with linespoints pointtype 6,\
"length-2dtorus-8core.dat"  using 1:2 title "Tascell_membar 4cores" with linespoints pointtype 5,\
"length-2dtorus-8core.dat"  using 1:2 title "Tascell_membar 8cores" with linespoints pointtype 3

set nomultiplot
