# $HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/examples/Barnes-Hut/Makefile $ $LastChangedBy: bradley $ $Rev: 79 $ $Date: 2002-12-26 01:43:37 -0500 (Thu, 26 Dec 2002) $

# set this to point to the Cilk distribution directory.
CILK_DIR = ../..

# include makefile from distribution directory
include $(CILK_DIR)/Makefile.common

# set this to 1 to enable critical-path measurements
CILK_CRITICAL_PATH = 1

LIBS	= -lm $(CILK_RTS) $(CILK_LIB) $(ARCHLIBS)
HEADERS	= $(CILK_HEADERS)

all:	code

%.o:	%.c $(HEADERS) $(CILK_DIR)/Makefile.common
	$(CC) $(CFLAGS) -DDOUBLEPREC -c $< -o $@

%.s:	%.c $(HEADERS) $(CILK_DIR)/Makefile.common
	$(CC) $(CFLAGS) -DDOUBLEPREC -S $< -o $@

%.c:	%.cilk $(HEADERS) $(CILK2C) $(CILK_DIR)/Makefile.common
	$(CILK2C) $(CILK2CFLAGS) -- $(CFLAGS) -DDOUBLEPREC -- $< -o $@ 

code:	code.o io.o load.o grav.o util.o getparam.o
	$(CC) $(LDFLAGS) code.o io.o load.o grav.o util.o getparam.o $(LIBS) -o code

clean:	
	$(RM) *.o core a.out *~ *.s *.bak code.c grav.c load.c io.c

distclean: clean
	$(RM) code
