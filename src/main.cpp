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


//////////////////////////////////////////////////////////////////////////////
/// @brief        Runs a pool worker.
///
/// @details
///
/// @param
/// @return
///
/// @pre
/// @post
///
/// @exception
///
void RunPoolWorker(Mpi::Communicator const& comm)
{
	printf("rank %d: running worker\n", comm.GetRank());

	int workerStatus = 1;
	while (workerStatus)
	{
		int myRank, parentId;
		MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
		parentId = getCommandData();    // We encode the parent ID in the wake up command data
		if (parentId == 0)
		{
			// If my parent is the master (one of the 10 that the master started) then spawn two further children
			int childOnePid = startWorkerProcess();
			int childTwoPid = startWorkerProcess();

			printf("Worker on process %d, started two child processes (%d and %d)\n", myRank, childOnePid, childTwoPid);
			// Wait for these children to send me a message
			MPI_Request childRequests[2];
			MPI_Irecv(NULL, 0, MPI_INT, childOnePid, 0, MPI_COMM_WORLD, &childRequests[0]);
			MPI_Irecv(NULL, 0, MPI_INT, childTwoPid, 0, MPI_COMM_WORLD, &childRequests[1]);
			MPI_Waitall(2, childRequests, MPI_STATUS_IGNORE);
			// Now tell the master that I am finished
			MPI_Send(NULL, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
		else
		{
			printf("Child worker on process %d started with parent %d\n", myRank, parentId);
			// Tell my parent that I have been alive and am about to die
			MPI_Send(NULL, 0, MPI_INT, parentId, 0, MPI_COMM_WORLD);
		}

		workerStatus = workerSleep(); // This MPI process will sleep, further workers may be run on this process now
	}
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
				RunPoolWorker(comm);
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
