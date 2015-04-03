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
	#include "squirrel-functions.h"	
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

		//  buffer for calls to MPI_Bsend()
		size_t const buf_size = 256 * 1024;
		unsigned char mpi_buffer[buf_size];
		
		Mpi::Mpi mpi(argc, argv);
		Mpi::Communicator comm(MPI_COMM_WORLD);
		MPI_Buffer_attach(mpi_buffer, buf_size);

		Mpi::ProcessPool process_pool;

		if (0 == comm.GetRank())
		{
			config.Print();
		}

		std::cout << "rank " << comm.GetRank() << " of " << comm.GetSize() << " started" << std::endl;
		
		double start_time = MPI_Wtime();

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
				printf("rank %d: process pool says to exit\n", comm.GetRank());
				break;
			}
		}

		if (0 == comm.GetRank())
		{
			printf("run time:\t%f\n", MPI_Wtime() - start_time);
		}

		std::cout << "rank " << comm.GetRank() << " exiting" << std::endl;

		return 0;
	}
	catch (std::exception const& e)
	{
		std::cout << "error: " << e.what() << std::endl;
		return -1;
	}
}
