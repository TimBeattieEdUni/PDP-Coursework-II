//////////////////////////////////////////////////////////////////////////////
/// @file     PoolWorker.hpp
///
/// @brief    Interface declaration for class PoolWorker.
///


#ifndef MPI_POOLWORKER_HPP
#define MPI_POOLWORKER_HPP


//////////////////////////////////////////////////////////////////////////////
//  Local headers.
#include "MpiCommunicator.hpp"


namespace Mpi
{
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Runs a process pool worker.
	///
	/// @details
	///
	class PoolWorker
	{
		public:
			PoolWorker(Communicator const& comm, Pdp::Config const& config);    ///< Constructor.
			~PoolWorker();   ///< Destructor.

			void Run();   ///< Does the worker's work.

		private:
			PoolWorker(PoolWorker const& rhs);              ///< Copy constructor.
			PoolWorker& operator=(PoolWorker const& rhs);   ///< Assignment operator.

			Communicator const& m_comm;   ///< MPI communcator for the pool.
			Pdp::Config const& m_config;   ///< App config.

	};

}   //  namespace Mpi


#endif  //  #ndef MPI_POOLWORKER_HPP
