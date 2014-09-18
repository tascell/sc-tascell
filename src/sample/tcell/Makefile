OS=${shell uname}
SC2C=sc2c
SC2C_OPT=
XCC=xcc
GCC=gcc #$(XCC)
ICC=icc
OPT=-O3 -g -msse4.2$(MOPT)
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


APPS=fib fib-guard fib-excep lu cmp pen pen6x17 pen6x17-guard nq grav matmul spanning bcast copine0 copine1 copine2 copine3 copine4

C_PROGS=$(addsuffix -c,$(APPS))
GCC_PROGS=$(addsuffix -gcc,$(APPS))
GCCLW_PROGS=$(addsuffix -lw,$(APPS))
GCCCLOS_PROGS=$(addsuffix -clos,$(APPS))
ALL_GCC_PROGS=$(C_PROGS) $(GCC_PROGS) $(GCCLW_PROGS) $(GCCCLOS_PROGS)

XCCLW_PROGS=$(addsuffix -xcc,$(APPS))
XCCCL_PROGS=$(addsuffix -xcccl,$(APPS))
ALL_XCC_PROGS=$(XCCLW_PROGS) $(XCCCL_PROGS)

ICC_C_PROGS=$(addsuffix -icc-c,$(APPS))
ICC_LW_PROGS=$(addsuffix -icc-lw,$(APPS))
ICC_CLOS_PROGS=$(addsuffix -icc-clos,$(APPS))
ALL_ICC_PROGS=$(ICC_C_PROGS) $(ICC_LW_PROGS) $(ICC_CLOS_PROGS)

ALL_PROGS=$(ALL_GCC_PROGS) $(ALL_XCC_PROGS) $(ALL_ICC_PROGS)

all: $(ALL_PROGS)

all-gcc-c: $(C_PROGS)
all-gcc-nf: $(GCC_PROGS)
all-gcc-lw: $(GCCLW_PROGS)
all-gcc-clos: $(GCCCLOS_PROGS)
all-gcc: $(ALL_GCC_PROGS)

all-xcclw: $(XCCLW_PROGS)
all-xcccl: $(XCCCL_PROGS)
all-xcc: $(ALL_XCC_PROGS)

all-icc-c: $(ICC_C_PROGS)
all-icc-lw: $(ICC_LW_PROGS)
all-icc-clos: $(ICC_CLOS_PROGS)
all-icc: $(ALL_ICC_PROGS)

.PHONY: all-gcc-c all-gcc-nf all-gcc-lw all-gcc-clos all-gcc all-xcclw all-xcccl all-xcc all-icc-c all-icc-lw all-cc-clos all-icc all clean


LIBS=sendrecv.c sock.c
WORKER_LIBS=worker cmd-serial
#WORKER_C=$(WORKER_GCC)
WORKER_GCC=$(addsuffix -gcc.c,$(WORKER_LIBS))
WORKER_LW=$(addsuffix -lw.c,$(WORKER_LIBS))
WORKER_CLOS=$(addsuffix -clos.c,$(WORKER_LIBS))
WORKER_XCC=$(addsuffix -xcc.c,$(WORKER_LIBS))
WORKER_XCCCL=$(addsuffix -xcccl.c,$(WORKER_LIBS))

# RAWC version (can't divide task; only for evaluation)
$(addsuffix .c,$(C_PROGS)): %-c.c: %.tcell
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=RAWC $<
$(C_PROGS): %: $(WORKER_GCC) $(LIBS) %.c
	$(GCC) $(OPT) $+ $(LIBOPTS) -o $@
# ICC: RAWC version (can't divide task; only for evaluation)
$(ICC_C_PROGS): %: $(WORKER_GCC) $(LIBS) %.c
	$(ICC) $(OPT) $+ $(LIBOPTS) -o $@

# GCC nested function
$(WORKER_GCC): %-gcc.c: %.sc worker.sh dprint.sh
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=GCC $<
$(addsuffix .c,$(GCC_PROGS)): %-gcc.c: %.tcell
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=GCC $<
$(GCC_PROGS): %: $(WORKER_GCC) $(LIBS) %.c
	$(GCC) $(OPT) $+ $(LIBOPTS) -o $@

# LW-SC
$(WORKER_LW): %-lw.c: %.sc worker.sh dprint.sh
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=LW-SC $<
$(addsuffix .c,$(GCCLW_PROGS)): %-lw.c: %.tcell
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=LW-SC $<
$(GCCLW_PROGS): %: $(WORKER_LW) $(LIBS) nestfunc.c %.c
	$(GCC) $(OPT) $+ $(LIBOPTS) -o $@
# ICC: LW-SC
$(ICC_LW_PROGS): %: $(WORKER_LW) $(LIBS) nestfunc.c %.c
	$(ICC) $(OPT) $+ $(LIBOPTS) -o $@

# CL-SC
$(WORKER_CLOS): %-clos.c: %.sc worker.sh dprint.sh
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=CL-SC $<
$(addsuffix .c,$(GCCCLOS_PROGS)): %-clos.c: %.tcell
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=CL-SC $<
$(GCCCLOS_PROGS): %: $(WORKER_CLOS) $(LIBS) nestfunc.c %.c
	$(GCC) $(OPT) $+ $(LIBOPTS) -o $@
# ICC: CL-SC
$(ICC_CLOS_PROGS): %: $(WORKER_CLOS) $(LIBS) nestfunc.c %.c
	$(ICC) $(OPT) $+ $(LIBOPTS) -o $@

# XCC nested function (lightweight)
$(WORKER_XCC): %-xcc.c: %.sc worker.sh dprint.sh
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=XCC $<
$(addsuffix .c,$(XCCLW_PROGS)): %-xcc.c: %.tcell
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=XCC $<
$(XCCLW_PROGS): %: $(WORKER_XCC) $(LIBS) %.c
	$(XCC) $(OPT) $+ $(LIBOPTS) -fno-omit-frame-pointer -o $@

# XCC nested function (closure)
$(WORKER_XCCCL): %-xcccl.c: %.sc worker.sh dprint.sh
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=XCCCL $<
$(addsuffix .c,$(XCCCL_PROGS)): %-xcccl.c: %.tcell
	$(SC2C) $(SC2C_OPT) -D NF-TYPE=XCCCL $<
$(XCCCL_PROGS): %: $(WORKER_XCCCL) $(LIBS) %.c
	$(XCC) $(OPT) $+ $(LIBOPTS) -o $@



# demo
# pen-demo: $(WORKER_GCC) pen-gcc-gtk.c $(LIBS)
#	$(GCC) $(OPT)  $(THROPT) $(WORKER_GCC) pen-gcc-gtk.c $(LIBS) -o pen-demo `pkg-config --cflags --libs gtk+-2.0`

# clean:
# 	rm -f $(PROGS)
