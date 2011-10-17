########################################################################
# Makefile
#   created by Ken Matsui <kmatsui@kuis.kyoto-u.ac.jp>
# 
# This Tascell program is implemented based on a Barnes-Hut simulation
# program called treecode created by Joshua E. Barnes. To refer to the
# original program, please visit here:
#   http://www.ifa.hawaii.edu/~barnes/treecode/treeguide.html
########################################################################

EXENAME = treecode

########################################################################
# Compiler options for Tascell framework:

OS=${shell uname}
SC2C=sc2c
SC2C_OPT=
GCC=gcc
XCC=/home/lab4/hiraisi/bin/xcc
ICC=icc
OPT=-O2 -g $(MOPT)
XCCOPT=-DCT=lightweight
XCCCLOPT=-DCT=closure

ifeq ($(OS),SunOS)
    SC2C_OPT += -D USE_AFFINITY=PBIND
else
    SC2C_OPT += -D USE_AFFINITY=SCHED
endif

ifeq ($(OS),SunOS)
    THROPTS=-lthread
else
    THROPTS=-lpthread
endif
LIBOPTS=$(THROPTS) -lm
ifeq ($(OS),SunOS)
    LIBOPTS += -lsocket -lnsl
endif

ifeq ($(OS),SunOS)
    OPT += -DNO_VASPRINTF
endif

########################################################################
# Compiler options for treecode:

CCFLAGS = -DLINUX -g
LDFLAGS =
OPTFLAG = -O3


# Precision.  Possible values are SINGLEPREC, MIXEDPREC, and DOUBLEPREC.

PREC = SINGLEPREC


# Variations:

# Uncomment the next line to report calculation result
# FORCEREPORT = -D FORCEREPORT=1

# Uncomment the next line to build treecode for distributed memory environemnts
# BROADCAST = -D BROADCAST=1

# Uncomment the next line to use locks for tree construction
# USELOCK = -D USELOCK=1

# Uncomment the next line to use freq instead of dtime:
# USEFREQ = -DUSEFREQ

# Uncomment the next line to use binary I/O:
# BINARYIO = -DBINARYIO

# Uncomment the next line to make quick-scan treecode
# QUICKSCAN = -DQUICKSCAN

########################################################################

C_LIBS  = sendrecv.c sock.c
SC_NAME = worker cmd-serial treecode
SC_LIBS = worker.sc cmd-serial.sc worker.sh dprint.sh

TREECODE_TCELL = treecode.tcell treecomm.tcell treegrav-distributed.tcell \
                 treegrav-shared.tcell treeload.tcell

SC_C     = $(SC_GCC)
SC_GCC   = $(addsuffix -gcc.c,$(SC_NAME))
SC_LW    = $(addsuffix -lw.c,$(SC_NAME))
SC_CL    = $(addsuffix -cl.c,$(SC_NAME))
SC_XCC   = $(addsuffix -xcc.c,$(SC_NAME))
SC_XCCCL = $(addsuffix -xcccl.c,$(SC_NAME))

TREEOBJ = treecode.o treeio.o treeload.o treegrav.o libZeno.a

.PHONY: all treecode clean nestfunc.c


all: gcc lw xcc xcccl

gcc: sc_gcc treecode
	$(GCC) $(OPT) $(LIBOPTS) $(C_LIBS) $(SC_GCC) $(TREEOBJ) \
	 $(CCFLAGS) -D$(PREC) $(QUICKSCAN) $(USELOCK) $(BROADCAST) \
	 -o $(EXENAME)-gcc

lw: sc_lw treecode nestfunc.c
	$(GCC) $(OPT) $(LIBOPTS) $(C_LIBS) $(SC_LW) $(TREEOBJ) nestfunc.c \
	 $(CCFLAGS) -D$(PREC) $(QUICKSCAN) $(USELOCK) $(BROADCAST) \
	 -o $(EXENAME)-lw

xcc: sc_xcc treecode
	$(XCC) $(OPT) $(LIBOPTS) -fno-omit-frame-pointer $(C_LIBS) $(SC_XCC) \
	 $(TREEOBJ) $(CCFLAGS) -D$(PREC) $(QUICKSCAN) $(USELOCK) $(BROADCAST) \
	 -o $(EXENAME)-xcc

xcccl: sc_xcccl treecode
	$(XCC) $(OPT) $(LIBOPTS) $(C_LIBS) $(SC_XCC) \
	 $(TREEOBJ) $(CCFLAGS) -D$(PREC) $(QUICKSCAN) $(USELOCK) $(BROADCAST) \
	 -o $(EXENAME)-xcccl

treecode: $(TREEOBJ)


########################################################################

sc_gcc: $(SC_LIBS) $(TREECODE_TCELL)
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=GCC worker.sc
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=GCC cmd-serial.sc
	$(SC2C) $(SC2C_OPT) $(FORCEREPORT) \
	 $(BROADCAST) -D NF-TYPE=GCC $(USELOCK) treecode.tcell

sc_lw: $(SC_LIBS) $(TREECODE_TCELL)
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=LW-SC worker.sc
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=LW-SC cmd-serial.sc
	$(SC2C) $(SC2C_OPT) $(FORCEREPORT) \
	 $(BROADCAST) -D NF-TYPE=LW-SC $(USELOCK) treecode.tcell

sc_xcc: $(SC_LIBS) $(TREECODE_TCELL)
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=XCC worker.sc
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=XCC cmd-serial.sc
	$(SC2C) $(SC2C_OPT) $(FORCEREPORT) \
	 $(BROADCAST) -D NF-TYPE=XCC $(USELOCK) treecode.tcell

sc_xcccl: $(SC_LIBS) $(TREECODE_TCELL)
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=XCCCL worker.sc
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=XCCCL cmd-serial.sc
	$(SC2C) $(SC2C_OPT) $(FORCEREPORT) \
	 $(BROADCAST) -D NF-TYPE=XCCCL $(USELOCK) treecode.tcell

########################################################################
# Vanila treecode:

treecode.o: treecode.c treecode.h treedefs.h
	$(GCC) $(CCFLAGS) $(FORCEREPORT) -D$(PREC) \
	 $(QUICKSCAN) $(USEFREQ) $(USELOCK) $(BROADCAST) -c treecode.c

treeio.o: treeio.c treecode.h treedefs.h
	$(GCC) $(CCFLAGS) -D$(PREC) $(QUICKSCAN) $(USEFREQ) $(BINARYIO) \
	 $(USELOCK) $(BROADCAST) -c treeio.c

ifdef BROADCAST
  ENV_SUFFIX=distributed
else
  ENV_SUFFIX=shared
endif

treeload.o: treeload-$(ENV_SUFFIX).c treedefs.h
	$(GCC) $(CCFLAGS) -D$(PREC) $(QUICKSCAN) $(USELOCK) -O2 -c \
	 $(BROADCAST) -o treeload.o treeload-$(ENV_SUFFIX).c

treegrav.o: treegrav.c treedefs.h
	$(GCC) $(CCFLAGS) -D$(PREC) $(QUICKSCAN) $(OPTFLAG) $(USELOCK) \
	 $(BROADCAST) -o treegrav.o -c treegrav.c

########################################################################
# Zeno library:

libZeno.a: clib.o getparam.o mathfns.o
	ar ruv libZeno.a clib.o getparam.o mathfns.o

clib.o: clib.c stdinc.h
	$(GCC) $(CCFLAGS) -D$(PREC) -c clib.c

getparam.o: getparam.c stdinc.h getparam.h
	$(GCC) $(CCFLAGS) -D$(PREC) -c getparam.c

mathfns.o: mathfns.c stdinc.h mathfns.h
	$(GCC) $(CCFLAGS) -D$(PREC) -c mathfns.c

########################################################################

clean:
	rm -f *.o libZeno.a treecode-*.c worker-*.c cmd-serial-*.c $(EXENAME)-*

