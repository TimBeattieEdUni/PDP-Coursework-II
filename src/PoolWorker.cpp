//////////////////////////////////////////////////////////////////////////////
/// @file     PoolWorker.cpp
///
/// @brief    Implementation of class PoolWorker.
///


//////////////////////////////////////////////////////////////////////////////
//  This class's header.
#include "PoolWorker.hpp"


//////////////////////////////////////////////////////////////////////////////
//  Local headers.
#include "Cell.hpp"
#include "SimCoordinator.hpp"
#include "Squirrel.hpp"
#include "PdpEnums.hpp"

extern "C"
{
	#include "pool.h"
}



//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <iostream>
#include <unistd.h>


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
	PoolWorker::PoolWorker(Communicator const& comm)
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
	PoolWorker::~PoolWorker()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}


	void PoolWorker::Run()
	{
		printf("rank %d: running worker\n", m_comm.GetRank());

		int task = -1;
		MPI_Status status;
		MPI_Recv(&task, 1, MPI_INT, MPI_ANY_SOURCE, Pdp::EMpiMsgTag::eAssignTask, m_comm.GetComm(), &status);
		
		switch(task)
	{
		case Pdp::ETask::eSquirrel:
		{
			Biology::Squirrel squirrel;
			do
			{
				squirrel.Update();
			}
			while(! shouldWorkerStop());
			break;
		}
		case Pdp::ETask::eCell:
		{
			Biology::Cell cell;
			do
			{
				cell.Update();
			}
			while(! shouldWorkerStop());
			break;
		}
		case Pdp::ETask::eCoordinator:
		{
			Biology::SimCoordinator sim_cdr;
			do
			{
				sim_cdr.Update();
			}
			while(! shouldWorkerStop());
			break;
		}
	}		
//		//  this loop represents the lifetime of an actor
//		do
//		{
//			for (int i=0; i<4; ++i)
//			{
//				usleep(1000000);
//				std::cout << "sleep " << i << std::endl;
//			}
//
//			shutdownPool();
//		}
//		while (workerSleep());

//		int workerStatus = 1;
//		while (workerStatus)
//		{
//			int myRank, parentId;
//			MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
//			parentId = getCommandData();    // We encode the parent ID in the wake up command data
//			if (parentId == 0)
//			{
//				// If my parent is the master (one of the 10 that the master started) then spawn two further children
//				int childOnePid = startWorkerProcess();
//				int childTwoPid = startWorkerProcess();
//
//				printf("Worker on process %d, started two child processes (%d and %d)\n", myRank, childOnePid, childTwoPid);
//				// Wait for these children to send me a message
//				MPI_Request childRequests[2];
//				MPI_Irecv(NULL, 0, MPI_INT, childOnePid, 0, MPI_COMM_WORLD, &childRequests[0]);
//				MPI_Irecv(NULL, 0, MPI_INT, childTwoPid, 0, MPI_COMM_WORLD, &childRequests[1]);
//				MPI_Waitall(2, childRequests, MPI_STATUS_IGNORE);
//				// Now tell the master that I am finished
//				MPI_Send(NULL, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
//			}
//			else
//			{
//				printf("Child worker on process %d started with parent %d\n", myRank, parentId);
//				// Tell my parent that I have been alive and am about to die
//				MPI_Send(NULL, 0, MPI_INT, parentId, 0, MPI_COMM_WORLD);
//			}
//
//			workerStatus = workerSleep(); // This MPI process will sleep, further workers may be run on this process now
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
//		PoolWorker::PoolWorker(PoolWorker const& rhs)
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
//		PoolWorker& PoolWorker::operator=(PoolWorker const& rhs)
//		{
//			std::cout << __PRETTY_FUNCTION__ << std::endl;
//
//			(void) rhs;
//			return *this;
//		}

}   //  namespace Mpi
