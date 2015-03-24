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
#include "PoolMaster.hpp"
#include "PoolWorker.hpp"


/// @todo Remove this once pool is wrapped up.
extern "C"
{
	#include "pool.h"
}


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


void RunPoolMaster(Mpi::Communicator const& comm)
{
}


void RunPoolWorker(Mpi::Communicator const& comm)
{
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

		Mpi::Communicator comm(MPI_COMM_WORLD);
		Mpi::ProcessPool process_pool;

		//  printf works better than std::cout when using MPI
		printf("rank %d of %d\n", comm.GetRank(), comm.GetSize());

		if (0 == comm.GetRank())
		{
			PrintCmdLine(argc, argv);
		}

		switch(process_pool.GetType())
		{
			case Mpi::ProcessPool::eMaster:
			{
				Mpi::PoolMaster master(comm);
				master.Run();
				break;
			}
			case Mpi::ProcessPool::eWorker:
			{
				Mpi::PoolWorker worker(comm);
				worker.Run();
				break;
			}
			case Mpi::ProcessPool::eQuit:
			{
				printf("rank %d: exiting\n", comm.GetRank());
				break;
			}
		}

		if (0 == comm.GetRank())
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
