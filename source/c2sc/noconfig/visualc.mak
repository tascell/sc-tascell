# makefile to compile MCPP version 2.* for Visual C / nmake
#		2003/11, 2004/02 	kmatsui
# To compile MCPP using resident cpp do
#		nmake
# To re-compile MCPP using compiled MCPP do
#		nmake PREPROCESSED=1
# To specify the preprocessor to compile MCPP
#		nmake CPP=cpp32_std
# To generate MCPP of modes other than STANDARD mode do as
#		nmake MODE=POST_STANDARD NAME=cpp32_poststd
# To link kmmalloc V.2.5 (malloc() package of kmatsui) or later do
#		nmake [PREPROCESSED=1] KMMALLOC=1
# To compile MCPP with C++, rename *.c other than lib.c to *.cpp and do
#		nmake CPLUS=1
# $(NAME), $(CPP) can be specified by command-line as
#		nmake NAME=cpp32_prestd
# WARNING: If you build by "makefile project" on Visual C IDE, you must
#       specify CPP=cpp* option, otherwise IDE will define CPP as 'cl'.

!ifndef NAME
NAME = cpp32_std
!endif

!ifndef CPP
CPP = cpp32_std
!endif

CFLAGS = $(CFLAGS) -Za -c	# -Zi # for debugging on Visual C / IDE
	# Don't use -Za to compile eval.c when OK_SIZE is set to TRUE
LINKFLAGS = -Fe$(NAME)		# -Zi

# to "pre-preprocess" with MCPP
!ifndef MODE
MODE = STANDARD
!endif
CPPFLAGS = -DMODE=$(MODE)

!ifndef PREPROCESSED
PREPROCESSED = 0
!endif
CPPFLAGS = $(CPPFLAGS) -DPREPROCESSED=$(PREPROCESSED)

# BINDIR : Adjust to your system.
#	for Visual C++ .net 2003
BINDIR = "$(MSVCDIR)"\bin
#	to make cpp for LSI C-86 V.3.3		# Don't use -DPREPROCESSED
# BINDIR = \LSIC86\BIN

!ifdef CPLUS
LANG = -Tp
!else
LANG = -Tc
	# Don't use -S1 to compile eval.c when OK_SIZE is set to TRUE
!endif

CC = cl

!ifdef KMMALLOC
MEM_MACRO = -DKMMALLOC -D_MEM_DEBUG -DXMALLOC
MEMLIB = kmmalloc_debug.lib
!else
MEM_MACRO =
MEMLIB =
!endif

OBJS = main.obj control.obj eval.obj expand.obj support.obj system.obj  \
    mbchar.obj lib.obj

$(NAME).exe : $(OBJS)
	$(CC) $(LINKFLAGS) $(OBJS) $(MEMLIB)
#install :
# Visual C++ / IDE "makefile project" do not has "make install" command
	copy /b $(NAME).exe $(BINDIR)\$(NAME).exe

!if $(PREPROCESSED)
CMACRO =
# make a "pre-preprocessed" header file to recompile MCPP with MCPP.
cpp.H	: system.H internal.H
	$(CPP) $(CPPFLAGS) $(LANG) $(MEM_MACRO) preproc.c cpp.H
$(OBJS) : cpp.H
system.H	: noconfig.H
!else
CMACRO = $(MEM_MACRO)
main.obj control.obj eval.obj expand.obj support.obj system.obj mbchar.obj:	\
	system.H internal.H
lib.obj : system.H
system.H	: noconfig.H
!endif

!if $(PREPROCESSED)
!ifdef CPLUS
.cpp.obj:
	$(CPP) $(CPPFLAGS) $(LANG) $< $(<B).i
	$(CC) $(CFLAGS) $(CMACRO) $(LANG) $(<B).i
.c.obj	:
	$(CPP) $< $(<B).i
	$(CC) $(CFLAGS) $(CMACRO) -Tc $(<B).i
!else
.c.obj	:
	$(CPP) $(CPPFLAGS) $(LANG) $< $(<B).i
	$(CC) $(CFLAGS) $(CMACRO) $(LANG) $(<B).i
!endif
!else
!ifdef CPLUS
.cpp.obj:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CMACRO) $(LANG) $<
.c.obj	:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CMACRO) -Tc $<
!else
.c.obj	:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CMACRO) $(LANG) $<
!endif
!endif

clean	:
	-del *.obj
	-del *.i
	-del cpp.H

