## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = no-dependencies
SUBDIRS = src
if MAINTAINER_MODE
SUBDIRS += tests .
endif

EXTRA_DIST = README.eng README.jpeuc config/mb_* \
    src/preproc.c src/noconfig.H src/*set_mcpp.sh tests/set_test.sh \
    noconfig cpp-test test-t test-c test-l tool doc doc.eng

install-data:
	@if test -d "$(prefix)/doc"; then :; else mkdir $(prefix)/doc; fi
	@if test -d "$(prefix)/doc/mcpp"; then :; else mkdir $(prefix)/doc/mcpp; fi
	cp -Rpf cpp-test test-t test-c test-l tool doc doc.eng $(prefix)/doc/mcpp

uninstall-data:
	rm -r -f $(prefix)/doc/mcpp

