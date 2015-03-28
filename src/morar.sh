#!/bin/bash
#$ -V
#$ -cwd
#$ -l h_rt=:10:

# Run with
# qsub -pe mpi N morar.sh <cells> <squirrels>
# ...where:
#   N is the number of PEs to run
#   <cells>      is the number of landscape cells.
#   <squirrels>  is the initial number of squirrels.
#   
# Note that <cells> + <squirrels> must be less than N.

(time mpiexec -n $NSLOTS ./squirrels $1 $2) 2>&1
