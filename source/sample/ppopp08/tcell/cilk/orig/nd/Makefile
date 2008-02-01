# $HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/examples/nd/Makefile $ $LastChangedBy: bradley $ $Rev: 80 $ $Date: 2002-12-26 01:49:35 -0500 (Thu, 26 Dec 2002) $

# set this to point to the Cilk distribution directory.
CILK_DIR = ../..

# include makefile from distribution directory
include $(CILK_DIR)/Makefile.common

LIBS    = -lm $(CILK_RTS) $(CILK_LIB) $(ARCHLIBS)
HEADERS = $(CILK_HEADERS)

all:	race.nd

# building the version with the Nondeterminator
%.nd:   %.nd.o $(CILK_RTS_ND) $(CILK_LIB)
	$(CC) $(LDFLAGS) $< $(LIBS_ND) -o $@
 
%.nd.o: %.nd.c $(HEADERS_ND) $(CILK_DIR)/Makefile.common
	$(CC) $(CFLAGS_ND) $(NDFLAGS) -c $< -o $@
 
%.nd.c: %.cilk $(HEADERS_ND) $(CILK2C) $(CILK_DIR)/Makefile.common
	$(CILK2CND) $(CILK2CFLAGS_ND) -- $(CFLAGS_ND) $(NDFLAGS) -- $< -o $@
 
# building executable without the Nondeterminator
%.o:	%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

%.s:	%.c $(HEADERS)
	$(CC) $(CFLAGS) -S $< -o $@

%.c:	%.cilk $(HEADERS) $(CILK2C)
	$(CILK2C) $(CILK2CFLAGS) -- $(CFLAGS) -- $< -o $@

%:	%.o $(CILK_RTS) $(CILK_LIB)
	$(CC) $(LDFLAGS) $< $(LIBS) -o $@

check:  race.nd
	@ ./race.nd > /dev/null 2> race.nd.output
	@ fgrep "Data race" race.nd.output > /dev/null
	@ echo nondeterminator looks OK.
	(cd library; $(MAKE) check)

clean:	
	$(RM) *.o core a.out *~ *.s *.bak *~
	$(RM) race.nd.output
	(cd library; $(MAKE) clean)
