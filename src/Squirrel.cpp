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
		, m_cur_cell(getCellFromPosition(m_x, m_y))
		, m_cur_step(0)
		, m_infected(false)
		, m_inf_step(-1)
		, m_dead(false)
	{
//		std::cout << __PRETTY_FUNCTION__ << std::endl;

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
//		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}


	bool Squirrel::Update()
	{
		usleep(10000);
		
		//  a squirrel's first step doesn't affect the landscape; take the step and be done
		static bool first_time = true;
		if (first_time)
		{
			first_time = false;
			squirrelStep(m_x, m_y, &m_x, &m_y, &m_rng_state);
			++m_cur_step;
			return true;
		}

		//  do not rise from the dead (just in case higher-level logic gets it wrong)
		if (m_dead)
		{
			return false;
		}

		//  reproduce if we're lucky
		if (0 == m_cur_step % 50)
		{
			float avg_pop = Average(m_last50pop, num_records);
			
			if (willGiveBirth(avg_pop, &m_rng_state))
			{
				GiveBirth();
			}			
		}
		
		//  progress the disease if we have it...
		if (m_infected) 
		{
			if (m_cur_step - m_inf_step > 50)
			{
				if (willDie(&m_rng_state))
				{
					Die();
					return false;
				}
			}
		}
		//  ...or catch it if we're unlucky
		else 
		{
			float avg_inf = Average(m_last50inf, num_records);
			if (willCatchDisease(avg_inf, &m_rng_state))
			{
				BecomeInfected();
			}
		}
		
		//  take a step
		Step();
		
		//  do comms
		HandleMessages();
		
		return true;
	}
	
	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Sets the squirrel's infected status, records the step at 
	///               at which it happened, and informs the coordinator.
	///
	void Squirrel::BecomeInfected()
	{
		m_infected = true;
		m_inf_step = m_cur_step;		

		MPI_Bsend(NULL, 0, MPI_INT, 1, EMpiMsgTag::eIAmInfected, m_comm.GetComm());
	}
	
	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Sets the squirrel's "dead" flag and informs the coordinator.
	///               Includes the squirrel's infection status so the coordinator
	///               can keep track.
	///
	void Squirrel::Die()
	{
		std::cout << "rank " << m_comm.GetRank() << ": squirrel dying at step " << m_cur_step << std::endl;
		
		m_dead = true;
		
		MPI_Bsend(&m_infected, 1, MPI_INT, 1, EMpiMsgTag::eSquirrelDeath, m_comm.GetComm());
	}
	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Asks the coordinator to create a new squirrel.
	///
	void Squirrel::GiveBirth()
	{		
		std::cout << "rank " << m_comm.GetRank() << ": squirrel giving birth at step " << m_cur_step << std::endl;

		float sq_data[2];
		sq_data[0] = m_x;
		sq_data[1] = m_y;
		
		MPI_Bsend(sq_data, 2, MPI_FLOAT, 1, EMpiMsgTag::eSquirrelBirth, m_comm.GetComm());		
	}

	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      
	///
//	void Squirrel::HandleFirstUpdate()
//	{		
//		NotifyCell(m_cur_cell, ESquirrelStep::eIn);
//	}
	
	
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
					case EMpiMsgTag::eYouAreInfected:
					{
						ReceiveInfectMsg();
						break;
					}
					case EMpiMsgTag::eCellStats:
					{
//						ReceiveCellStatsMsg();
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
		
		//  where are we, and have we moved?
		int new_cell = getCellFromPosition(m_x, m_y);
		if ((0 > new_cell) || (m_config.GetCells() <= new_cell))
		{
			std::cout << "rank " << m_comm.GetRank() << ": squirrel: error: getCellFromPosition() returned " << new_cell << std::endl;			
		}
		
		//  let interested parties know

//		std::cout << "rank " << m_comm.GetRank() << ": squirrel: step: cur_cell is " << m_cur_cell << "  new_cell is " << new_cell << std::endl;

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
	/// @details      Informs the given cell that this squirrel has stepped 
	///               in/out/within the cell.
	///
	/// @param        cell  Cell index.  Note that cells don't have IDs; we just use their rank - 2 to distinguish them.
	/// @param        step  The type of step - in, out, or within.
	///
	/// @post         The cell's population and infection levels have been recorded.
	///
	void Squirrel::NotifyCell(int cell, ESquirrelStep::ESquirrelStep step)
	{
//		std::cout << "rank " << m_comm.GetRank() << ": squirrel sending step to cell " << cell << std::endl;

		int sq_data[2];
		
		sq_data[0] = step;
		sq_data[1] = m_infected ? 1 : 0;
		
		int cell_stats[2];
		
		//  blocking send+recv call to ensure statistics are accurate before moving on
		
		//MPI_Bsend(sq_data, 2, MPI_INT, cell + 2, EMpiMsgTag::eSquirrelStep, m_comm.GetComm());
		std::cout << "rank " << m_comm.GetRank() << ": squirrel start sendrecv with cell " << cell << std::endl;
		MPI_Status msg_status;			
		MPI_Sendrecv(sq_data,    2, MPI_INT, cell + 2, EMpiMsgTag::eSquirrelStep, 
					 cell_stats, 2, MPI_INT, cell + 2, EMpiMsgTag::eCellStats, 
					 m_comm.GetComm(), &msg_status);
		std::cout << "rank " << m_comm.GetRank() << ": squirrel done  sendrecv with cell " << cell << std::endl;

		//  update records
		int cell_pop = cell_stats[0];
		int cell_inf = cell_stats[1];
		
		m_last50pop[m_last50index] = cell_pop;
		m_last50inf[m_last50index] = cell_inf;
		
		++m_last50index %= num_records;
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
		MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, EMpiMsgTag::eYouAreInfected, m_comm.GetComm(), &msg_status);
		
//		std::cout << "rank " << m_comm.GetRank() << ": squirrel: received infect message" << std::endl;

		BecomeInfected();
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
		
//		std::cout << "rank " << m_comm.GetRank() << ": squirrel: cell stats msg rx from rank " << msg_status.MPI_SOURCE << ": " << cell_pop << " " << cell_inf << std::endl;

		m_last50pop[m_last50index] = cell_pop;
		m_last50inf[m_last50index] = cell_inf;
		
		++m_last50index %= num_records;
	}

	//////////////////////////////////////////////////////////////////////////////
	/// @details      Calculates the average of the values in the given array.
	///
	/// @param        values  Array of values to average.
	/// @param        count   Number of values in the array.
	/// @return       The average value.
	///
	float Squirrel::Average(int* values, int count)
	{
		int total = 0; 
		for (int i = 0; i < num_records; ++i)
		{
			total += values[i];
		}
		
		return (float)total / (float)count;
	}
	
}   //  namespace Biology
