#!/bin/sh
for g in *.plt; do
    gnuplot $g
done
