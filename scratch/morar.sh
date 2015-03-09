#!/bin/bash
#$ -V
#$ -cwd
#$ -l h_rt=:10:


# Run with
# qsub -pe mpi N morar.sh
# ...where N is the number of PEs to run


(time mpiexec -n $NSLOTS ./squirrels) 2>&1
