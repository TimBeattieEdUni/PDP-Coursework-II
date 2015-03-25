#!/bin/bash
#$ -V
#$ -cwd
#$ -l h_rt=:10:

# Run with
# qsub -pe mpi N morar.sh <cells>
# ...where:
#   N is the number of PEs to run
#   L is the number of landscape cells

(time mpiexec -n $NSLOTS ./squirrels $1) 2>&1
