//////////////////////////////////////////////////////////////////////////////
/// @file     Cell.cpp
///
/// @brief    Implementation of class Cell.
///


//////////////////////////////////////////////////////////////////////////////
//  This class's header.
#include "Cell.hpp"


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
	Cell::Cell(Mpi::Communicator const& comm, Config const& config)
		: m_comm(comm)
		, m_config(config)
		, m_ticker(config.GetDayLen())
		, m_cur_day(0)
		, m_cur_week(0)
//		, m_num_sq(0)
		, m_sq_steps1(0)
		, m_sq_steps2(0)
		, m_sq_steps3(0)
		, m_inf_steps1(0)
		, m_inf_steps2(0)
		, m_inf_steps3(0)
		
	{
		std::cout << __PRETTY_FUNCTION__ <<  std::endl;
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
	Cell::~Cell()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}


	//////////////////////////////////////////////////////////////////////////////
	/// @details    Updates the state of the cell.
	///
	/// @return     True if the cell is ok, false otherwise.
	///
	/// @note       Currently always returns true, but consistent with other Actors' interfaces.
	///
	bool Cell::Update()
	{
//		std::cout << __PRETTY_FUNCTION__ << std::endl;

		//  detect new day
		unsigned int today = m_ticker.GetDay();
				
		if (today > m_cur_day)
		{
//			std::cout << "rank " << m_comm.GetRank() << ": cell: day " << today << std::endl;

			//  if more than one day has passed, stats for multiple days will be included, but this is acceptable.

			//  print stats at the end of each week
			int this_week = today / 7;
			if (this_week > m_cur_week)
			{
				//  update the coordinator
				SendStatistics(1);

				m_cur_week = this_week;
				
				std::cout << "rank " << m_comm.GetRank() << ": cell: week " << this_week 
						  << ": pop influx: " << m_sq_steps1 + m_sq_steps2 + m_sq_steps3 
				          << "  infection level: " << m_inf_steps1 + m_inf_steps2 + m_inf_steps3 
				          << std::endl;
			}

			//  ensure all cells stop on the right day (after printing stats for previous day/week)
			if (today >= m_config.GetSimLen())
			{
				std::cout << "rank " << m_comm.GetRank() << ": cell: max days reached; exiting " << today << std::endl;
				shutdownPool();
				return false;
			}

			//  after all the day's work is done, we start a new day
			m_cur_day = today;
			BumpStatistics();
		}
		
		//  handle messages by polling
		int msg_waiting = 0;
		do
		{
			MPI_Status msg_status;			
			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, m_comm.GetComm(), &msg_waiting, &msg_status);
			
			if(msg_waiting)
			{				
				switch (msg_status.MPI_TAG)
				{
					case EMpiMsgTag::eSquirrelStep:
					{
						ReceiveSquirrelStep();
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
						std::cout << "rank " << m_comm.GetRank() << ": cell: error: unrecognised message tag: " << msg_status.MPI_TAG << "; exiting" << std::endl;
						return false;
					}
				}					
			}
			
		} while(msg_waiting);
	
		return true;
	}

	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Shifts the set of 3 days worth of statistics along by one.
	///
	void Cell::BumpStatistics()
	{
//		std::cout << "rank " << m_comm.GetRank() << ": bump: " << m_inf_steps1 << " " << m_inf_steps2 << " " << m_inf_steps3 << std::endl;

		m_sq_steps3 = m_sq_steps2;
		m_sq_steps2 = m_sq_steps1;
		m_sq_steps1 = 0;
		
		m_inf_steps3 = m_inf_steps2;
		m_inf_steps2 = m_inf_steps1;
		m_inf_steps1 = 0;
	}
	
	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Calculates population influx and infection level and sends
	///               them to the given pid.
	///
	void Cell::SendStatistics(int pid)
	{	
		int sq_data[2];
		sq_data[0] = m_sq_steps1  + m_sq_steps2  + m_sq_steps3;
		sq_data[1] = m_inf_steps1 + m_inf_steps2 + m_inf_steps3;
		
		MPI_Bsend(sq_data, 2, MPI_INT, pid, EMpiMsgTag::eCellStats, m_comm.GetComm());
	}	

	
	void Cell::ReceiveSquirrelStep()
	{
//		std::cout << __PRETTY_FUNCTION__ << std::endl;
				
		int sq_data[2];
		MPI_Status msg_status;
		MPI_Recv(sq_data, 2, MPI_INT, MPI_ANY_SOURCE, EMpiMsgTag::eSquirrelStep, m_comm.GetComm(), &msg_status);

		int step = sq_data[0];
		bool infected = (bool)sq_data[1];
		
		switch(step)
		{
			case ESquirrelStep::eIn:
			{
				++m_sq_steps1;

				if (infected)
				{
					++m_inf_steps1;
				}
				break;
			}
			case ESquirrelStep::eOut:
			{
				break;
			}
			case ESquirrelStep::eWithin:
			{
				++m_sq_steps1;				

				if (infected)
				{
					++m_inf_steps1;
				}
				break;
			}
		}
	}

}   //  namespace Biology
