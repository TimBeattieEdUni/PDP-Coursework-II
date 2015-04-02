//////////////////////////////////////////////////////////////////////////////
/// @file     Squirrel.cpp
///
/// @brief    Implementation of class Squirrel.
///


//////////////////////////////////////////////////////////////////////////////
//  This class's header.
#include "Squirrel.hpp"


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
#include <unistd.h>


namespace Biology
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
	Squirrel::Squirrel(Mpi::Communicator const& comm)
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
	Squirrel::~Squirrel()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}


	bool Squirrel::Update()
	{
		//  @todo remove this
		usleep(1000000);
		
		//  do initial setup first time we're called
		static bool first_time = true;
		if (first_time)
		{
			first_time = false;
			
			//  tell coordinator there's a new squirrel in town
			std::cout << "rank " << m_comm.GetRank() << " first update; sending birth record to coordinator" << std::endl;	

			//  we aren't concerned with whether this message is received
			MPI_Request msg_req;
			int birth = 1;
			MPI_Isend(&birth, 1, MPI_INT, 1, Pdp::EMpiMsgTag::eSquirrelLifetime, m_comm.GetComm(), &msg_req);

			std::cout << "rank " << m_comm.GetRank() << " first update; sent birth record to coordinator" << std::endl;	
			return true;
		}
		
		//  @todo remove this
		static int age = 0;
		++age;
		if (8 < age)
		{
			//  tell coordinator "this is an ex-squirrel" /Cleese
			MPI_Request msg_req;
			int birth = -1;
			MPI_Isend(&birth, 1, MPI_INT, 1, Pdp::EMpiMsgTag::eSquirrelLifetime, m_comm.GetComm(), &msg_req);
			return false;
		}

		/// @todo remove this
		Spawn(m_comm);

		return true;
	}
	
	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Starts a worker process and tells it to be a squirrel.
	///
	/// @param        comm  MPI communicator.
	///
	void Squirrel::Spawn(Mpi::Communicator const& comm)
	{
		//  @todo this is a kludge; sometimes we're trying to give birth when the sim is over.
		if (shouldWorkerStop())
		{
			return;
		}
		
		std::cout << "rank " << comm.GetRank() << ": squirrel giving birth" << std::endl;

		int pid = startWorkerProcess();
		
		std::cout << "rank " << comm.GetRank() << ": started worker for newborn squirrel: pid " << pid << std::endl;

		int task = Pdp::ETask::eSquirrel;
		MPI_Request msg_req;
		MPI_Isend(&task, 1, MPI_INT, pid, Pdp::EMpiMsgTag::eAssignTask, comm.GetComm(), &msg_req);

		std::cout << "rank " << comm.GetRank() << ": gave birth to squirrel on rank " << pid << std::endl;
	}

	
//	//////////////////////////////////////////////////////////////////////////////
//	/// @details    Describe copy construction here.
//	///
//	/// @param      rhs  Object to copy.
//	///
//	/// @pre        List what must be true before this function is called.
//	/// @post       List what is guaranteed to be true after this function returns.
//	///
//	/// @exception  List exceptions this function may throw here.
//	///
//	Squirrel::Squirrel(Squirrel const& rhs)
//	{
//		std::cout << __PRETTY_FUNCTION__ << std::endl;
//
//		(void) rhs;
//	}
//
//
//	//////////////////////////////////////////////////////////////////////////////
//	/// @details    Describe object assignment here.
//	///
//	/// @param      rhs  Object on the right-hand side of the assignment statement.
//	/// @return     Object which has been assigned.
//	///
//	/// @pre        List what must be true before this function is called.
//	/// @post       List what is guaranteed to be true after this function returns.
//	///
//	/// @exception  List exceptions this function may throw here.
//	///
//	Squirrel& Squirrel::operator=(Squirrel const& rhs)
//	{
//		std::cout << __PRETTY_FUNCTION__ << std::endl;
//
//		(void) rhs;
//		return *this;
//	}

}   //  namespace Biology
