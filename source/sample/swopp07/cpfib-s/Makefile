CC=/export/home/lab4/yasugi/tmp/sparc-sol2.6/n7/bin/gcc
OPT=-O2

all: cpfib-mtune.s cpfib-mcpu.s

cpfib-mtune.s: cpfib.c
	$(CC) cpfib.c -S -mtune=ultrasparc3 $(OPT) -o cpfib-mtune.s

cpfib-mcpu.s: cpfib.c
	$(CC) cpfib.c -S -mcpu=ultrasparc3 $(OPT) -o cpfib-mcpu.s
