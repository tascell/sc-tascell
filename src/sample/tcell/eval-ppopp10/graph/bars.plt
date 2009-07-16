#!/usr/bin/env gnuplot
set term postscript eps enhanced color
# set terminal tgif color "Times-Roman" 30 # monochrome
set output 'bars.eps' # 4x1--4x16 for small problem

# set linestyle 1 lt 1 lw 50
# for gnuplot ver.4
# set style line 1 lt 1 lw 50

set xrange [30:240]
# 40*graph_no+15
set xtics ("Random" 55, "Hypercube(20)" 95, "2D-torus(2000)" 134, "Bintree(24)" 175, "Hypercube(21)" 215)
set yrange [0:1.2]
set ytics ("0.2" 0.2, "0.4" 0.4, "0.6" 0.6, "0.8" 0.8, "1.0" 1.0)
set y2tics ("" 1.0)
set grid noxtics noytics y2tics

set ylabel "speedup"

set key below

set style fill pattern border

plot \
"serial-bar.dat" usi ($1*40):2:(3) title "serial" w boxes lt 2 fs solid 1,\
"serial_call-bar.dat" usi ($1*40+3):2:(3) title "serial\\_call" w boxes lt 2 fs solid 0.5,\
"serial_call_cas-bar.dat" usi ($1*40+6):2:(3) title "serial\\_call\\_cas" w boxes lt 2 fs pattern 4,\
"serial_call_membar-bar.dat" usi ($1*40+9):2:(3) title "serial\\_call\\_membar" w boxes lt 2 fs pattern 2,\
"Cilk_cas-bar.dat" usi ($1*40+12):2:(3) title "Cilk\\_cas" w boxes lt 1 fs pattern 4,\
"Cilk_membar-bar.dat" usi ($1*40+15):2:(3) title "Cilk\\_membar" w boxes lt 1 fs pattern 2,\
"Cilk_R_cas-bar.dat" usi ($1*40+18):2:(3) title "Cilk\\_R\\_cas" w boxes lt 3 fs pattern 4,\
"Cilk_R_membar-bar.dat" usi ($1*40+21):2:(3) title "Cilk\\_R\\_membar" w boxes lt 3 fs pattern 2,\
"Tascell_cas-bar.dat" usi ($1*40+24):2:(3) title "Tascell\\_cas" w boxes lt 4 fs pattern 4,\
"Tascell_membar-bar.dat" usi ($1*40+27):2:(3) title "Tascell\\_membar" w boxes lt 4 fs pattern 2
