//////////////////////////////////////////////////////////////////////////////
/// @file       mpi.hpp
///
/// @brief      Implementation of class Mpi.
///

//////////////////////////////////////////////////////////////////////////////
//  This class's header.
#include "Mpi.hpp"


//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <stdexcept>


namespace Mpi
{
	Mpi::Mpi(int argc, char** argv)
	{
		if (MPI_Init(&argc, &argv))
		{
			throw std::runtime_error("failed to initialise MPI");
		}
	}

	Mpi::~Mpi()
	{
		 MPI_Finalize();
	}


	std::string Mpi::GetProcName()
	{
		int namelen = 0;
		char procname[MPI_MAX_PROCESSOR_NAME];
		if (MPI_Get_processor_name(procname, &namelen))
		{
			throw std::runtime_error("failed to obtain MPI processor name");
		}

		return std::string(procname, namelen);
	}

}   //  namespace Mpi
