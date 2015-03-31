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
			: m_config(config)
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
			static bool first_time = true;
			
			if (first_time)
			{
				first_time = false;
				CreateInitialActors();
				return;
			}

			std::cout << __PRETTY_FUNCTION__ << std::endl;
			
			//  wait for a second to pass.
			usleep(1000000);
			
			// tmp - shut down sim after 1s
			shutdownPool();
			
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
			std::cout << "coordinator starting " << m_config.GetCells() << " cells and " << m_config.GetSqrls() << " squirrels " << std::endl;
			for (int i=0; i<m_config.GetCells(); ++i)
			{
				int pid = startWorkerProcess();
				std::cout << "started cell on rank " << pid << std::endl;
				int task = Pdp::ETask::eCell;
				MPI_Send(&task, 1, MPI_INT, pid, Pdp::EMpiMsgTag::eAssignTask, m_comm.GetComm());
			}
			
			std::cout << "master starting squirrel workers " << std::endl;
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
