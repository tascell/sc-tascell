#!/bin/sh

gnuplot bar.plt

for b in ("" "_800"); do
    for g in (Random Hypercube21 Bintree 2D-torus) ; do
        sed "s/%1%/$g/g" oresen.plt.template | \
            sed "s/%2%/$b/g" | gnuplot
    done
done

for obj in (*.obj) ; do
    tgif $obj
done
