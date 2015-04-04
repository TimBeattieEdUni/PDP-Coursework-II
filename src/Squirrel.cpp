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
	Squirrel::Squirrel(Mpi::Communicator const& comm, Config const& config)
		: m_last50pop()
		, m_last50inf()
		, m_last50index(0)
		, m_comm(comm)
		, m_config(config)
		, m_rng_state(-1 - comm.GetRank())
		, m_x(0.0)
		, m_y(0.0)
		, m_cur_cell(-1)
		, m_cur_step(-1)
		, m_infected(false)
		, m_inf_step(-1)
		, m_dead(false)
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
		//  do initial setup first time we're called
		static bool first_time = true;
		if (first_time)
		{
			first_time = false;
			HandleFirstUpdate();
		}

		if (m_dead)
		{
			return false;
		}

		if (m_infected)
		{
			if (m_cur_step - m_inf_step > 50)
			{
				if (willDie(&m_rng_state))
				{
					std::cout << "rank " << m_comm.GetRank() << ": squirrel dying at step " << m_cur_step << std::endl;
					m_dead = true;
					return false;
				}
			}
		}

		Step();
		
		HandleMessages();
		
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
		std::cout << "rank " << m_comm.GetRank() << ": squirrel reproducing at step " << m_cur_step << std::endl;

		float sq_data[2];
		sq_data[0] = m_x;
		sq_data[1] = m_y;
		
		MPI_Bsend(sq_data, 2, MPI_FLOAT, 1, EMpiMsgTag::eSquirrelBirth, m_comm.GetComm());		
	}

	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Informs coordinator of this squirrel's birth.
	///
	void Squirrel::HandleFirstUpdate()
	{
		std::cout << "rank " << m_comm.GetRank() << ": squirrel: first update: cell was " << m_cur_cell << std::endl;
		
		//  start squirrel in a cell
		m_cur_cell = getCellFromPosition(m_x, m_y);

		std::cout << "rank " << m_comm.GetRank() << ": squirrel: first update: cell is now " << m_cur_cell << std::endl;

		NotifyCell(m_cur_cell, ESquirrelStep::eIn);
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
//				std::cout << "rank " << m_comm.GetRank() << ": squirrel: message waiting" << std::endl;
				
				switch (msg_status.MPI_TAG)
				{
					case EMpiMsgTag::eInfect:
					{
						ReceiveInfectMsg();
						break;
					}
					case EMpiMsgTag::eCellStats:
					{
						ReceiveCellStatsMsg();
						break;
					}
					case EMpiMsgTag::ePoisonPill:
					{
						
						MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, EMpiMsgTag::ePoisonPill, m_comm.GetComm(), &msg_status);
						std::cout << "rank " << m_comm.GetRank() << ": squirrel: poison pill received" << std::endl;
						m_dead = true;
						shutdownPool();
						break;
					}
					case EMpiMsgTag::ePoolPid:
					case EMpiMsgTag::ePoolCtrl:
					{
						//  these will be handled by the pool
						break;
					}
					default:
					{
						//  unrecognised message; fail hard and fast to help diagnosis
						std::cout << "rank " << m_comm.GetRank() << ": squirrel: error: msg from rank " << msg_status.MPI_SOURCE << " with unrecognised message tag: " << msg_status.MPI_TAG << "; exiting" << std::endl;
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
		if (49 == m_cur_step % 50)
		{
			/// @todo implement reproduction probability
			if (4999 == m_cur_step % 5000)
			{
				std::cout << "rank " << m_comm.GetRank() << ": squirrel giving birth at step " << m_cur_step << std::endl;
				//Spawn();
			}
			
		}
		
		//  where are we, and have we moved?
		int new_cell = getCellFromPosition(m_x, m_y);
		if ((0 > new_cell) || (m_config.GetCells() <= new_cell))
		{
			std::cout << "rank " << m_comm.GetRank() << ": squirrel: error: getCellFromPosition returned " << new_cell << std::endl;			
		}
		
		//  let interested parties know

		std::cout << "rank " << m_comm.GetRank() << ": squirrel: step: cur_cell is " << m_cur_cell << "  new_cell is " << new_cell << std::endl;

		if (new_cell != m_cur_cell)
		{
			NotifyCell(m_cur_cell, ESquirrelStep::eOut);
			NotifyCell(new_cell,   ESquirrelStep::eIn);
		}
		else
		{
			NotifyCell(m_cur_cell, ESquirrelStep::eWithin);
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

		MPI_Bsend(NULL, 0, MPI_INT, 1, EMpiMsgTag::eSquirrelDeath, m_comm.GetComm());

		std::cout << "rank " << m_comm.GetRank() << ": informed coordinator of squirrel death" << std::endl;
	}

	
	void Squirrel::NotifyCell(int cell, ESquirrelStep::ESquirrelStep step)
	{
//		std::cout << "rank " << m_comm.GetRank() << ": squirrel sending step to cell " << cell << std::endl;

		int sq_data[2];
		
		sq_data[0] = step;
		sq_data[1] = m_infected ? 1 : 0;
		
		MPI_Bsend(sq_data, 2, MPI_INT, cell + 2, EMpiMsgTag::eSquirrelStep, m_comm.GetComm());
	}

	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Retrieves and handles an "infect" message from the 
	///               coordinator early in the simulation.
	///
	/// @post         The squirrel is infected.
	///
	/// @note         The coordinator uses this to create the initial infected 
	///               squirrels.  The squirrel might take several steps at the 
	///               start of the simulation before this message arrives.  This 
	///               is considered an acceptable innacuracy.
	///
	void Squirrel::ReceiveInfectMsg()
	{
		MPI_Status msg_status;			
		MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, EMpiMsgTag::eInfect, m_comm.GetComm(), &msg_status);
		
		std::cout << "rank " << m_comm.GetRank() << ": squirrel: received infect message" << std::endl;
		
		m_infected = true;
		m_inf_step = m_cur_step;		
	}
	
	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Retrieves and handles a "Cell Statitics" message from a 
	///               cell.  Squirrels keep stats on the last 50 cells they visit 
	///               in circular buffers.
	///
	/// @note         These messages may come in in any order; it's assumed that 
	///               having 50 of them will smooth this out to an acceptable
	///               accuracy.
	///
	void Squirrel::ReceiveCellStatsMsg()
	{
		int cell_stats[2];
		MPI_Status msg_status;			
		MPI_Recv(cell_stats, 2, MPI_INT, MPI_ANY_SOURCE, EMpiMsgTag::eCellStats, m_comm.GetComm(), &msg_status);
		
		int cell_pop =  cell_stats[0];
		int cell_inf = cell_stats[1];
		
		std::cout << "rank " << m_comm.GetRank() << ": squirrel: cell stats msg rx from rank " << msg_status.MPI_SOURCE << ": " << cell_pop << " " << cell_inf << std::endl;

		m_last50pop[m_last50index] = cell_pop;
		m_last50inf[m_last50index] = cell_inf;
		
		++m_last50index %= num_records;
	}
	
}   //  namespace Biology
