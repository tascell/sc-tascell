#!/usr/bin/env gnuplot
set term postscript eps enhanced color

# set linestyle 1 lt 1 lw 50
# for gnuplot ver.4
# set style line 1 lt 1 lw 50

plot "serial-bar.dat" usi 1:2:(3)      w boxes fs pattern 1,\
"serial-bar.dat" usi 1:2:(3)      w boxes fs pattern 1,\
"serial-bar.dat" usi 1:2:(3)      w boxes fs pattern 1,\
"serial-bar.dat" usi 1:2:(3)      w boxes fs pattern 1,\
"serial-bar.dat" usi 1:2:(3)      w boxes fs pattern 1,\
"serial-bar.dat" usi 1:2:(3)      w boxes fs pattern 1,\
"serial-bar.dat" usi 1:2:(3)      w boxes fs pattern 1,\
"serial-bar.dat" usi 1:2:(3)      w boxes fs pattern 1,\
"serial-bar.dat" usi 1:2:(3)      w boxes fs pattern 1,\
"serial-bar.dat" usi 1:2:(3)      w boxes fs pattern 1,\

