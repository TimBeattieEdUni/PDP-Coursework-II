//////////////////////////////////////////////////////////////////////////////
/// @file     PoolMaster.cpp
///
/// @brief    Implementation of class PoolMaster.
///


//////////////////////////////////////////////////////////////////////////////
//  This class's header.
#include "PoolMaster.hpp"


//////////////////////////////////////////////////////////////////////////////
//  Local headers.
extern "C"
{
	#include "pool.h"
}


//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <iostream>


namespace Mpi
{
	//////////////////////////////////////////////////////////////////////////////
	/// @details    Describe object initialisation here.
	///
	/// @param      Describe parameters here, one line each.
	///
	/// @post       List what is guaranteed to be true after this function returns.
	///
	/// @exception  List exceptions this function may throw here.
	///
	PoolMaster::PoolMaster(Communicator const& comm)
		: m_comm(comm)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}


	//////////////////////////////////////////////////////////////////////////////
	/// @details    Describe object destruction here.
	///
	/// @param      Describe parameters here, one line each.
	///
	/// @pre        List what must be true before this function is called.
	/// @post       List what is guaranteed to be true after this function returns.
	///
	/// @exception  None; this is a destructor.
	///
	PoolMaster::~PoolMaster()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}


	//////////////////////////////////////////////////////////////////////////////
	/// @details    Runs the pool's master process.
	///
	void PoolMaster::Run()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;

//		int initial_workers = 1;
		startWorkerProcess();
		while(masterPoll())
		{
			std::cout << "master woke up" << std::endl;
		}

		/*
		 * This is the master, each call to master poll will block until a message is received and then will handle it and return
		 * 1 to continue polling and running the pool and 0 to quit.
		 * Basically it just starts 10 workers and then registers when each one has completed. When they have all completed it
		 * shuts the entire pool down
		 */
//		int i, activeWorkers = 0, returnCode;
//		MPI_Request initialWorkerRequests[initial_workers];
//		for (i = 0; i < initial_workers; i++)
//		{
//			int workerPid = startWorkerProcess();
//			MPI_Irecv(NULL, 0, MPI_INT, workerPid, 0, MPI_COMM_WORLD, &initialWorkerRequests[i]);
//			activeWorkers++;
//			printf("Master started worker %d on MPI process %d\n", i , workerPid);
//		}
//		int masterStatus = masterPoll();
//		while (masterStatus)
//		{
//			masterStatus = masterPoll();
//			for (i = 0; i < initial_workers; i++)
//			{
//				// Checks all outstanding workers that master spawned to see if they have completed
//				if (initialWorkerRequests[i] != MPI_REQUEST_NULL)
//				{
//					MPI_Test(&initialWorkerRequests[i], &returnCode, MPI_STATUS_IGNORE);
//					if (returnCode)
//					{
//						activeWorkers--;
//					}
//				}
//			}
//			// If we have no more active workers then quit poll loop which will effectively shut the pool down when  processPoolFinalise is called
//			if (activeWorkers == 0)
//			{
//				break;
//			}
//		}
	}

//		//////////////////////////////////////////////////////////////////////////////
//		/// @details    Describe copy construction here.
//		///
//		/// @param      rhs  Object to copy.
//		///
//		/// @pre        List what must be true before this function is called.
//		/// @post       List what is guaranteed to be true after this function returns.
//		///
//		/// @exception  List exceptions this function may throw here.
//		///
//		PoolMaster::PoolMaster(PoolMaster const& rhs)
//		{
//			std::cout << __PRETTY_FUNCTION__ << std::endl;
//
//			(void) rhs;
//		}
//
//
//		//////////////////////////////////////////////////////////////////////////////
//		/// @details    Describe object assignment here.
//		///
//		/// @param      rhs  Object on the right-hand side of the assignment statement.
//		/// @return     Object which has been assigned.
//		///
//		/// @pre        List what must be true before this function is called.
//		/// @post       List what is guaranteed to be true after this function returns.
//		///
//		/// @exception  List exceptions this function may throw here.
//		///
//		PoolMaster& PoolMaster::operator=(PoolMaster const& rhs)
//		{
//			std::cout << __PRETTY_FUNCTION__ << std::endl;
//
//			(void) rhs;
//			return *this;
//		}

}   //  namespace Mpi
