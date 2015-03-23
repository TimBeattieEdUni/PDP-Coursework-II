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
/// @brief        Runs the process pool master.
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
void RunPoolMaster(Mpi::Communicator const& comm)
{
	printf("rank %d: running master\n", comm.GetRank());	
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
				RunPoolMaster(comm);
				break;
			}
			case Mpi::ProcessPool::eWorker:
			{
				RunPoolWorker(comm);
				break;
			}
			case Mpi::ProcessPool::eQuit:
			{
				printf("rank %d: exiting\n", mpi_comm.GetRank());
				break;
			}
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
