# makefile to compile MCPP version 2.* for 4.4BSD / GNU C / UCB make
#		1998/08, 2003/11, 2004/02		kmatsui
# To compile MCPP using resident cpp do
#		make
# To re-compile MCPP using compiled MCPP do
#		make PREPROCESSED=1
# To generate MCPP of modes other than STANDARD mode do as
#		make MODE=POST_STANDARD NAME=cpp_poststd
# To link malloc() package of kmatsui do
#		make [PREPROCESSED=1] MALLOC=KMMALLOC
# To compile MCPP with C++, rename *.c other than lib.c and preproc.c to *.cc
#   then do
#		make CPLUS=1

NAME ?= cpp_std
CPP ?= cpp0
CC = gcc
GPP = g++
GCC = $(CC)
CPPOPTS = -v -Q
LINKFLAGS = -o $(NAME)

CFLAGS = -c -O2 -I/usr/local/include -Wall
# for gcc 3.x to use MCPP
#CFLAGS = -c -O2 -no-integrated-cpp -v -Wall

# Adjust to your system
BINDIR ?= /usr/libexec
#BINDIR ?= /usr/local/src/gcc-3.2-install/lib/gcc-lib/i386-unknown-freebsd4.7/3.2

CPLUS =
.if     $(CPLUS)
    GCC = $(GPP)
.endif

MALLOC =
.if		!empty(MALLOC)
.if		$(MALLOC) == KMMALLOC
    MEMLIB = /usr/local/lib/libkmmalloc_debug.a	# -lkmmalloc_debug
    MEM_MACRO = -D_MEM_DEBUG -DXMALLOC
.endif
    MEM_MACRO += -D$(MALLOC)
.else
    MEMLIB =
    MEM_MACRO =
.endif

OBJS = main.o control.o eval.o expand.o support.o system.o mbchar.o lib.o

$(NAME) : $(OBJS)
	$(GCC) $(LINKFLAGS) $(OBJS) $(MEMLIB)
#	strip $(NAME)

PREPROCESSED ?= 0

.if $(PREPROCESSED) == 1
CMACRO =
# Make a "pre-preprocessed" header file to recompile MCPP with MCPP.
cpp.H	: system.H noconfig.H internal.H
	$(GCC) -E -Wp,-b $(CPPOPTS) $(MEM_MACRO) -o cpp.H preproc.c
$(OBJS) : cpp.H
.else
CMACRO = $(MEM_MACRO)
$(OBJS) : system.H noconfig.H
main.o control.o eval.o expand.o support.o system.o mbchar.o: internal.H
.endif

.if $(CPLUS)
.cc.o	:
	$(GPP) $(CFLAGS) $(CMACRO) -DPREPROCESSED=$(PREPROCESSED) $<
.c.o	:
	$(CC) $(CFLAGS) $(CMACRO) $<
.else
.c.o	:
	$(CC) $(CFLAGS) $(CMACRO) -DPREPROCESSED=$(PREPROCESSED) $<
.endif

install :
	install -b $(NAME) $(BINDIR)/$(NAME)
# Do backup GNU CPP before executing the following command.
#.if $(NAME) != $(CPP)
#	ln -f $(BINDIR)/$(NAME) $(BINDIR)/$(CPP)
#.endif

clean	:
	-rm *.o cpp.H cpp.err

