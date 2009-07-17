#!/bin/sh

gnuplot bars.plt

for b in "" "_800" ; do
    sed "s/%2%/$b/g" bars.plt.template | gnuplot
    for g in "Random" "Hypercube21" "Bintree" "2D-torus" ; do
        sed "s/%1%/$g/g" oresen.plt.template | \
            sed "s/%2%/$b/g" | gnuplot
    done
done

for obj in *.obj ; do
     tgif $obj
done
