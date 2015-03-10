//////////////////////////////////////////////////////////////////////////////
/// @file       main.cpp
///
/// @brief      Provides main() for "squirrels" program.
///


//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <iostream>


//////////////////////////////////////////////////////////////////////////////
//  Local headers.
#include "Mpi.hpp"
#include "MpiCommunicator.hpp"


//////////////////////////////////////////////////////////////////////////////
/// @brief      Program entry point.
///
/// @details    Nowt yet.
///
int main(int argc, char* argv[])
{
	Mpi mpi(argc, argv);
	MpiCommunicator mpi_comm(MPI_COMM_WORLD);

	double prog_start_time = MPI_Wtime();

	int size = mpi_comm.GetSize();
	int rank = mpi_comm.GetRank();

	std::cout << "rank " << rank << " of " << size << std::endl;

	if (0 == rank)
	{
		//  print command line
		int i;
		std::cout << argv[0];
		for (i = 0; i < argc; ++i)
		{
			std::cout << " " << argv[i];
		}
		std::cout << std::endl;
	}

	if (0 == rank)
	{
		double total_prog_time = MPI_Wtime() - prog_start_time;
		printf("total program time:\t%f\n", total_prog_time);
	}
}
