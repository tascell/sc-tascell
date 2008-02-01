XCC=/tmp/local/bin/gcc
GCC=gcc #$(XCC)
ICC=icc
OPT=-O2 -w $(MOPT)
XCCOPT=-DCT=lightweight
XCCCLOPT=-DCT=closure
THROPT=-lpthread

PROGS_GCC=\
 fib-c lu-c cmp-c pen-c pen6x17-c nq-c\
 fib-gcc lu-gcc cmp-gcc pen-gcc pen6x17-gcc nq-gcc\
 fib-lw lu-lw cmp-lw pen-lw pen6x17-lw nq-lw\
 fib-clos lu-clos cmp-clos pen-clos  pen6x17-clos nq-clos\
 fib-xcc lu-xcc cmp-xcc pen-xcc  pen6x17-xcc nq-xcc\
 fib-xcccl lu-xcccl cmp-xcccl pen-xcccl pen6x17-xcccl nq-xcccl

PROGS_ICC=\
 fib-icc-c lu-icc-c cmp-icc-c pen-icc-c pen6x17-icc-c nq-icc-c\
 fib-icc-lw lu-icc-lw cmp-icc-lw pen-icc-lw pen6x17-icc-lw nq-icc-lw\
 fib-icc-clos lu-icc-clos cmp-icc-clos pen-icc-clos pen6x17-icc-clos nq-icc-clos

PROGS=$(PROGS_GCC) $(PROGS_ICC)

WORKER=worker2.c
WORKER_XCLW=worker2-lw.c
WORKER_XCCL=worker2-clos.c

all: $(PROGS)
all-gcc: $(PROGS_GCC)
all-icc: $(PROGS_ICC)

# RAWC version (can't divide task)
fib-c: $(WORKER) fib-c.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER) fib-c.c sendrecv.c -o fib-c
lu-c: $(WORKER) lu-c.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER) lu-c.c sendrecv.c -o lu-c
cmp-c: $(WORKER) cmp-c.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER) cmp-c.c sendrecv.c -o cmp-c
pen-c: $(WORKER) pen-c.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER) pen-c.c sendrecv.c -o pen-c
pen6x17-c: $(WORKER) pen6x17-c.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER) pen6x17-c.c sendrecv.c -o pen6x17-c
nq-c: $(WORKER) nq-c.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER) nq-c.c sendrecv.c -o nq-c

# ICC: RAWC version (can't divide task)
fib-icc-c: $(WORKER) fib-c.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER) fib-c.c sendrecv.c -o fib-icc-c
lu-icc-c: $(WORKER) lu-c.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER) lu-c.c sendrecv.c -o lu-icc-c
cmp-icc-c: $(WORKER) cmp-c.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER) cmp-c.c sendrecv.c -o cmp-icc-c
pen-icc-c: $(WORKER) pen-c.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER) pen-c.c sendrecv.c -o pen-icc-c
pen6x17-icc-c: $(WORKER) pen6x17-c.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER) pen6x17-c.c sendrecv.c -o pen6x17-icc-c
nq-icc-c: $(WORKER) nq-c.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER) nq-c.c sendrecv.c -o nq-icc-c

# GCC nested function
fib-gcc: $(WORKER) fib-gcc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER) fib-gcc.c sendrecv.c -o fib-gcc	
lu-gcc: $(WORKER) lu-gcc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER) lu-gcc.c sendrecv.c -o lu-gcc
cmp-gcc: $(WORKER) cmp-gcc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER) cmp-gcc.c sendrecv.c -o cmp-gcc
pen-gcc: $(WORKER) pen-gcc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER) pen-gcc.c sendrecv.c -o pen-gcc
pen6x17-gcc: $(WORKER) pen6x17-gcc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER) pen6x17-gcc.c sendrecv.c -o pen6x17-gcc
nq-gcc: $(WORKER) nq-gcc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER) nq-gcc.c sendrecv.c -o nq-gcc

# LW-SC
fib-lw: $(WORKER_XCLW) fib-lw.c nestfunc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCLW) fib-lw.c nestfunc.c sendrecv.c -o fib-lw
lu-lw: $(WORKER_XCLW) lu-lw.c nestfunc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCLW) lu-lw.c nestfunc.c sendrecv.c -o lu-lw
cmp-lw: $(WORKER_XCLW) cmp-lw.c nestfunc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCLW) cmp-lw.c nestfunc.c sendrecv.c -o cmp-lw
pen-lw: $(WORKER_XCLW) pen-lw.c nestfunc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCLW) pen-lw.c nestfunc.c sendrecv.c -o pen-lw
pen6x17-lw: $(WORKER_XCLW) pen6x17-lw.c nestfunc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCLW) pen6x17-lw.c nestfunc.c sendrecv.c -o pen6x17-lw
nq-lw: $(WORKER_XCLW) nq-lw.c nestfunc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCLW) nq-lw.c nestfunc.c sendrecv.c -o nq-lw

# ICC: LW-SC
fib-icc-lw: $(WORKER_XCLW) fib-lw.c nestfunc.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCLW) fib-lw.c nestfunc.c sendrecv.c -o fib-icc-lw
lu-icc-lw: $(WORKER_XCLW) lu-lw.c nestfunc.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCLW) lu-lw.c nestfunc.c sendrecv.c -o lu-icc-lw
cmp-icc-lw: $(WORKER_XCLW) cmp-lw.c nestfunc.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCLW) cmp-lw.c nestfunc.c sendrecv.c -o cmp-icc-lw
pen-icc-lw: $(WORKER_XCLW) pen-lw.c nestfunc.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCLW) pen-lw.c nestfunc.c sendrecv.c -o pen-icc-lw
pen6x17-icc-lw: $(WORKER_XCLW) pen6x17-lw.c nestfunc.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCLW) pen6x17-lw.c nestfunc.c sendrecv.c -o pen6x17-icc-lw
nq-icc-lw: $(WORKER_XCLW) nq-lw.c nestfunc.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCLW) nq-lw.c nestfunc.c sendrecv.c -o nq-icc-lw

# CL-SC
fib-clos: $(WORKER_XCCL) fib-clos.c nestfunc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCCL) fib-clos.c nestfunc.c sendrecv.c -o fib-clos
lu-clos: $(WORKER_XCCL) lu-clos.c nestfunc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCCL) lu-clos.c nestfunc.c sendrecv.c -o lu-clos
cmp-clos: $(WORKER_XCCL) cmp-clos.c nestfunc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCCL) cmp-clos.c nestfunc.c sendrecv.c -o cmp-clos
pen-clos: $(WORKER_XCCL) pen-clos.c nestfunc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCCL) pen-clos.c nestfunc.c sendrecv.c -o pen-clos
pen6x17-clos: $(WORKER_XCCL) pen6x17-clos.c nestfunc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCCL) pen6x17-clos.c nestfunc.c sendrecv.c -o pen6x17-clos
nq-clos: $(WORKER_XCCL) nq-clos.c nestfunc.c sendrecv.c
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCCL) nq-clos.c nestfunc.c sendrecv.c -o nq-clos

# ICC: CL-SC
fib-icc-clos: $(WORKER_XCCL) fib-clos.c nestfunc.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCCL) fib-clos.c nestfunc.c sendrecv.c -o fib-icc-clos
lu-icc-clos: $(WORKER_XCCL) lu-clos.c nestfunc.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCCL) lu-clos.c nestfunc.c sendrecv.c -o lu-icc-clos
cmp-icc-clos: $(WORKER_XCCL) cmp-clos.c nestfunc.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCCL) cmp-clos.c nestfunc.c sendrecv.c -o cmp-icc-clos
pen-icc-clos: $(WORKER_XCCL) pen-clos.c nestfunc.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCCL) pen-clos.c nestfunc.c sendrecv.c -o pen-icc-clos
pen6x17-icc-clos: $(WORKER_XCCL) pen6x17-clos.c nestfunc.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCCL) pen6x17-clos.c nestfunc.c sendrecv.c -o pen6x17-icc-clos
nq-icc-clos: $(WORKER_XCCL) nq-clos.c nestfunc.c sendrecv.c
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCCL) nq-clos.c nestfunc.c sendrecv.c -o nq-icc-clos

# XCC nested function (lightweight)
fib-xcc: $(WORKER) fib-xcc.c sendrecv.c
	$(XCC) $(OPT) $(XCCOPT) $(THROPT) $(WORKER) fib-xcc.c sendrecv.c -o fib-xcc
lu-xcc: $(WORKER) lu-xcc.c sendrecv.c
	$(XCC) $(OPT) $(XCCOPT) $(THROPT) $(WORKER) lu-xcc.c sendrecv.c -o lu-xcc
cmp-xcc: $(WORKER) cmp-xcc.c sendrecv.c
	$(XCC) $(OPT) $(XCCOPT) $(THROPT) $(WORKER) cmp-xcc.c sendrecv.c -o cmp-xcc
pen-xcc: $(WORKER) pen-xcc.c sendrecv.c
	$(XCC) $(OPT) $(XCCOPT) $(THROPT) $(WORKER) pen-xcc.c sendrecv.c -o pen-xcc
pen6x17-xcc: $(WORKER) pen6x17-xcc.c sendrecv.c
	$(XCC) $(OPT) $(XCCOPT) $(THROPT) $(WORKER) pen6x17-xcc.c sendrecv.c -o pen6x17-xcc
nq-xcc: $(WORKER) nq-xcc.c sendrecv.c
	$(XCC) $(OPT) $(XCCOPT) $(THROPT) $(WORKER) nq-xcc.c sendrecv.c -o nq-xcc

# XCC nested function (closure)
fib-xcccl: $(WORKER) fib-xcccl.c sendrecv.c
	$(XCC) $(OPT) $(XCCCLOPT) $(THROPT) $(WORKER) fib-xcccl.c sendrecv.c -o fib-xcccl
lu-xcccl: $(WORKER) lu-xcccl.c sendrecv.c
	$(XCC) $(OPT) $(XCCCLOPT) $(THROPT) $(WORKER) lu-xcccl.c sendrecv.c -o lu-xcccl
cmp-xcccl: $(WORKER) cmp-xcccl.c sendrecv.c
	$(XCC) $(OPT) $(XCCCLOPT) $(THROPT) $(WORKER) cmp-xcccl.c sendrecv.c -o cmp-xcccl
pen-xcccl: $(WORKER) pen-xcccl.c sendrecv.c
	$(XCC) $(OPT) $(XCCCLOPT) $(THROPT) $(WORKER) pen-xcccl.c sendrecv.c -o pen-xcccl
pen6x17-xcccl: $(WORKER) pen6x17-xcccl.c sendrecv.c
	$(XCC) $(OPT) $(XCCCLOPT) $(THROPT) $(WORKER) pen6x17-xcccl.c sendrecv.c -o pen6x17-xcccl
nq-xcccl: $(WORKER) nq-xcccl.c sendrecv.c
	$(XCC) $(OPT) $(XCCCLOPT) $(THROPT) $(WORKER) nq-xcccl.c sendrecv.c -o nq-xcccl


# もとのC Version
009b: 009b.c ../../obayashi/fib.c
	$(GCC) $(OPT) $(THROPT) 009b.c ../../obayashi/fib.c -o 009b

# workerだけSCで書き直し
fib-orig: worker.c ../../obayashi/fib.c
	$(GCC) $(OPT) $(THROPT) worker.c ../../obayashi/fib.c -o fib-orig

clean:
	rm -f $(PROGS)
