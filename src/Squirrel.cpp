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
	#include "squirrel-functions.h"
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
		, m_rng_state(-1 - comm.GetRank())
		, m_x(0.0)
		, m_y(0.0)
		, m_cur_cell(-1)
		, m_cur_step(-1)
		, m_shutdown(false)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;

		//  init random seed as required by biologists' code
		initialiseRNG(&m_rng_state);
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
		if (m_shutdown)
		{
			return false;
		}

		//  do initial setup first time we're called
		static bool first_time = true;
		if (first_time)
		{
			first_time = false;
			HandleFirstUpdate();
		}
		
		Step();
		
		HandleMessages();
		
		/// @todo implement reproduction 

		/// @todo implement dying 

		return true;
	}
	
	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Starts a worker process and tells it to be a squirrel.
	///
	/// @param        comm  MPI communicator.
	///
	void Squirrel::Spawn()
	{		
		std::cout << "rank " << m_comm.GetRank() << ": squirrel reproducing" << std::endl;

		float sq_data[2];
		sq_data[0] = m_x;
		sq_data[1] = m_y;
		
		MPI_Bsend(sq_data, 2, MPI_FLOAT, 1, Pdp::EMpiMsgTag::eSquirrelBirth, m_comm.GetComm());		
	}

	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Informs coordinator of this squirrel's birth.
	///
	void Squirrel::HandleFirstUpdate()
	{
//		//  tell coordinator there's a new squirrel in town
//		/// @todo move to buffered sending
//		std::cout << "rank " << m_comm.GetRank() << " squirrel: sending birth to coordinator" << std::endl;	
//		MPI_Request msg_req;
//		int birth = 1;
//		MPI_Isend(&birth, 1, MPI_INT, 1, Pdp::EMpiMsgTag::eSquirrelLifetime, m_comm.GetComm(), &msg_req);
//		std::cout << "rank " << m_comm.GetRank() << " squirrel: sent birth to coordinator" << std::endl;	
		
		//  start squirrel in a cell
		m_cur_cell = getCellFromPosition(m_x, m_y);
		NotifyCell(m_cur_cell, Pdp::ESquirrelStep::eIn);
	}
	
	
	void Squirrel::HandleMessages()
	{
		//  handle messages by polling
		int msg_waiting = 0;
		do
		{
			MPI_Status msg_status;			
			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, m_comm.GetComm(), &msg_waiting, &msg_status);
			
			if(msg_waiting)
			{
				std::cout << "squirrel " << m_comm.GetRank() << ": message waiting" << std::endl;
				
				switch (msg_status.MPI_TAG)
				{
					case Pdp::EMpiMsgTag::ePoisonPill:
					{
						
						MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, Pdp::EMpiMsgTag::ePoisonPill, m_comm.GetComm(), &msg_status);
						std::cout << "squirrel: poison pill received" << std::endl;
						m_shutdown = true;
						break;
					}
					case Pdp::EMpiMsgTag::ePoolPid:
					case Pdp::EMpiMsgTag::ePoolCtrl:
					{
						//  these will be handled by the pool
						break;
					}
					default:
					{
						//  unrecognised message; fail hard and fast to help diagnosis
						std::cout << "squirrel " << m_comm.GetRank() << ": error: unrecognised message tag: " << msg_status.MPI_TAG << std::endl;
						break;
					}
				}					
			}
			
		} while(msg_waiting);
	}
	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Moves the squirrel one step and updates any cells affected.
	///
	void Squirrel::Step()
	{
		//  take a step
		squirrelStep(m_x, m_y, &m_x, &m_y, &m_rng_state);
		++m_cur_step;

		//  chance of reproduction every 50 steps
		if (0 == m_cur_step % 50)
		{
			/// @todo implement reproduction probability
			if (0 == m_cur_step % 500)
			{
				std::cout << "rank " << m_comm.GetRank() << ": squirrel giving birth" << std::endl;
				Spawn();
			}
			
		}

//		std::cout << "rank " << m_comm.GetRank() << ": squirrel pos: " << m_x << " " << m_y << std::endl;
		
		//  where are we, and have we moved?
		int new_cell = getCellFromPosition(m_x, m_y);		
		if (new_cell != m_cur_cell)
		{
//			std::cout << "rank " << m_comm.GetRank() << ": squirrel moved from cell " << m_cur_cell << " to " << new_cell << std::endl;
		}
		
		//  let interested parties know
		if (new_cell != m_cur_cell)
		{
			NotifyCell(m_cur_cell, Pdp::ESquirrelStep::eOut);
			NotifyCell(new_cell,   Pdp::ESquirrelStep::eIn);
		}
		else
		{
			NotifyCell(m_cur_cell, Pdp::ESquirrelStep::eWithin);
		}

		m_cur_cell = new_cell;
	}

	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Informs coordinator and current land cell that the squirrel
	///               is no more.
	///
	void Squirrel::Die()
	{		
		std::cout << "rank " << m_comm.GetRank() << ": informing coordinator of squirrel death" << std::endl;

		//  tell coordinator "this is an ex-squirrel" /Cleese
		MPI_Request msg_req;
		int birth = -1;
		MPI_Isend(&birth, 1, MPI_INT, 1, Pdp::EMpiMsgTag::eSquirrelLifetime, m_comm.GetComm(), &msg_req);

		std::cout << "rank " << m_comm.GetRank() << ": informed coordinator of squirrel death" << std::endl;
	}

	
	void Squirrel::NotifyCell(int cell, Pdp::ESquirrelStep::ESquirrelStep step)
	{
		MPI_Bsend(&step, 1, MPI_INT, cell + 2, Pdp::EMpiMsgTag::eSquirrelStep, m_comm.GetComm());
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
