#!/bin/sh
# set SC_PREFIX to your SC directory
export SC_PREFIX=/path/to/sc-directory

export SC_PATH=$SC_PREFIX/bin
export SC_CMDLINE=$SC_PREFIX/src/sc-cmdline.lsp
SC_LISP=alisp
SC_LISP_PATH=`which alisp`
if [ $? != 0 ]; then
    SC_LISP=clisp
    SC_LISP_PATH=`which clisp`
    if [ $? != 0 ]; then
        SC_LISP=alisp
        SC_LISP_PATH='/cygdrive/c/Program Files/acl81/alisp' # Path to windows ACL
        if [ -x $SC_LISP_PATH ]; then
            SC_CMDLINE=`cygpath -m $SC_CMDLINE`
        else
            echo "alisp nor clisp found"
            exit 99
        fi
    fi
fi
export SC_LISP_PATH
export SC_LISP
export PATH=$SC_PATH:$PATH
