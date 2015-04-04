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
#include "PdpEnums.hpp"

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
	PoolMaster::PoolMaster(Communicator const& comm, Biology::Config const& config)
		: m_comm(comm)
		, m_config(config)
	{
//		std::cout << __PRETTY_FUNCTION__ << std::endl;
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
//		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}


	//////////////////////////////////////////////////////////////////////////////
	/// @details    Runs the pool's master process.
	///
	void PoolMaster::Run()
	{
		int pid = startWorkerProcess();
//		std::cout << "rank " << m_comm.GetRank() << ": started process for coordinator on rank " << pid << std::endl;
		
		int task = Biology::ETask::eCoordinator;
		MPI_Send(&task, 1, MPI_INT, pid, Biology::EMpiMsgTag::eAssignTask, m_comm.GetComm());
		
		while(masterPoll())
		{
//			std::cout << "pool master woken by pool" << std::endl;
		}

	}

}   //  namespace Mpi
