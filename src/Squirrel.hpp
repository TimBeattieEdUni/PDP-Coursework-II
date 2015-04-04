//////////////////////////////////////////////////////////////////////////////
/// @file     Squirrel.hpp
///
/// @brief    Interface declaration for class Squirrel.
///


#ifndef BIOLOGY_SQUIRREL_HPP
#define BIOLOGY_SQUIRREL_HPP


//////////////////////////////////////////////////////////////////////////////
//  Local headers.
#include "Config.hpp"
#include "MpiCommunicator.hpp"
#include "PdpEnums.hpp"


namespace Biology
{
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Represents a squirrel.
	///
	class Squirrel
	{
		public:
			Squirrel(Mpi::Communicator const& comm,  Config const& config);   ///< Constructor.
			~Squirrel();   ///< Destructor.

			bool Update();   ///< Driver.
		
		private:
			Squirrel(Squirrel const& rhs);              ///< Copy constructor.
			Squirrel& operator=(Squirrel const& rhs);   ///< Assignment operator.

			void GiveBirth();           ///< Gives birth to a squirrel.
			void HandleFirstUpdate();   ///< Does the first update.      
			void HandleMessages();      ///< Handles incoming MPI messages.
			void Step();                ///< Moves this squirrel one step.	

			void ReceiveCellStatsMsg();   ///< Retrieves and handles a "Cell Statistics" message.
			void ReceiveInfectMsg();      ///< Retrieves and handles an "infect" message.
		
			/// Tells a cell we've stepped.
			void NotifyCell(int cell, ESquirrelStep::ESquirrelStep step);

			/// Squrrel biths and deaths are affected by this many recently-visited cells.
			static int const num_records = 50;
		
			int m_last50pop[num_records];    ///< Cell population influx values from the last 50 steps.
			int m_last50inf[num_records];    ///< Cell infection level values from the last 50 steps.
			int m_last50index;               ///< Index into the "last n-many" arrays.
		
			Mpi::Communicator const& m_comm;   ///< MPI communcator for the pool.
			Config const& m_config;            ///< App config.
		
			long m_rng_state;   ///< Random number generator state (required by biologists' code).
			
			float m_x;          ///< X-coordinate of squirrel.
			float m_y;          ///< Y-coordinate of squirrel.
			
			int m_cur_cell;     ///< Landscape cell where this squirrel is.
			int m_cur_step;     ///< Number of steps since the squirrel was born.
		
			bool m_infected;    ///< Indicates squirrel has the virus. 
			int  m_inf_step;    ///< Step at which the squirrel became infected.
			bool m_dead;        ///< Indicates squirrel is dead or pool is shutting down.
	};

}   //  namespace Biology


#endif  //  #ndef BIOLOGY_SQUIRREL_HPP
