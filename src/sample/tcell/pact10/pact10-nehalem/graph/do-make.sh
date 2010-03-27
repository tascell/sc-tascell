#!/bin/sh

# gnuplot bars.plt

for b in "" ; do # "" "_800"
    sed "s/%2%/$b/g" bars.plt.template | gnuplot
    for g in "Random" "Hypercube21" "Bintree" "2D-torus-4000" ; do
        if [ 1 = 1 -o $g = "2D-torus-4000" ]; then
            sed "s/%1%/$g/g" oresen.plt.template | \
                sed "s/%2%/$b/g" | \
                sed "s/%3%/left top/g" | gnuplot
        else
            sed "s/%1%/$g/g" oresen.plt.template | \
                sed "s/%2%/$b/g" | \
                sed "s/%3%/right bottom/g" | gnuplot
        fi
    done
done

#for obj in *.obj ; do
#     tgif $obj
#done
