# $HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/examples/nd/library/Makefile $ $LastChangedBy: bradley $ $Rev: 80 $ $Date: 2002-12-26 01:49:35 -0500 (Thu, 26 Dec 2002) $

# set this to point to the Cilk distribution directory.
CILK_DIR = ../../..

# include makefile from distribution directory
include $(CILK_DIR)/Makefile.common

LIBS	= -lm $(CILK_LIBRARY_RTS) $(CILK_LIB) $(ARCHLIBS)
HEADERS = $(CILK_HEADERS)

# macros for the Nondeterminator
CILK2CFLAGS_ND = $(CILK2CFLAGS) -nd
CFLAGS_ND  = -I$(ND_DIR) $(CFLAGS)
NDFLAGS = -DCILK_ND
 
CILK_LIBRARY_RTS_ND = $(ND_DIR)/cilk-library-rts.nd.a
LIBS_ND = -lm $(CILK_LIBRARY_RTS_ND) $(CILK_LIB) $(ARCHLIBS)
HEADERS_ND = $(CILK_HEADERS) $(ND_DIR)/cilk-nd.h $(ND_DIR)/cilk-nd-cilk2c.h

all:	fib.nd

# building the version with the Nondeterminator
%.nd: $(CILK_LIBRARY_RTS_ND) $(CILK_LIB)
%.nd:   %.nd.o
	$(CC) $(LDFLAGS) $< $(LIBS_ND) -o $@
 
%.nd.o: $(HEADERS_ND) $(CILK_DIR)/Makefile.common
%.nd.o: %.nd.c
	$(CC) $(CFLAGS_ND) $(NDFLAGS) -DCILK_LIBRARY_VERSION -c $< -o $@
 
%.nd.c: $(HEADERS_ND) $(CILK2C) $(CILK_DIR)/Makefile.common
%.nd.c: %.cilk
	$(CILK2C) $(CILK2CFLAGS_ND) -- $(CFLAGS_ND) $(NDFLAGS) -DCILK_LIBRARY_VERSION -- $< -o $@
 
# building executable without the Nondeterminator
%.o:	%.c $(HEADERS) $(CILK_DIR)/Makefile.common
	$(CC) $(CFLAGS) -DCILK_LIBRARY_VERSION -c $< -o $@

%.s:	%.c $(HEADERS) $(CILK_DIR)/Makefile.common
	$(CC) $(CFLAGS) -DCILK_LIBRARY_VERSION -S $< -o $@

%.c:	%.cilk $(HEADERS) $(CILK2C) $(CILK_DIR)/Makefile.common
	$(CILK2C) $(CILK2CFLAGS) -- $(CFLAGS) -DCILK_LIBRARY_VERSION -- $< -o $@ 

%:	%.o $(CILK_LIBRARY_RTS) $(CILK_LIB)
	$(CC) $(LDFLAGS) $< $(LIBS) -o $@

check: fib.nd race.nd
	@ ./fib.nd 2 > fib.nd.output.1 2> fib.nd.output.2
	@ diff fib.nd.output.2 /dev/null
	@ ./race.nd > race.nd.output.2 2> race.nd.output.2
	@ fgrep "Data race" race.nd.output.2 > /dev/null
	@ echo Library cilk appears OK with nondeterminator.
clean:	
	$(RM) *.o core a.out *~ *.s *.bak *~ fib.nd
	$(RM) fib.nd.output.? race.nd.output.?
