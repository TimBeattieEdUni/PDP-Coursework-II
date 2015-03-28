//////////////////////////////////////////////////////////////////////////////
/// @file     MpiCommunicator.hpp
///
/// @brief    Interface declaration for class MpiCommunicator.
///


#ifndef MPI_COMMUNICATOR_HPP
#define MPI_COMMUNICATOR_HPP


//////////////////////////////////////////////////////////////////////////////
//  Framework headers.
#include "mpi.h"

namespace Mpi
{
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      MPI communicator.
	///
	/// @details    Stores details about an MPI communicator.
	///
	class Communicator
	{
		public:
			Communicator(int comm);
		
			int GetComm() const { return m_comm; }
			int GetRank() const { return m_rank; }
			int GetSize() const { return m_size; }

		private:
			int m_comm;
			int m_rank;
			int m_size;
	};

}   //  namespace Mpi


#endif  /// #defined MPI_COMMUNICATOR_HPP
