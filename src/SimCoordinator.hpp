//////////////////////////////////////////////////////////////////////////////
/// @file     SimCoordinator.hpp
///
/// @brief    Interface declaration for class SimCoordinator.
///


#ifndef BIOLOGY_SIMCOORDINATOR_HPP
#define BIOLOGY_SIMCOORDINATOR_HPP


//////////////////////////////////////////////////////////////////////////////
//  Local headers.
#include "Config.hpp"
#include "MpiCommunicator.hpp"


namespace Biology
{
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Manages the simulation.  Provides a day ticker to landscape cells.
	///
	/// @details
	///
	/// @note       You should probably only create one of these.
	///
	class SimCoordinator
	{
		public:
			SimCoordinator(Mpi::Communicator const& comm, Pdp::Config const& config);    ///< Constructor.
			~SimCoordinator();   ///< Destructor.

			void Update();       ///< Runs the Coordinator's part of the simulation.
			void CreateInitialActors();   ///< Sets up the landscape and initial squirrels.

	private:
			SimCoordinator(SimCoordinator const& rhs);              ///< Copy constructor.
			SimCoordinator& operator=(SimCoordinator const& rhs);   ///< Assignment operator.
			
			Mpi::Communicator const& m_comm;   ///< MPI communcator for the pool.
			Pdp::Config const& m_config;   ///< App config.
	};

}   //  namespace Biology


#endif  //  #ndef BIOLOGY_SIMCOORDINATOR_HPP