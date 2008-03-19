## Process this file with automake to produce Makefile.in

##AUTOMAKE_OPTIONS = no-dependencies

bin_PROGRAMS = cpp_std cpp_poststd
libexec_PROGRAMS = cpp_std cpp_poststd
cpp_std_SOURCES = system.H configed.H internal.H main.c control.c eval.c expand.c support.c system.c mbchar.c lib.c
cpp_poststd_SOURCES = $(cpp_std_SOURCES)
cpp_poststd_CPPFLAGS = -DMODE=POST_STANDARD

if MAINTAINER_MODE

bin_PROGRAMS += cpp_prestd cpp_old
libexec_PROGRAMS += cpp_prestd cpp_old
cpp_prestd_SOURCES = $(cpp_std_SOURCES)
cpp_old_SOURCES = $(cpp_std_SOURCES)
cpp_prestd_CPPFLAGS = -DMODE=PRE_STANDARD
cpp_old_CPPFLAGS = -DMODE=PRE_STANDARD -DOLD_PREPROCESSOR
install-exec-hook:
	@if test x"$(CC)" = x"gcc"; then \
        ./set_mcpp.sh '$(gcc_path)' $(gcc_maj_ver) '$(cpp_call)' '$(CXX)' 'x$(EXEEXT)' '$(LN_S)'; \
    fi
uninstall-hook:
	@if test x"$(CC)" = x"gcc"; then \
        ./unset_mcpp.sh '$(gcc_path)' $(gcc_maj_ver) '$(cpp_call)' '$(CXX)' 'x$(EXEEXT)'; \
    fi

else

install-exec-hook:
uninstall-hook:

endif

