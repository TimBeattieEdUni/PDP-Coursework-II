MSc HPC
Parallel Design Patterns
Coursework Part II
Exam no. B063622

To build the program, run:

make


The program can then be submitted to MORAR's SGE batch system.  
Below is an example:

	qsub -pe mpi 64 morar.sh 16 40 46 56 0.01

This set of values has been found to work reasonably well and completes in a few seconds.


In more detail, the program is run as follows:

	qsub -pe mpi N morar.sh <cells> <squirrels> <maxsq> <simdays> <daylen> 

...where:

	N is the number of PEs to run
	<cells>      is the number of landscape cells.
	<squirrels>  is the initial number of squirrels.
	<maxsq>      is the maximum number of squirrels allowed.
	<simdays>    is the maximum length of the simulation in days.
	<daylen>     is the number of wall-time seconds in a simulated day.


Note that:
	<cells> must be 16 due to a hard-coded value in the biologists' code.
	N must be at least <cells> + <squirrels> + 2.
	<maxsq> must be at least equal to <squirrels>.
	The program will run for up to <simdays> * <daylen> seconds.
