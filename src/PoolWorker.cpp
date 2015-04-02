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
	PoolWorker::PoolWorker(Communicator const& comm, Pdp::Config const& config)
		: m_comm(comm)
		, m_config(config)
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


	//////////////////////////////////////////////////////////////////////////////
	/// @details    Called once in the lifetime of a process pool worker.  Finds 
	///             out which type of Actor to be, then creates one of that type
	///             and runs it until it dies.  Then loops and waits for the next 
	///             one.  
	///
	void PoolWorker::Run()
	{
		std::cout << "rank " << m_comm.GetRank() << " waiting for assignment message" << std::endl;

		do
		{
			int task = -1;
			MPI_Status status;
			MPI_Recv(&task, 1, MPI_INT, MPI_ANY_SOURCE, Pdp::EMpiMsgTag::eAssignTask, m_comm.GetComm(), &status);
			std::cout << "rank " << m_comm.GetRank() << " received assignment message: " << task << std::endl;
			
			switch(task)
			{
				case Pdp::ETask::eSquirrel:
				{
					Biology::Squirrel squirrel(m_comm);
					
					bool squirrel_ok = true;
					bool pool_ok     = true;
					
					while(squirrel_ok && pool_ok)
					{
						squirrel_ok = squirrel.Update();
						pool_ok     = ! shouldWorkerStop();
					}
					break;
				}
				case Pdp::ETask::eCell:
				{
					Biology::Cell cell(m_comm, m_config);
					
					bool cell_ok = true;
					bool pool_ok = true;

					while(cell_ok && pool_ok)
					{
						cell_ok = cell.Update();
						pool_ok = ! shouldWorkerStop();
					}
					break;
				}
				case Pdp::ETask::eCoordinator:
				{
					Biology::SimCoordinator sim_coord(m_comm, m_config);

					bool coord_ok = true;
					bool pool_ok  = true;

					while(coord_ok && pool_ok)
					{
						coord_ok = sim_coord.Update();
						pool_ok  = ! shouldWorkerStop();
					}
					break;
				}
			}

		} while (workerSleep());

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
