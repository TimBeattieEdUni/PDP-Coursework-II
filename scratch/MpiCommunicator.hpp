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


//  MPI just uses ints; lets give the data type a name.
typedef int mpi_comm_t;


//////////////////////////////////////////////////////////////////////////////
/// @brief      MPI communicator.
///
/// @details    Stores details about an MPI communicator.
///
class MpiCommunicator
{
    public:
        MpiCommunicator(mpi_comm_t comm_id);
        int GetRank() { return m_rank; }
		int GetSize() { return m_size; }

    private:
		mpi_comm_t  m_comm_id;

        int m_rank;
        int m_size;
};


#endif  /// #defined MPI_COMMUNICATOR_HPP
