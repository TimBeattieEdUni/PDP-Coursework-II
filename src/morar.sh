#!/bin/bash
#$ -V
#$ -cwd
#$ -l h_rt=:10:

# Run with
# qsub -pe mpi N morar.sh <cells> <squirrels> <infected> <maxsq> <simdays> <daylen> 
# ...where:
#   N is the number of PEs to run
#   <cells>      is the number of landscape cells.
#   <squirrels>  is the number of squirrels at the start of the simulation.
#   <infected>   is the number of infected squirrels at the start of the simulation.
#   <maxsq>      is the maximum number of squirrels allowed.
#   <simdays>    is the maximum length of the simulation in days.
#   <daylen>     is the length of the simulation's day in wall-time seconds.
#
#
# The program will run for up to <simday> * <daylen> seconds.
# 
# Note that <cells> + <squirrels> + 1 must be less than N.


(time mpiexec -n $NSLOTS ./squirrels $1 $2 $3 $4 $5 $6) 2>&1
