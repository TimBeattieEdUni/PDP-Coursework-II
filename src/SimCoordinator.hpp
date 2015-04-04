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
#include "DayTicker.hpp"


//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <vector>


namespace Biology
{
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Coordinates the simulation.
	///
	/// @details    Creates the cells and initial infected squirrels.  Creates new
	///             squirrels on request.  Tracks statistics.  Shuts the 
	///             simulation down if maximum days or maximum squirrels reached.
	///
	/// @note       Only create one of these.
	///
	class SimCoordinator
	{
		public:
			SimCoordinator(Mpi::Communicator const& comm, Config const& config);    ///< Constructor.
			~SimCoordinator();   ///< Destructor.

			bool Update();                          ///< Driver. 
			void CreateInitialActors();             ///< Sets up the landscape and initial squirrels.

		private:
			SimCoordinator(SimCoordinator const& rhs);              ///< Copy constructor.
			SimCoordinator& operator=(SimCoordinator const& rhs);   ///< Assignment operator.
			
			void ReceiveIAmInfectedMsg();          ///< Receives "I am infected" message.
			void ReceiveSquirrelBirthMsg();        ///< Receives "squirrel should be born" message.
			void ReceiveSquirrelDeathMsg();        ///< Receives "squirrel has died" message.

			void KillSquirrels();                  ///< Sends poison pill to all squirrel.
			void SpawnCell();                      ///< Adds a landscape cell to the simulation.
			int SpawnSquirrel(float x, float y);   ///< Gives birth to a squirreal.
		
			Mpi::Communicator const& m_comm;   ///< MPI communcator for the pool.
			Config const& m_config;            ///< App config.

			DayTicker m_ticker;             ///< Keeps track of simulation time.
			unsigned int m_cur_day;         ///< The current day.
			unsigned int m_cur_week;        ///< The current week.
			unsigned int m_num_sq;          ///< Total number of squirrels in the simulation.
			unsigned int m_num_infected;    ///< Total infected squirrels in the simulation.

			bool m_shutdown;                ///< Indicates pool is shutting down.
		
	};

}   //  namespace Biology


#endif  //  #ndef BIOLOGY_SIMCOORDINATOR_HPP
