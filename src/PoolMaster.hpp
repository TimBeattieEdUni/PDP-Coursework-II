//////////////////////////////////////////////////////////////////////////////
/// @file     PoolMaster.hpp
///
/// @brief    Interface declaration for class PoolMaster.
///


#ifndef REPO_SRC_POOLMASTER_HPP
#define REPO_SRC_POOLMASTER_HPP


//////////////////////////////////////////////////////////////////////////////
//  Local headers.
#include "MpiCommunicator.hpp"


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
			PoolMaster(Communicator const& comm);    ///< Constructor.
			~PoolMaster();   ///< Destructor.

			void Run();

		private:
			PoolMaster(PoolMaster const& rhs);              ///< Copy constructor.
			PoolMaster& operator=(PoolMaster const& rhs);   ///< Assignment operator.

			Communicator const& m_comm;   ///< MPI communcator for the pool.
	};

}   //  namespace Mpi


#endif  //  #ndef REPO_SRC_POOLMASTER_HPP
