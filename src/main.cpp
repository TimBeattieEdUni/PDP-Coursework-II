//////////////////////////////////////////////////////////////////////////////
/// @file       main.cpp
///
/// @brief      Provides main() for "squirrels" program.
///
/// @details	Parallel Design Patterns
///             Coursework Part II
///				B063622
///


//////////////////////////////////////////////////////////////////////////////
//  Local headers.
#include "Mpi.hpp"
#include "MpiCommunicator.hpp"


//////////////////////////////////////////////////////////////////////////////
//  Headers provided as part of the coursework.
extern "C"
{
	#include "pool.h"
}


//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <iostream>
#include <sstream>


//////////////////////////////////////////////////////////////////////////////
/// @brief      Program entry point.
///
int main(int argc, char* argv[])
{
	try
	{
		Mpi mpi(argc, argv);
		MpiCommunicator mpi_comm(MPI_COMM_WORLD);
		
		double start_time = MPI_Wtime();

		//  printf works better than std::cout when using MPI
		printf("rank %d of %d\n", mpi_comm.GetRank(), mpi_comm.GetSize());

		int init_val = processPoolInit();
		printf("rank %d: processPoolInit() returned %d\n", mpi_comm.GetRank(), init_val);

		switch(init_val)
		{
			case 1:
			{
				break;
			}
			case 2:
			{
				break;
			}
			default:
			{
				break;
			}
		}
		
		processPoolFinalise();

		printf("rank %d exiting\n", mpi_comm.GetRank());

		if (0 == mpi_comm.GetRank())
		{
			//  print command line
			std::stringstream ss;
			int i;
			ss << argv[0];
			for (i = 1; i < argc; ++i)
			{
				ss << " " << argv[i];
			}

			printf("%s\n", ss.str().c_str());
		}

		if (0 == mpi_comm.GetRank())
		{
			printf("total program time:\t%f\n", MPI_Wtime() - start_time);
		}

		return 0;
	}
	catch (std::exception const& e)
	{
		std::cout << "error: " << e.what() << std::endl;
		return -1;
	}
}
