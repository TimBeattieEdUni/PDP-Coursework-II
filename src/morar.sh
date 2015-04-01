#!/bin/bash
#$ -V
#$ -cwd
#$ -l h_rt=:10:

# Run with
# qsub -pe mpi N morar.sh <cells> <squirrels> <daylen>
# ...where:
#   N is the number of PEs to run
#   <cells>      is the number of landscape cells.
#   <squirrels>  is the initial number of squirrels.
#   <daylen>     is the number of wall-time seconds in a simulated day.
#
# Note that <cells> + <squirrels> + 1 must be less than N.

(time mpiexec -n $NSLOTS ./squirrels $1 $2 $3) 2>&1
