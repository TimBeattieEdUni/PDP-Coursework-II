//////////////////////////////////////////////////////////////////////////////
/// @file       mpi.hpp
///
/// @brief      Interface declaration for class Mpi.
///


#ifndef SQUIRRELS_MPI_HPP
#define SQUIRRELS_MPI_HPP


//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <string>


//////////////////////////////////////////////////////////////////////////////
//  Framework headers.
#include "mpi.h"


class Mpi
{
    public:
        Mpi(int argc, char** argv);
        ~Mpi() { MPI_Finalize(); }
		std::string GetProcName();
};


#endif //  #defined SQUIRRELS_MPI_H
