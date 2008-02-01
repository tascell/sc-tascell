# set this to point to the Cilk distribution directory.
CILK_DIR = /home/drd/cilk

# set this to 1 to enable critical-path measurements
CILK_CRITICAL_PATH = 0

# set this to 1 to compile a regular serial C version
CILK_SERIAL = 0

# include makefile from distribution directory

LIBS	= -lm $(CILK_RTS) $(CILK_LIB) $(ARCHLIBS)
HEADERS = $(CILK_HEADERS) eval.h globals.h rtab.h depth.h weights.h
SHEADERS = s/eval.h s/globals.h s/rtab.h s/depth.h s/weights.h s/serial.h

SOURCES=depth.h eval.cilk eval.h globals.h pousse.cilk rtab.c rtab.h weights.h Makefile

ALL_POUSSE= pousse_4 pousse_5 pousse_6 pousse_7 pousse_8 pousse_9	\
        pousse_10 pousse_11 pousse_12 pousse_13 pousse_14 pousse_15	\
	pousse_16 pousse_17 pousse_18 pousse_19 pousse_20

ALL_SPOUSSE= spousse_4 spousse_5 spousse_6 spousse_7 spousse_8 spousse_9 \
	spousse_10 spousse_11 spousse_12 spousse_13 spousse_14 spousse_15 \
	spousse_16 spousse_17 spousse_18 spousse_19 spousse_20

# Flags for submission:
FLAGS= -DSUBMISSION -DMAX_DEPTH=100
NEWLDFLAGS=
TOTALCFLAGS= $(CFLAGS) $(FLAGS)
TOTALLDFLAGS= $(NEWLDFLAGS) $(LDFLAGS)

SERIALLDFLAGS= -pg
SERIALLIBS= -lm
SERIALCFLAGS=-O3 -Wall -DSERIAL -pg

pousseall: $(ALL_POUSSE)

spousseall: $(ALL_SPOUSSE)

pousse_%.o: pousse.cilk $(HEADERS)
	cilkc pousse.cilk -DBD_SIZE=$* $(TOTALCFLAGS) -c -o $@

eval_%.o: eval.cilk $(HEADERS) 
	cilkc -DBD_SIZE=$* $(TOTALCFLAGS) $< -c -o $@

# this rule links, say, pousse_6 from pousse_6 and eval_6
pousse_%: pousse_%.o eval_%.o rtab.o $(SOURCES) 
	cilkc $(TOTALLDFLAGS) pousse_$*.o eval_$*.o rtab.o $(LIBS) -o $@

spousse_%: $(SOURCES) s/spousse_%.o s/seval_%.o s/srtab.o
	$(CC) $(SERIALLDFLAGS) s/spousse_$*.o s/seval_$*.o s/srtab.o $(SERIALLIBS) -o $@

s/spousse_%.o: s/pousse.c $(SHEADERS) Makefile
	$(CC) -c $(SERIALCFLAGS) -DBD_SIZE=$* $< -o $@

s/seval_%.o: s/eval.c $(SHEADERS) Makefile
	$(CC) -c $(SERIALCFLAGS) -DBD_SIZE=$* $< -o $@

s/srtab.o: rtab.c $(SHEADERS) Makefile
	$(CC) -c $(SERIALCFLAGS) $< -o $@

distclean: clean
	rm -f $(ALL_POUSSE) eval_*.c pousse_*.c $(ALL_SPOUSSE)

clean :
	rm -f *.o *~ core s/*.h s/*.o


weights.h :
	touch weights.h

depth.h :
	touch depth.h

s/%.o:	s/%.c $(SHEADERS) Makefile
	$(CC) $(SERIALCFLAGS) -c $< -o $@

s/%.c:	%.cilk
	cp $< $@

s/%.h:	%.h
	cp $< $@

%.o:	%.c $(HEADERS) $(CILK_DIR)/Makefile.common Makefile
	$(CC) $(TOTALCFLAGS) -c $< -o $@

%.s:	%.c $(HEADERS) $(CILK_DIR)/Makefile.common Makefile
	$(CC) $(TOTALCFLAGS) -S $< -o $@

%.c:	%.cilk $(HEADERS) $(CILK2C) $(CILK_DIR)/Makefile.common Makefile
	$(CILK2C) $(CILK2CFLAGS) -- $(TOTALCFLAGS) -- $< -o $@

%:	%.o $(CILK_RTS) $(CILK_LIB)
	$(CC) $(LDFLAGS) $< $(LIBS) -o $@

tar : pousseall readme $(SOURCES)
	./make_tar_file $(SOURCES)


