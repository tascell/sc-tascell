XCC=/tmp/local/bin/gcc
GCC=gcc #$(XCC)
ICC=icc
OPT=-O2 $(MOPT)
XCCOPT=-DCT=lightweight
XCCCLOPT=-DCT=closure
THROPT=-lpthread
LIBS=sendrecv.c sock.c -lm

all: all-gcc all-icc

all-gcc: all-gcc-c all-gcc-nf all-gcc-lw all-gcc-clos
all-xcc: all-xcclw all-xcccl
all-icc: all-icc-c all-icc-lw all-icc-lw

all-gcc-c: fib-c lu-c cmp-c pen-c pen6x17-c nq-c grav-c
all-gcc-nf: fib-gcc lu-gcc cmp-gcc pen-gcc pen6x17-gcc nq-gcc grav-gcc
all-gcc-lw: fib-lw lu-lw cmp-lw pen-lw pen6x17-lw nq-lw grav-lw
all-gcc-clos: fib-clos lu-clos cmp-clos pen-clos  pen6x17-clos nq-clos grav-clos

all-xcclw: fib-xcc lu-xcc cmp-xcc pen-xcc  pen6x17-xcc nq-xcc grav-xcc
all-xcccl: fib-xcccl lu-xcccl cmp-xcccl pen-xcccl pen6x17-xcccl nq-xcccl grav-xccl

all-icc-c: fib-icc-c lu-icc-c cmp-icc-c pen-icc-c pen6x17-icc-c nq-icc-c grav-icc-c
all-icc-lw: fib-icc-lw lu-icc-lw cmp-icc-lw pen-icc-lw pen6x17-icc-lw nq-icc-lw grav-icc-lw
all-icc-clos: fib-icc-clos lu-icc-clos cmp-icc-clos pen-icc-clos pen6x17-icc-clos nq-icc-clos grav-icc-clos


WORKER=worker-gcc.c cmd-serial-gcc.c
WORKER_GCC=worker-gcc.c cmd-serial-gcc.c
WORKER_XCLW=worker-lw.c cmd-serial-lw.c
WORKER_XCCL=worker-clos.c

# RAWC version (can't divide task)
fib-c: $(WORKER) fib-c.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER) fib-c.c $(LIBS) -o fib-c
lu-c: $(WORKER) lu-c.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER) lu-c.c $(LIBS) -o lu-c
cmp-c: $(WORKER) cmp-c.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER) cmp-c.c $(LIBS) -o cmp-c
pen-c: $(WORKER) pen-c.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER) pen-c.c $(LIBS) -o pen-c
pen6x17-c: $(WORKER) pen6x17-c.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER) pen6x17-c.c $(LIBS) -o pen6x17-c
nq-c: $(WORKER) nq-c.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER) nq-c.c $(LIBS) -o nq-c
grav-c: $(WORKER) grav-c.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER) grav-c.c $(LIBS) -o grav-c

# ICC: RAWC version (can't divide task)
fib-icc-c: $(WORKER) fib-c.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER) fib-c.c $(LIBS) -o fib-icc-c
lu-icc-c: $(WORKER) lu-c.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER) lu-c.c $(LIBS) -o lu-icc-c
cmp-icc-c: $(WORKER) cmp-c.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER) cmp-c.c $(LIBS) -o cmp-icc-c
pen-icc-c: $(WORKER) pen-c.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER) pen-c.c $(LIBS) -o pen-icc-c
pen6x17-icc-c: $(WORKER) pen6x17-c.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER) pen6x17-c.c $(LIBS) -o pen6x17-icc-c
nq-icc-c: $(WORKER) nq-c.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER) nq-c.c $(LIBS) -o nq-icc-c
grav-icc-c: $(WORKER) grav-c.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER) grav-c.c $(LIBS) -o grav-icc-c

# GCC nested function
fib-gcc: $(WORKER_GCC) fib-gcc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_GCC) fib-gcc.c $(LIBS) -o fib-gcc	
lu-gcc: $(WORKER_GCC) lu-gcc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_GCC) lu-gcc.c $(LIBS) -o lu-gcc
cmp-gcc: $(WORKER_GCC) cmp-gcc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_GCC) cmp-gcc.c $(LIBS) -o cmp-gcc
pen-gcc: $(WORKER_GCC) pen-gcc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_GCC) pen-gcc.c $(LIBS) -o pen-gcc
pen6x17-gcc: $(WORKER_GCC) pen6x17-gcc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_GCC) pen6x17-gcc.c $(LIBS) -o pen6x17-gcc
nq-gcc: $(WORKER_GCC) nq-gcc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_GCC) nq-gcc.c $(LIBS) -o nq-gcc
grav-gcc: $(WORKER_GCC) grav-gcc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_GCC) grav-gcc.c $(LIBS) -o grav-gcc

# LW-SC
fib-lw: $(WORKER_XCLW) fib-lw.c nestfunc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCLW) fib-lw.c nestfunc.c $(LIBS) -o fib-lw
lu-lw: $(WORKER_XCLW) lu-lw.c nestfunc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCLW) lu-lw.c nestfunc.c $(LIBS) -o lu-lw
cmp-lw: $(WORKER_XCLW) cmp-lw.c nestfunc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCLW) cmp-lw.c nestfunc.c $(LIBS) -o cmp-lw
pen-lw: $(WORKER_XCLW) pen-lw.c nestfunc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCLW) pen-lw.c nestfunc.c $(LIBS) -o pen-lw
pen6x17-lw: $(WORKER_XCLW) pen6x17-lw.c nestfunc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCLW) pen6x17-lw.c nestfunc.c $(LIBS) -o pen6x17-lw
nq-lw: $(WORKER_XCLW) nq-lw.c nestfunc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCLW) nq-lw.c nestfunc.c $(LIBS) -o nq-lw
grav-lw: $(WORKER_XCLW) grav-lw.c nestfunc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCLW) grav-lw.c nestfunc.c $(LIBS) -o grav-lw

# ICC: LW-SC
fib-icc-lw: $(WORKER_XCLW) fib-lw.c nestfunc.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCLW) fib-lw.c nestfunc.c $(LIBS) -o fib-icc-lw
lu-icc-lw: $(WORKER_XCLW) lu-lw.c nestfunc.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCLW) lu-lw.c nestfunc.c $(LIBS) -o lu-icc-lw
cmp-icc-lw: $(WORKER_XCLW) cmp-lw.c nestfunc.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCLW) cmp-lw.c nestfunc.c $(LIBS) -o cmp-icc-lw
pen-icc-lw: $(WORKER_XCLW) pen-lw.c nestfunc.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCLW) pen-lw.c nestfunc.c $(LIBS) -o pen-icc-lw
pen6x17-icc-lw: $(WORKER_XCLW) pen6x17-lw.c nestfunc.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCLW) pen6x17-lw.c nestfunc.c $(LIBS) -o pen6x17-icc-lw
nq-icc-lw: $(WORKER_XCLW) nq-lw.c nestfunc.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCLW) nq-lw.c nestfunc.c $(LIBS) -o nq-icc-lw
grav-icc-lw: $(WORKER_XCLW) grav-lw.c nestfunc.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCLW) grav-lw.c nestfunc.c $(LIBS) -o grav-icc-lw

# CL-SC
fib-clos: $(WORKER_XCCL) fib-clos.c nestfunc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCCL) fib-clos.c nestfunc.c $(LIBS) -o fib-clos
lu-clos: $(WORKER_XCCL) lu-clos.c nestfunc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCCL) lu-clos.c nestfunc.c $(LIBS) -o lu-clos
cmp-clos: $(WORKER_XCCL) cmp-clos.c nestfunc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCCL) cmp-clos.c nestfunc.c $(LIBS) -o cmp-clos
pen-clos: $(WORKER_XCCL) pen-clos.c nestfunc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCCL) pen-clos.c nestfunc.c $(LIBS) -o pen-clos
pen6x17-clos: $(WORKER_XCCL) pen6x17-clos.c nestfunc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCCL) pen6x17-clos.c nestfunc.c $(LIBS) -o pen6x17-clos
nq-clos: $(WORKER_XCCL) nq-clos.c nestfunc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCCL) nq-clos.c nestfunc.c $(LIBS) -o nq-clos
grav-clos: $(WORKER_XCCL) grav-clos.c nestfunc.c $(LIBS)
	$(GCC) $(OPT)  $(THROPT) $(WORKER_XCCL) grav-clos.c nestfunc.c $(LIBS) -o grav-clos

# ICC: CL-SC
fib-icc-clos: $(WORKER_XCCL) fib-clos.c nestfunc.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCCL) fib-clos.c nestfunc.c $(LIBS) -o fib-icc-clos
lu-icc-clos: $(WORKER_XCCL) lu-clos.c nestfunc.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCCL) lu-clos.c nestfunc.c $(LIBS) -o lu-icc-clos
cmp-icc-clos: $(WORKER_XCCL) cmp-clos.c nestfunc.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCCL) cmp-clos.c nestfunc.c $(LIBS) -o cmp-icc-clos
pen-icc-clos: $(WORKER_XCCL) pen-clos.c nestfunc.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCCL) pen-clos.c nestfunc.c $(LIBS) -o pen-icc-clos
pen6x17-icc-clos: $(WORKER_XCCL) pen6x17-clos.c nestfunc.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCCL) pen6x17-clos.c nestfunc.c $(LIBS) -o pen6x17-icc-clos
nq-icc-clos: $(WORKER_XCCL) nq-clos.c nestfunc.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCCL) nq-clos.c nestfunc.c $(LIBS) -o nq-icc-clos
grav-icc-clos: $(WORKER_XCCL) grav-clos.c nestfunc.c $(LIBS)
	$(ICC) $(OPT)  $(THROPT) $(WORKER_XCCL) grav-clos.c nestfunc.c $(LIBS) -o grav-icc-clos

# XCC nested function (lightweight)
fib-xcc: $(WORKER) fib-xcc.c $(LIBS)
	$(XCC) $(OPT) $(XCCOPT) $(THROPT) $(WORKER) fib-xcc.c $(LIBS) -o fib-xcc
lu-xcc: $(WORKER) lu-xcc.c $(LIBS)
	$(XCC) $(OPT) $(XCCOPT) $(THROPT) $(WORKER) lu-xcc.c $(LIBS) -o lu-xcc
cmp-xcc: $(WORKER) cmp-xcc.c $(LIBS)
	$(XCC) $(OPT) $(XCCOPT) $(THROPT) $(WORKER) cmp-xcc.c $(LIBS) -o cmp-xcc
pen-xcc: $(WORKER) pen-xcc.c $(LIBS)
	$(XCC) $(OPT) $(XCCOPT) $(THROPT) $(WORKER) pen-xcc.c $(LIBS) -o pen-xcc
pen6x17-xcc: $(WORKER) pen6x17-xcc.c $(LIBS)
	$(XCC) $(OPT) $(XCCOPT) $(THROPT) $(WORKER) pen6x17-xcc.c $(LIBS) -o pen6x17-xcc
nq-xcc: $(WORKER) nq-xcc.c $(LIBS)
	$(XCC) $(OPT) $(XCCOPT) $(THROPT) $(WORKER) nq-xcc.c $(LIBS) -o nq-xcc
grav-xcc: $(WORKER) grav-xcc.c $(LIBS)
	$(XCC) $(OPT) $(XCCOPT) $(THROPT) $(WORKER) grav-xcc.c $(LIBS) -o grav-xcc

# XCC nested function (closure)
fib-xcccl: $(WORKER) fib-xcccl.c $(LIBS)
	$(XCC) $(OPT) $(XCCCLOPT) $(THROPT) $(WORKER) fib-xcccl.c $(LIBS) -o fib-xcccl
lu-xcccl: $(WORKER) lu-xcccl.c $(LIBS)
	$(XCC) $(OPT) $(XCCCLOPT) $(THROPT) $(WORKER) lu-xcccl.c $(LIBS) -o lu-xcccl
cmp-xcccl: $(WORKER) cmp-xcccl.c $(LIBS)
	$(XCC) $(OPT) $(XCCCLOPT) $(THROPT) $(WORKER) cmp-xcccl.c $(LIBS) -o cmp-xcccl
pen-xcccl: $(WORKER) pen-xcccl.c $(LIBS)
	$(XCC) $(OPT) $(XCCCLOPT) $(THROPT) $(WORKER) pen-xcccl.c $(LIBS) -o pen-xcccl
pen6x17-xcccl: $(WORKER) pen6x17-xcccl.c $(LIBS)
	$(XCC) $(OPT) $(XCCCLOPT) $(THROPT) $(WORKER) pen6x17-xcccl.c $(LIBS) -o pen6x17-xcccl
nq-xcccl: $(WORKER) nq-xcccl.c $(LIBS)
	$(XCC) $(OPT) $(XCCCLOPT) $(THROPT) $(WORKER) nq-xcccl.c $(LIBS) -o nq-xcccl
grav-xcccl: $(WORKER) grav-xcccl.c $(LIBS)
	$(XCC) $(OPT) $(XCCCLOPT) $(THROPT) $(WORKER) grav-xcccl.c $(LIBS) -o grav-xcccl


# もとのC Version
009b: 009b.c ../../obayashi/fib.c
	$(GCC) $(OPT) $(THROPT) 009b.c ../../obayashi/fib.c -o 009b

# workerだけSCで書き直し
fib-orig: worker.c ../../obayashi/fib.c
	$(GCC) $(OPT) $(THROPT) worker.c ../../obayashi/fib.c -o fib-orig

# demo
# pen-demo: $(WORKER_GCC) pen-gcc-gtk.c $(LIBS)
#	$(GCC) $(OPT)  $(THROPT) $(WORKER_GCC) pen-gcc-gtk.c $(LIBS) -o pen-demo `pkg-config --cflags --libs gtk+-2.0`

# clean:
# 	rm -f $(PROGS)
