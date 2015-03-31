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
		SimCoordinator::SimCoordinator(Mpi::Communicator const& comm, Pdp::Config const& config)
			: m_comm(comm)
			, m_config(config)
			, m_cell_pids(config.GetCells())

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
		SimCoordinator::~SimCoordinator()
		{
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}


		//////////////////////////////////////////////////////////////////////////////
		/// @details    Handles any new messages.  Sends out day ticks. In charge of 
		///             ending the simulation when appropriate.
		///
		void SimCoordinator::Update()
		{
			std::cout << __PRETTY_FUNCTION__ << std::endl;

			//  start initial set of actors first time we're called.
			static bool first_time = true;
			if (first_time)
			{
				first_time = false;
				CreateInitialActors();
				return;
			}

			static double last_time = MPI_Wtime();
			double now = MPI_Wtime();
			if (now - last_time > 1.0)
			{
				last_time = now;
				std::cout << "coordinator: 1s passed; shutting down pool" << std::endl;

				// tmp - shut down sim after 1s
				shutdownPool();
			}

			//  send a message to all landscape cells at the end of each day.
			
			//  get stats back from landscape cells.

			//  detect end of simulation			
		}

	
		//////////////////////////////////////////////////////////////////////////////
		/// @details    Starts the number of landscape cells and initial squirrels
		///             given in the config.
		///
		void SimCoordinator::CreateInitialActors()
		{
			//  start cells
			std::cout << "coordinator starting " << m_config.GetCells() << " cells" << std::endl;
			for (int cell_id = 0; 
				 cell_id < m_config.GetCells(); 
				 ++cell_id)
			{
				//  store process ID in list using cell ID as index
				m_cell_pids[cell_id] = startWorkerProcess();
				std::cout << "started cell " << cell_id << " on rank " << m_cell_pids[cell_id] << std::endl;
				int task = Pdp::ETask::eCell;
				MPI_Send(&task, 1, MPI_INT, pid, Pdp::EMpiMsgTag::eAssignTask, m_comm.GetComm());
			}
			
			//  start squirrels
			std::cout << "coordinator starting " << m_config.GetSqrls() << " squirrels" << std::endl;
			for (int i=0; i<m_config.GetSqrls(); ++i)
			{
				int pid = startWorkerProcess();
				std::cout << "started squirrel on rank " << pid << std::endl;
				int task = Pdp::ETask::eSquirrel;
				MPI_Send(&task, 1, MPI_INT, pid, Pdp::EMpiMsgTag::eAssignTask, m_comm.GetComm());
			}			
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
//		SimCoordinator::SimCoordinator(SimCoordinator const& rhs)
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
//		SimCoordinator& SimCoordinator::operator=(SimCoordinator const& rhs)
//		{
//			std::cout << __PRETTY_FUNCTION__ << std::endl;
//
//			(void) rhs;
//			return *this;
//		}

}   //  namespace Biology
