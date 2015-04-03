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

			static void Spawn(Mpi::Communicator const& comm);   ///< Gives birth to a squirrel.
		
		private:
			Squirrel(Squirrel const& rhs);              ///< Copy constructor.
			Squirrel& operator=(Squirrel const& rhs);   ///< Assignment operator.

			void HandleFirstUpdate();   ///< Does the first update.      
			void Step();                ///< Moves this squirrel one step.	
			void Die();                 ///< Kills the squirrel.
			
			Mpi::Communicator const& m_comm;    ///< MPI communcator for the pool.
			unsigned int update_count;          ///< Number of updates since last.

			long m_rng_state;   ///< Random number generator state (required by biologists' code).
			
			float m_x;          ///< X-coordinate of squirrel.
			float m_y;          ///< Y-coordinate of squirrel.
			int m_cur_cell;     ///< Landscape cell where this squirrel is.

			bool m_shutdown;    ///< Indicates pool is shutting down.
	};

}   //  namespace Biology


#endif  //  #ndef BIOLOGY_SQUIRREL_HPP
