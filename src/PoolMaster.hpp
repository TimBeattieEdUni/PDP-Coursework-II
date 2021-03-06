//////////////////////////////////////////////////////////////////////////////
/// @file     PoolMaster.hpp
///
/// @brief    Interface declaration for class PoolMaster.
///


#ifndef MPI_POOLMASTER_HPP
#define MPI_POOLMASTER_HPP


//////////////////////////////////////////////////////////////////////////////
//  Local headers.
#include "MpiCommunicator.hpp"
#include "Config.hpp"


namespace Mpi
{
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Runs the process pool's master.
	///
	/// @details
	///
	class PoolMaster
	{
		public:
			PoolMaster(Communicator const& comm, Biology::Config const& config);    ///< Constructor.
			~PoolMaster();   ///< Destructor.

			void Run();   ///< Does the master's work.

		private:
			PoolMaster(PoolMaster const& rhs);              ///< Copy constructor.
			PoolMaster& operator=(PoolMaster const& rhs);   ///< Assignment operator.

			Communicator const& m_comm;   ///< MPI communcator for the pool.
			Biology::Config const& m_config;   ///< App config.
	};

}   //  namespace Mpi


#endif  //  #ndef MPI_POOLMASTER_HPP
