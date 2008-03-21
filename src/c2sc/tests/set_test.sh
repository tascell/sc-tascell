#! /bin/sh
# script to set mcpp testsuite corresponding to the version of GCC 2 or 3
# ./set_test.sh ${gcc_testsuite_dir} ${gcc_maj_ver} ${LN_S}

cur_dir=`pwd`
echo "  cd $1/gcc.dg/cpp-test/test-t"
cd "$1/gcc.dg/cpp-test/test-t"
for i in *_run.c
do
    rm -f $i
    echo "  $3 $i.gcc$2 $i"
    $3 $i.gcc$2 $i
done
echo "  cd ${cur_dir}"
cd "${cur_dir}"
