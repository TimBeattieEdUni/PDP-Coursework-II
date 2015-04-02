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
		
		private:
			Squirrel(Squirrel const& rhs);              ///< Copy constructor.
			Squirrel& operator=(Squirrel const& rhs);   ///< Assignment operator.

			Mpi::Communicator const& m_comm;    ///< MPI communcator for the pool.
	};

}   //  namespace Biology


#endif  //  #ndef BIOLOGY_SQUIRREL_HPP
