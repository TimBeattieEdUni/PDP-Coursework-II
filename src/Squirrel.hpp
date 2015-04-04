//////////////////////////////////////////////////////////////////////////////
/// @file     Squirrel.hpp
///
/// @brief    Interface declaration for class Squirrel.
///


#ifndef BIOLOGY_SQUIRREL_HPP
#define BIOLOGY_SQUIRREL_HPP


//////////////////////////////////////////////////////////////////////////////
//  Local headers.
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
			Squirrel(Mpi::Communicator const& comm);    ///< Constructor.
			~Squirrel();   ///< Destructor.

			bool Update();   ///< Driver.
		
		private:
			Squirrel(Squirrel const& rhs);              ///< Copy constructor.
			Squirrel& operator=(Squirrel const& rhs);   ///< Assignment operator.

			void Die();                 ///< Kills the squirrel.
			void HandleFirstUpdate();   ///< Does the first update.      
			void HandleMessages();      ///< Handles incoming MPI messages.
			void Spawn();               ///< Gives birth to a squirrel.
			void Step();                ///< Moves this squirrel one step.	

			/// Tells a cell we've stepped.
			void NotifyCell(int cell, Pdp::ESquirrelStep::ESquirrelStep step);

			int m_last50pop[50];    ///< Cell population influx values from the last 50 steps.
			int m_last50inf[50];    ///< Cell infection level values from the last 50 steps.
			
			Mpi::Communicator const& m_comm;    ///< MPI communcator for the pool.
			unsigned int update_count;          ///< Number of updates since last.

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
