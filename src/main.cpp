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
#include "Config.hpp"


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


//////////////////////////////////////////////////////////////////////////////
/// @brief      Program entry point.
///
int main(int argc, char* argv[])
{
	try
	{
		Pdp::Config config(argc, argv);
		
		Mpi::Mpi mpi(argc, argv);
		double start_time = MPI_Wtime();

		Mpi::Communicator comm(MPI_COMM_WORLD);
		Mpi::ProcessPool process_pool;

		if (0 == comm.GetRank())
		{
			PrintCmdLine(argc, argv);
		}

		//  printf works better than std::cout when using MPI
		printf("rank %d of %d\n", comm.GetRank(), comm.GetSize());

		switch(process_pool.GetType())
		{
			case Mpi::ProcessPool::eMaster:
			{
				Mpi::PoolMaster pool_master(comm, config);
				pool_master.Run();
				break;
			}
			case Mpi::ProcessPool::eWorker:
			{
				Mpi::PoolWorker pool_worker(comm, config);
				pool_worker.Run();
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
			printf("run time:\t%f\n", MPI_Wtime() - start_time);
		}

		return 0;
	}
	catch (std::exception const& e)
	{
		std::cout << "error: " << e.what() << std::endl;
		return -1;
	}
}
