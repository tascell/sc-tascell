
#============ Options ============
#QSUB -q gr10034b
#QSUB -ug gr10034
#QSUB -W 2:00
#QSUB -A p=2:t=1:c=1:m=120G
#QSUB -N cmpd2-1
#============ Shell Script ============
set -x
cd $QSUB_WORKDIR

# automatically
# export OMP_NUM_THREADS=$QSUB_THREADS

mpiexec.hydra ./cmp-gcc -n 1 -v 1 -i "0 10000 -1"