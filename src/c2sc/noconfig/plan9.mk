# mkfile for PLAN9 PCC to mk MCPP
# 		2004/02	kamtsui

</$objtype/mkfile

CC=pcc
LD=pcc
PREPROCESSED=0
CFLAGS= -DPREPROCESSED=$PREPROCESSED -c -FVw

TARG=cpp_std

OFILES=main.$O\
	control.$O\
	eval.$O\
	expand.$O\
	support.$O\
	system.$O\
	mbchar.$O\
	lib.$O

HFILES=system.H\
	internal.H\
	noconfig.H

BIN=/$objtype/bin
</sys/src/cmd/mkone

preprocessed:V:
	cpp_std preproc.c cpp.H
	mk all

clean:V:
	rm -f *.$O $O.out cpp.H

