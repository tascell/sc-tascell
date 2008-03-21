# makefile to compile MCPP version 2.* for Turbo C, Borland C / TC, BC make
#		1998/08, 2003/11, 2004/02 	kmatsui
# To compile MCPP using resident cpp do
#		make
# To re-compile MCPP using compiled MCPP do
#		make -DPREPROCESSED
# To specify the preprocessor to compile MCPP
#		make -DCPP=cpp32_std
# To generate MCPP of modes other than STANDARD mode do as
#		make -DMODE=POST_STANDARD -DNAME=cpp32_poststd
# To link malloc() package of kmatsui do
#		make [-DPREPROCESSED] -DKMMALLOC
# To compile MCPP with C++, rename *.c other than lib.c and preproc.c to *.cpp,
#   then do
#		make -DCPLUS
# To compile with bcc32
#		make -DWIN32
# To use Turbo C (rather than Borland C) edit this makefile.

!if 	!$d( NAME)
!if 	$d( WIN32)
NAME = cpp32_std
!else
NAME = cpp_std
!endif
!endif

!if 	!$d( CPP)
!if 	$d( WIN32)
CPP = cpp32_std
!else
CPP = cpp_std
!endif
!endif

CFLAGS = $(CFLAGS) -c -a -d -f- -G # -5
	# Don't use -A to compile eval.c when OK_SIZE is set to TRUE
LINKFLAGS = -e$(NAME)

# BINDIR : Adjust to your system.
#	for Borland C V.5.5
CFLAGS = $(CFLAGS) -Oi
BINDIR = \BCC55\BIN
#	for Turbo C V.2.0
# BINDIR = \TC2
#	to make cpp for LSI C-86 V.3.3		# Don't use -DPREPROCESSED
# BINDIR = \LSIC86\BIN

!if 	$d( CPLUS)
LANG = -+
!else
LANG = -S1
	# Don't use -S1 to compile eval.c when OK_SIZE is set to TRUE
!endif

!if 	$d( WIN32)
# '-N -D__BORLANDC__=0x0452' to work around bugs of bcc32 V.4.0
CC = bcc32
# CFLAGS = $(CFLAGS) -N -D__BORLANDC__=0x0452
MM = 32
MEM =
!else
CC = bcc
# CC = tcc
MM = l
MEM = -m$(MM)
!endif

!if		$d( MODE)
CPPFLAGS = -DMODE=$(MODE)
!else
CPPFLAGS =
!endif

!if 	$d( KMMALLOC)
MEM_MACRO = -DKMMALLOC=1 -D_MEM_DEBUG=1 -DXMALLOC=1
!if 	$d( WIN32)
MEMLIB = kmmalloc_debug32.lib
!else
MEMLIB = kmma_d_$(MM).lib
!endif
!else
MEM_MACRO =
MEMLIB =
!endif

OBJS = main.obj control.obj eval.obj expand.obj support.obj system.obj  \
    mbchar.obj lib.obj

$(NAME).exe : $(OBJS)
	$(CC) $(MEM) $(LINKFLAGS) $(OBJS) $(MEMLIB)

!if 	$d( PREPROCESSED)
CMACRO =
# Make a "pre-preprocessed" header file to recompile MCPP with MCPP.
cpp.H	: system.H noconfig.H internal.H
	$(CPP) $(LANG) $(CPPFLAGS) $(MEM_MACRO) $(MEM) preproc.c cpp.H
$(OBJS) : cpp.H
!else
CMACRO = $(MEM_MACRO)
main.obj control.obj eval.obj expand.obj support.obj system.obj mbchar.obj:	\
	system.H noconfig.H internal.H
lib.obj : system.H noconfig.H
!endif

!if 	$d( PREPROCESSED)
!if 	$d( CPLUS)
.cpp.obj:
	$(CPP) $(LANG) $(MEM) -DPREPROCESSED=1 $< $(<B).i
	$(CC) $(CFLAGS) $(MEM) $(<B).i
.c.obj	:
	$(CC) $(CFLAGS) $(MEM) $(CMACRO) $<
!else
.c.obj	:
	$(CPP) $(LANG) $(MEM) -DPREPROCESSED=1 $< $(<B).i
	$(CC) $(CFLAGS) $(MEM) $(<B).i
!endif
!else
.cpp.obj:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(MEM) $(CMACRO) $<
.c.obj	:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(MEM) $(CMACRO) $<
!endif

install :
	copy /b $(NAME).exe $(BINDIR)\$(NAME).exe

clean	:
	-del *.obj
	-del $(NAME).exe
	-del *.bak
	-del cpp.H
	-del *.i
	-del *.tds

