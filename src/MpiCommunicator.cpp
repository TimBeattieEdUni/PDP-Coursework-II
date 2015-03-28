//////////////////////////////////////////////////////////////////////////////
/// @file     MpiCommunicator.cpp
///
/// @brief    Implementation of class MpiCommunicator.
///


//////////////////////////////////////////////////////////////////////////////
//  This class's header.
#include "MpiCommunicator.hpp"


//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <stdexcept>

namespace Mpi
{
	Communicator::Communicator(int comm_id)
		: m_comm(comm)
		, m_rank(0)
		, m_size(0)
	{
		if (MPI_Comm_rank(comm_id, &m_rank))
		{
			throw std::runtime_error("failed to obtain MPI rank");
		}

		if (MPI_Comm_size(comm_id, &m_size))
		{
			throw std::runtime_error("failed to obtain number of MPI processes");
		}
	}

}   //  namespace Mpi
