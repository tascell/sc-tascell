#!/bin/zsh
# foeach i in lw clos; do
#   foreach j in 30 800; do
#     gmake SC2C_OPT="-D CALL-BOUND=$j -D SEARCH-P-MEM-BARRIER-1=1" -B spanning-$i.c \
#         && gmake MOPT=-mcpu=niagara2 spanning-$i
#         && cp spanning-$i spanning-$i-cas-$j
#     gmake SC2C_OPT="-D CALL-BOUND=$j -D SEARCH-P-MEM-BARRIER-3=1" -B spanning-$i.c \
#         && gmake MOPT=-mcpu=niagara2 spanning-$i
#         && cp spanning-$i spanning-$i-membar-$j
#   done
# done

foreach i in xcccl; do # xcc xcccl
  foreach j in 30 800; do
    gmake SC2C_OPT="-D CALL-BOUND=$j -D SEARCH-P-MEM-BARRIER-1=1" -B spanning-$i.c \
        && gmake MOPT=-mcpu=ultrasparc spanning-$i \
        && cp spanning-$i spanning-$i-cas-$j
    gmake SC2C_OPT="-D CALL-BOUND=$j -D SEARCH-P-MEM-BARRIER-3=1" -B spanning-$i.c \
        && gmake MOPT=-mcpu=ultrasparc spanning-$i \
        && cp spanning-$i spanning-$i-membar-$j
  done
done

# for statistics (i is not important)
foreach i in xcccl; do
  foreach j in 30; do
  gmake SC2C_OPT="-D CALL-BOUND=$j -D SEARCH-P-MEM-BARRIER-3=1 -D STATISTICS=1" -B spanning-$i.c \
      && gmake MOPT=-mcpu=ultrasparc spanning-$i \
      && cp spanning-$i spanning-$i-membar-$j-statistics
  done
done
