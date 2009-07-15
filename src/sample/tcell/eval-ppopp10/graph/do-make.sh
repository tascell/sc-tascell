#!/bin/sh
gnuplot *.plt && tgif -print -eps *.obj
