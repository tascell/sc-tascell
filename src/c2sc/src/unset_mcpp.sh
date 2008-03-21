#! /bin/sh
# script to set GNU CPP to be called from gcc
# ./unset_mcpp.sh ${gcc_path} ${gcc_maj_ver} ${cpp_call} ${CXX} x${EXEEXT}

cpp_path=`echo $3 | sed 's,/cpp.*,,'`
cpp_name=`echo $3 | sed 's,.*/,,'`
gcc_path=`echo $1 | sed 's,/gcc$,,'`
EXEEXT=`echo $5 | sed 's/^x//'`
if test x${EXEEXT} != x; then
    cpp_base=`echo ${cpp_name} | sed "s/${EXEEXT}//"`
else
    cpp_base=${cpp_name}
fi

echo "  cd ${cpp_path}"
cd ${cpp_path}
sym_link=`ls -l ${cpp_name} | sed 's/^l.*/l/; s/^[^l].*//'`;
if test x${sym_link} = xl && test -f ${cpp_base}_gnuc${EXEEXT}; then
    rm -f ${cpp_name}
    rm -f cpp.sh
    echo "  mv ${cpp_base}_gnuc${EXEEXT} ${cpp_name}"
    mv -f ${cpp_base}_gnuc${EXEEXT} ${cpp_name}
fi

if test x$2 != x3; then
    exit 0
fi

echo "  cd ${gcc_path}"
cd ${gcc_path}
sym_link=`ls -l gcc${EXEEXT} | sed 's/^l.*/l/; s/^[^l].*//'`
if test x${sym_link} = xl; then     # symbolic link
    rm -f gcc${EXEEXT} gcc.sh
    echo "  mv gcc_proper${EXEEXT} gcc${EXEEXT}"
    mv -f gcc_proper${EXEEXT} gcc${EXEEXT}
fi
sym_link=`ls -l $4 | sed 's/^l.*/l/; s/^[^l].*//'`
if test x${sym_link} = xl; then     # symbolic link
    rm -f $4${EXEEXT} $4.sh
    echo "  mv $4_proper${EXEEXT} $4${EXEEXT}"
    mv -f $4_proper${EXEEXT} $4${EXEEXT}
fi

