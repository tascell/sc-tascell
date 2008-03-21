# Simulate CILK before installation
# To run the check after installation, try
#  make CILK=cilk CILK2C=/usr/local/lib/cilk/cilk2c check
CILK=$(top_builddir)/support/cilkclocal
CILK2C=$(top_builddir)/cilk2c/cilk2c

SUFFIXES = .cilk
NPROC = 4
PROGFLAGS = --nproc $(NPROC)

AM_CILKFLAGS=@CFLAGS@ @PERFCTR_LIBS@
# Compile above with 
#  ./configure CFLAGS="-ansi -pedantic -D_POSIX_SOURCE -Wno-long-long"
# to get lots of ansi and pedantic checking.
CILKFLAGS=
CILKCOMPILE = $(CILK) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CILKFLAGS) $(CILKFLAGS)
CILKCPP = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CILKFLAGS) $(CILKFLAGS)
CILKLD = $(CILK)
CILKLINK = $(CILKLD) $(AM_CILKFLAGS) $(CILKFLAGS) $(LDFLAGS) -o $@
LINK=$(CILKLINK)

# As we edit this list, tend to make the list be one entry per line so that "svn diff"
# can better understand what happened.
noinst_PROGRAMS=hello fib fib-serial testall ck magic knapsack queens	\
cilksort fft test-locks bucket cholesky game nfib kalah matmul lu plu	\
heat test-errno strassen rectmul spacemul \
test-void-func \
velin \
cholesky-lib \
nfib-lib
MOSTLYCLEANFILES=pragma.E pragma.E.cilk pragma.E.c deprecated.log deprecated2.out deprecated2.log deprecated2.i

GETOPTIONS_SRC = getoptions.c getoptions.h

hello_SOURCES=hello.cilk
hello_LDADD=hello.o

fib_SOURCES=fib.cilk
fib_LDADD=fib.o

velin_SOURCES=velin.cilk
velin_LDADD=velin.o

fib_serial_SOURCES = fib-serial.c
fib-serial$(EXEEXT): $(fib_serial_OBJECTS) $(fib_serial_DEPENDENCIES)
	@rm -f fib-serial
	$(CC) -o $@ $(fib_serial_OBJECTS) $(LIBS) $(LDFLAGS)

strassen_SOURCES=strassen.cilk  $(GETOPTIONS_SRC)
strassen_LDADD=strassen.o

rectmul_SOURCES=rectmul.cilk  $(GETOPTIONS_SRC)
rectmul_LDADD=rectmul.o

spacemul_SOURCES=spacemul.cilk  $(GETOPTIONS_SRC)
spacemul_LDADD=spacemul.o

testall_SOURCES=testall.cilk rand.cilk
testall_LDADD=testall.o rand.o

test_locks_SOURCES=test-locks.cilk
test_locks_LDADD=test-locks.o

test_errno_SOURCES=test-errno.cilk
test_errno_LDADD=test-errno.o

test_void_func_SOURCES=test-void-func.cilk
test_void_func_LDADD=test-void-func.o

ck_SOURCES=ck.cilk rand.cilk $(GETOPTIONS_SRC)
ck_LDADD=ck.o rand.o

magic_SOURCES=magic.cilk $(GETOPTIONS_SRC)
magic_LDADD=magic.o

knapsack_SOURCES=knapsack.cilk $(GETOPTIONS_SRC)
knapsack_LDADD=knapsack.o 

queens_SOURCES=queens.cilk
queens_LDADD=queens.o

cilksort_SOURCES=cilksort.cilk $(GETOPTIONS_SRC)
cilksort_LDADD=cilksort.o

fft_SOURCES=fft.cilk $(GETOPTIONS_SRC)
fft_LDADD=fft.o -lm

bucket_SOURCES=bucket.cilk $(GETOPTIONS_SRC)
bucket_LDADD=bucket.o -lm

cholesky_SOURCES=cholesky.cilk rand.cilk $(GETOPTIONS_SRC)
cholesky_LDADD=cholesky.o rand.o -lm

cholesky_lib_SOURCES=cholesky-lib.cilk rand.cilk $(GETOPTIONS_SRC)
cholesky_lib_LDADD=cholesky-lib.o rand.o -lm

matmul_SOURCES=matmul.cilk rand.cilk $(GETOPTIONS_SRC)
matmul_LDADD=matmul.o rand.o

game_SOURCES=game.cilk $(GETOPTIONS_SRC)
game_LDADD=game.o

nfib_SOURCES=nfib.cilk
nfib_LDADD=nfib.o

nfib_lib_SOURCES=nfib-lib.cilk
nfib_lib_LDADD=nfib-lib.o


kalah_SOURCES=kalah.cilk
kalah_LDADD=kalah.o

lu_SOURCES=lu.cilk  $(GETOPTIONS_SRC)
lu_LDADD=lu.o -lm

plu_SOURCES=plu.cilk  $(GETOPTIONS_SRC)
plu_LDADD=plu.o -lm

heat_SOURCES=heat.cilk  $(GETOPTIONS_SRC)
heat_LDADD=heat.o -lm

EXTRA_DIST = knapsack-example1.input knapsack-example2.input	\
knapsack-example3.input

check:  check-pragma check-deprecated \
  check-cilkc-E0 \
  check-cilkc-E1 \
  hello$(EXEEXT) testall$(EXEEXT) fft$(EXEEXT) fib$(EXEEXT) lu$(EXEEXT) matmul$(EXEEXT) cilksort$(EXEEXT) heat$(EXEEXT) test-locks$(EXEEXT) test-errno$(EXEEXT) strassen$(EXEEXT) rectmul$(EXEEXT) spacemul$(EXEEXT) \
  test-void-func$(EXEEXT) \
  velin$(EXEEXT)
	@echo "Running Cilk tests.  This will take a while."
	./nfib -stats 3 3
	./hello $(PROGFLAGS)
	./test-void-func $(PROGFLAGS)
	./test-locks $(PROGFLAGS)
	./test-errno $(PROGFLAGS)
	./testall $(PROGFLAGS)
	./fft $(PROGFLAGS) -c
	./fib $(PROGFLAGS) 35
	./lu $(PROGFLAGS) -n 1024
	./matmul $(PROGFLAGS) 1066
	./cilksort $(PROGFLAGS) -n 3000000
	./heat $(PROGFLAGS)
	./strassen $(PROGFLAGS)
	./velin $(PROGFLAGS)

check-deprecated:
	$(CILK) deprecated.cilk -c 2> deprecated.log
	@fgrep "is deprecated" deprecated.log > /dev/null
	@echo " The cilk-lib.h deprecation feature appears to work"
	@touch deprecated2-2.h
	@touch deprecated2-3.cilkh
	$(CC) deprecated2.c -E -o deprecated2.i
	$(CILK2C) -name deprecated2.c deprecated2.i > deprecated2.out 2> deprecated2.log
	if fgrep "Warning:" deprecated2.log > /dev/null; then echo Test failed; exit 1; fi
	@rm deprecated2-2.h
	@rm deprecated2-3.cilkh
	@echo " The cilk-lib.h deprecation stickyness appears to work"

check-pragma:
	$(CILKCPP) -D__CILK__ -D__CILK2C__ -D_REENTRANT -x c pragma.cilk -E > pragma.E.cilk
	$(CILK2C) -W3 pragma.E.cilk -o pragma.E.c
	egrep '#ident[ ]+"fib to me"' pragma.E.c
	fgrep "#pragma int_to_unsigned" pragma.E.c

cilk-as-c:
	cp fib.cilk cilk-fib.c
	$(CILK) -c -x cilk cilk-fib.c

check-cilkc-E0:
	rm -f fib.cilki
	$(CILK) -E0 fib.cilk
	test -f fib.cilki
	rm -f fib.cilki
	$(CILK) -E0 fib.cilk -o fib.foo
	test -f fib.foo
	rm -f fib.foo 
	$(CILK) -save-temps fib.cilk
	test -f fib.cilki
	rm -f fib.cilki fib.cilkp fib.i fib.s fib.o fib

check-cilkc-E1:
	rm -f notmuch.cilkc notmuch.i notmuch.s notmuch.o
	$(CILK) -E1 notmuch.cilki
	test -f notmuch.cilkc
	rm -f notmuch.cilkc
	$(CILK) -E1 notmuch.cilki -o notmuch.foo
	test -f notmuch.foo
	rm -f notmuch.foo 
	$(CILK) -save-temps notmuch.cilki -c
	test -f notmuch.cilkc
	test -f notmuch.i
	test -f notmuch.s
	test -f notmuch.o
	rm -f notmuch.cilkc notmuch.i notmuch.s notmuch.o

# the following rules simulate automake's dependencies
# tracking for Cilk

# I don't see anything that uses this, and this syntax is causing some sort of trouble during "make install"
# on SGI, so I am commenting it out.
# DEPS_MAGIC := $(shell mkdir .deps > /dev/null 2>&1 || :)

%.o: %.cilk
	$(CILKCOMPILE) -M $< > .deps/$(*F).P
	$(CILKCOMPILE) -c $<
