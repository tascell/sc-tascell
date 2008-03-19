#! /bin/sh
# script to set mcpp to be called from gcc
# ./set_mcpp.sh ${gcc_path} ${gcc_maj_ver} ${cpp_call} ${CXX} x${EXEEXT}
#       ${LN_S}

cpp_path=`echo $3 | sed 's,/cpp.*,,'`;
cpp_name=`echo $3 | sed 's,.*/,,'`;
gcc_path=`echo $1 | sed 's,/gcc$,,'`
EXEEXT=`echo $5 | sed 's/^x//'`
if test x${EXEEXT} != x; then
    cpp_base=`echo ${cpp_name} | sed "s/${EXEEXT}//"`
else
    cpp_base=${cpp_name}
fi

echo "  cd ${cpp_path}"
cd ${cpp_path}
echo '#!/bin/sh'        > cpp.sh;
if test x$2 = x2; then
    echo 'for i in $@'  >> cpp.sh
    echo '    do'       >> cpp.sh
    echo '    case $i in'           >> cpp.sh
    echo '        -traditional*)'   >> cpp.sh
    echo "            ${cpp_path}/${cpp_base}_gnuc"' "$@"'  >> cpp.sh
    echo '            exit ;;'      >> cpp.sh
    echo '    esac'     >> cpp.sh
    echo 'done'         >> cpp.sh
fi
echo ${cpp_path}/cpp_std -23j '$@'  >> cpp.sh;
chmod a+x cpp.sh
if test `echo '' | $3 -v - 2>&1 | grep 'GNU CPP' > /dev/null; echo $?` = 0; then
    sym_link=`ls -l ${cpp_name} | sed 's/^l.*/l/; s/^[^l].*//'`
    if test x${sym_link} != xl; then
        echo "  mv ${cpp_name} ${cpp_base}_gnuc${EXEEXT}"
        mv -f ${cpp_name} ${cpp_base}_gnuc${EXEEXT}
    fi
fi
if test -f ${cpp_name}; then
    rm -f ${cpp_name}
fi
echo "  $6 cpp.sh ${cpp_name}"
$6 cpp.sh ${cpp_name}

if test x$2 != x3; then
    exit 0
fi

echo "  cd ${gcc_path}"
cd ${gcc_path}
echo '#!/bin/sh' > gcc.sh
echo ${gcc_path}/gcc_proper -no-integrated-cpp '$@' >> gcc.sh
echo '#!/bin/sh' > $4.sh
echo ${gcc_path}/$4_proper -no-integrated-cpp '$@'  >> $4.sh
chmod a+x gcc.sh $4.sh
sym_link=`ls -l gcc${EXEEXT} | sed 's/^l.*/l/; s/^[^l].*//'`
if test x${sym_link} != xl; then    # not symbolic link
    echo "  mv gcc${EXEEXT} gcc_proper${EXEEXT}"
    mv -f gcc${EXEEXT} gcc_proper${EXEEXT}
else
    rm -f gcc${EXEEXT}
fi
sym_link=`ls -l $4${EXEEXT} | sed 's/^l.*/l/; s/^[^l].*//'`
if test x${sym_link} != xl; then    # not symbolic link
    echo "  mv $4${EXEEXT} $4_proper${EXEEXT}"
    mv -f $4${EXEEXT} $4_proper${EXEEXT}
else
    rm -f $4${EXEEXT}
fi
echo "  $6 gcc.sh gcc${EXEEXT}"
$6 gcc.sh gcc${EXEEXT}
echo "  $6 $4.sh $4${EXEEXT}"
$6 $4.sh $4${EXEEXT}

