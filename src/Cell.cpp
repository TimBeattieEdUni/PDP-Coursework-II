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
	Cell::Cell(Mpi::Communicator const& comm, Pdp::Config const& config)
		: m_comm(comm)
		, m_config(config)
		, m_ticker(config.GetDayLen())
		, m_cur_day(0)
		, m_num_sq(0)
		
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
		//  detect new day
		unsigned int today = m_ticker.GetDay();

		if (today > m_cur_day)
		{
			//  if more than one day has passed, stats for all will be sent, but this is acceptable.
			std::cout << "cell: day " << m_cur_day << " complete; sending stats to coordinator" << std::endl;
			
			//  we aren't concerned with whether this message is received
			MPI_Request msg_req;
			MPI_Isend(&m_num_sq, 1, MPI_INT, 1, Pdp::EMpiMsgTag::eCellStats, m_comm.GetComm(), &msg_req);

			//  @todo: this shouldn't be needed; coordinator shuts down pool but cells carry on.
			if (today > m_config.GetSimLen())
			{
				std::cout << "rank " << m_comm.GetRank() << ": max sim days reached; exiting" << std::endl;	
				shutdownPool();
				return false;
			}
			
			//  after all the day's work is done, we start a new day
			m_cur_day = today;
		}
		
		//  handle messages by polling
		int msg_waiting = 0;
		do
		{
			MPI_Status msg_status;			
			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, m_comm.GetComm(), &msg_waiting, &msg_status);
			
			if(msg_waiting)
			{
				std::cout << "cell " << m_comm.GetRank() << ": message waiting" << std::endl;
				
				switch (msg_status.MPI_TAG)
				{
					case Pdp::EMpiMsgTag::eSquirrelStep:
					{
						std::cout << "cell " << m_comm.GetRank() << ": squirrel step msg rxd" << std::endl;
						Pdp::ESquirrelStep::ESquirrelStep step;
						MPI_Recv(&step, 1, MPI_INT, MPI_ANY_SOURCE, Pdp::EMpiMsgTag::eSquirrelStep, m_comm.GetComm(), &msg_status);
						std::cout << "cell " << m_comm.GetRank() << ": squirrel step rxd: " << step << std::endl;
						break;

					}
					case Pdp::EMpiMsgTag::ePoolPid:
					case Pdp::EMpiMsgTag::ePoolCtrl:
					{
						//  these will be handled by the pool
						return true;
					}
					default:
					{
						//  unrecognised message; fail hard and fast to help diagnosis
						std::cout << "cell " << m_comm.GetRank() << ": error: unrecognised message tag: " << msg_status.MPI_TAG << "; exiting" << std::endl;
						return false;
					}
				}					
			}
			
		} while(msg_waiting);
	

		return true;
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
//	Cell::Cell(Cell const& rhs)
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
//	Cell& Cell::operator=(Cell const& rhs)
//	{
//		std::cout << __PRETTY_FUNCTION__ << std::endl;
//
//		(void) rhs;
//		return *this;
//	}

}   //  namespace Biology
