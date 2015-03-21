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
#include "ProcessPool.hpp"


//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <iostream>
#include <sstream>


//////////////////////////////////////////////////////////////////////////////
/// @brief      Prints command line.
///
/// @param      argc Standard argument count.
/// @param      argv Standard argument list.
///
void PrintCmdLine(int argc, char* argv[])
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


//////////////////////////////////////////////////////////////////////////////
/// @brief      Program entry point.
///
int main(int argc, char* argv[])
{
	try
	{
		Mpi::Mpi mpi(argc, argv);
		double start_time = MPI_Wtime();

		Mpi::MpiCommunicator mpi_comm(MPI_COMM_WORLD);
		Mpi::ProcessPool process_pool;

		//  printf works better than std::cout when using MPI
		printf("rank %d of %d\n", mpi_comm.GetRank(), mpi_comm.GetSize());

		printf("rank %d: processPoolInit() returned %d\n", mpi_comm.GetRank(), process_pool.GetType());

		switch(process_pool.GetType())
		{
			case Mpi::ProcessPool::eMaster:
			{
				printf("rank %d: master\n", mpi_comm.GetRank());
				break;
			}
			case Mpi::ProcessPool::eWorker:
			{
				printf("rank %d: worker\n", mpi_comm.GetRank());
				break;
			}
			case Mpi::ProcessPool::eQuit:
			{
				printf("rank %d: quit\n", mpi_comm.GetRank());
				break;
			}
		}
		
		printf("rank %d exiting\n", mpi_comm.GetRank());

		if (0 == mpi_comm.GetRank())
		{
			PrintCmdLine(argc, argv);
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
