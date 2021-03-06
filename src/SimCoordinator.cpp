//////////////////////////////////////////////////////////////////////////////
/// @file     SimCoordinator.cpp
///
/// @brief    Implementation of class SimCoordinator.
///


//////////////////////////////////////////////////////////////////////////////
//  This class's header.
#include "SimCoordinator.hpp"


//////////////////////////////////////////////////////////////////////////////
//  Local headers.
#include "PdpEnums.hpp"
#include "Squirrel.hpp"

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
	/// @param      config  Simulation configuration.
	///
	/// @post       List what is guaranteed to be true after this function returns.
	///
	/// @exception  List exceptions this function may throw here.
	///
	SimCoordinator::SimCoordinator(Mpi::Communicator const& comm, Config const& config)
		: m_comm(comm)
		, m_config(config)
		, m_ticker(config.GetDayLen())
		, m_cur_day(0)
		, m_cur_week(0)
		, m_num_sq(0)
		, m_num_infected(0)
		, m_shutdown(false)

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
	SimCoordinator::~SimCoordinator()
	{
//		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}


	//////////////////////////////////////////////////////////////////////////////
	/// @details    Handles any new messages.  Sends out day ticks. In charge of 
	///             ending the simulation when appropriate.
	///
	/// @return     True if the coordinator wants to keep running; false otherwise.
	///
	bool SimCoordinator::Update()
	{
//			std::cout << __PRETTY_FUNCTION__ << std::endl;

		if (m_shutdown)
		{
			return false;
		}

		//  do initial setup first time we're called
		static bool first_time = true;
		if (first_time)
		{
//			std::cout << "coordinator: first update" << std::endl;
			first_time = false;
			CreateInitialActors();
			return true;
		}
		
		//  do "new day" events
		unsigned int today = m_ticker.GetDay();
		if (today > m_cur_day)
		{
//			std::cout << "coordinator: day " << today << std::endl;
			
//			/// @todo remove this - artificially maintaining the number of squirrels
//			if (m_num_sq < 40)
//			{
//				for (int i = m_num_sq; i < 30; ++i)
//				{
//					SpawnSquirrel(0, 0);
//				}
//			}

			//  shut down sim after configured number of days
			if (today >= m_config.GetSimLen())
			{
				std::cout << "coordinator: max days reached; shutting down" << std::endl;
				KillSquirrels();
				KillCells();
				shutdownPool();
				return false;
			}
			
			//  print stats at the end of each week
			int this_week = today / 7;
			if (this_week > m_cur_week)
			{
				m_cur_week = this_week;
				
				std::cout << "coordinator: week " << this_week << ": total squirrels: " << m_num_sq << "  infected: " << m_num_infected << std::endl;
			}

			m_cur_day = today;
			return true;
		}


		//  handle messages by polling
		int msg_waiting = 0;
		do
		{
			MPI_Status msg_status;			
			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, m_comm.GetComm(), &msg_waiting, &msg_status);
			
			if(msg_waiting)
			{
//					std::cout << "coordinator: message waiting" << std::endl;

				switch (msg_status.MPI_TAG)
				{
					case EMpiMsgTag::eCellStats:
					{
						int sq_data[2];
						MPI_Recv(sq_data, 2, MPI_INT, MPI_ANY_SOURCE, EMpiMsgTag::eCellStats, m_comm.GetComm(), &msg_status);
						
						int pop_inf   = sq_data[0];
						int inf_level = sq_data[1];
						std::cout << "cell " << msg_status.MPI_SOURCE - 2 << ":  populationInflux: " << pop_inf << "  infectionLevel: " << inf_level << std::endl;
						break;
					}
						
					case EMpiMsgTag::eSquirrelBirth:
					{
						ReceiveSquirrelBirthMsg();
						break;							
					}
					
					case EMpiMsgTag::eSquirrelDeath:
					{
						ReceiveSquirrelDeathMsg();
						break;							
					}
					
					case EMpiMsgTag::eIAmInfected:
					{
						ReceiveIAmInfectedMsg();
						break;
					}
					case EMpiMsgTag::ePoolPid:
					case EMpiMsgTag::ePoolCtrl:
					{
						//  these will be handled by the pool
						break;;
					}
					default:
					{
						//  unrecognised message; fail hard and fast to help diagnosis
						std::cout << "coordinator: error: msg from rank " << msg_status.MPI_SOURCE << " with unrecognised message tag: " << msg_status.MPI_TAG << "; exiting" << std::endl;
						return false;
					}
				}					
			}
			
		} while(msg_waiting);
			
		return true;
	}


	//////////////////////////////////////////////////////////////////////////////
	/// @details    Starts the number of landscape cells and initial squirrels
	///             given in the config.
	///
	void SimCoordinator::CreateInitialActors()
	{
//		std::cout << __PRETTY_FUNCTION__ << std::endl;

		//  start cells
		std::cout << "coordinator starting " << m_config.GetCells() << " cells" << std::endl;
		for (int cell_id = 0; 
			 cell_id < m_config.GetCells(); 
			 ++cell_id)
		{
			SpawnCell();
		}
		
		//  start initial squirrels
		std::cout << "coordinator starting " << m_config.GetIniSqrls() << " infected squirrels" << std::endl;
		for (int i=0; i<m_config.GetIniSqrls(); ++i)
		{
			int pid = SpawnSquirrel(0.0, 0.0);
			
			//  first squirrels are infected from the start
			MPI_Bsend(NULL, 0, MPI_INT, pid, EMpiMsgTag::eYouAreInfected, m_comm.GetComm());			
		}			
	}

	
	void SimCoordinator::SpawnCell()
	{
		int pid = startWorkerProcess();
		
		int task = ETask::eCell;

		//  blocking async send to make sure cells are all started first
		MPI_Ssend(&task, 1, MPI_INT, pid, EMpiMsgTag::eAssignTask, m_comm.GetComm());
//		MPI_Bsend(&task, 1, MPI_INT, pid, EMpiMsgTag::eAssignTask, m_comm.GetComm());			

//		std::cout << "coordinator: started cell on rank " << pid << std::endl;
}
	

	int SimCoordinator::SpawnSquirrel(float x, float y)
	{
		int pid = startWorkerProcess();

		++m_num_sq;

		//  tell the worker process to run a squirrel
		int task = ETask::eSquirrel;
		MPI_Bsend(&task, 1, MPI_INT, pid, EMpiMsgTag::eAssignTask, m_comm.GetComm());			

//		std::cout << "rank " << m_comm.GetRank() << ": gave birth to squirrel on rank " << pid << std::endl;
		
		return pid;
	}


	//////////////////////////////////////////////////////////////////////////////
	/// @details      Retrieves and handles an "I'm giving birth" message from a
	///               squirrel.  Coordinator does all births so it can know if 
	///               max squirrels is exceeded and end the simulation.
	///
	void SimCoordinator::ReceiveSquirrelBirthMsg()
	{
//		std::cout << __PRETTY_FUNCTION__ << std::endl;

		MPI_Status msg_status;
		float sq_data[2];
		MPI_Recv(sq_data, 2, MPI_INT, MPI_ANY_SOURCE, EMpiMsgTag::eSquirrelBirth, m_comm.GetComm(), &msg_status);

		if (m_num_sq >= m_config.GetMaxSqrls())
		{
			std::cout << "coordinator: max squirrels exceeded; shutting down" << std::endl;
			m_shutdown = true;
			KillSquirrels();
			KillCells();
			shutdownPool();
			return;
		}
			
		float x = sq_data[0];
		float y = sq_data[1];

		SpawnSquirrel(x, y);
	}
	
	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Updates the total squirrels and the total infected squirrels
	///               in the system.  Detects "no more squirrels" and shuts down 
	///               the simulation.
	///
	void SimCoordinator::ReceiveSquirrelDeathMsg()
	{
//		std::cout << __PRETTY_FUNCTION__ << std::endl;
		
		int infected;
		
		MPI_Status msg_status;
		MPI_Recv(&infected, 1, MPI_INT, MPI_ANY_SOURCE, EMpiMsgTag::eSquirrelDeath, m_comm.GetComm(), &msg_status);
		
		--m_num_sq;
		if (0 == m_num_sq)
		{
			std::cout << "coordinator: no more squirrels; shutting down on day " << m_cur_day << std::endl;

			m_shutdown = true;
			shutdownPool();
		}
		
		--m_num_infected;
	}

	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Retreives an "I am infected" message from a squirrel. 
	///               Updates the number of infected squirrels in the simulation.
	///
	void SimCoordinator::ReceiveIAmInfectedMsg()
	{
//		std::cout << __PRETTY_FUNCTION__ << std::endl;
		
		MPI_Status msg_status;
		MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, EMpiMsgTag::eIAmInfected, m_comm.GetComm(), &msg_status);
		++m_num_infected;		
	}

	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Sends poison pill to all squirrels.
	///
	void SimCoordinator::KillSquirrels()
	{
		std::cout << "coordinator: killing squirrels " << std::endl;

		for (int pid = 2 + m_config.GetCells(); 
			 pid < m_comm.GetSize();
			 ++pid)
		{
			MPI_Bsend(NULL, 0, MPI_INT, pid, EMpiMsgTag::ePoisonPill, m_comm.GetComm());
		}
	}
	
	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Sends poison pill to all squirrels.
	///
	void SimCoordinator::KillCells()
	{
		std::cout << "coordinator: killing cells " << std::endl;

		for (int cell = 0; 
			 cell < m_config.GetCells();
			 ++cell)
		{
			//  blocking synchronous call as a first try
			MPI_Ssend(NULL, 0, MPI_INT, cell + 2, EMpiMsgTag::ePoisonPill, m_comm.GetComm());
		}
	}
	
}   //  namespace Biology
