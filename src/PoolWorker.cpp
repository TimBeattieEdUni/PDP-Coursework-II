//////////////////////////////////////////////////////////////////////////////
/// @file     PoolWorker.cpp
///
/// @brief    Implementation of class PoolWorker.
///


//////////////////////////////////////////////////////////////////////////////
//  This class's header.
#include "PoolWorker.hpp"


//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <iostream>


namespace repo
{
	namespace src
	{
		//////////////////////////////////////////////////////////////////////////////
		/// @details    Describe object initialisation here.
		///
		/// @param      Describe parameters here, one line each.
		///
		/// @post       List what is guaranteed to be true after this function returns.
		///
		/// @exception  List exceptions this function may throw here.
		///
		PoolWorker::PoolWorker()
		{
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}


		//////////////////////////////////////////////////////////////////////////////
		/// @details    Describe object destruction here.
		///
		/// @param      Describe parameters here, one line each.
		///
		/// @pre        List what must be true before this function is called.
		/// @post       List what is guaranteed to be true after this function returns.
		///
		/// @exception  None; this is a destructor.
		///
		PoolWorker::~PoolWorker()
		{
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}


		//////////////////////////////////////////////////////////////////////////////
		/// @details    Describe copy construction here.
		///
		/// @param      rhs  Object to copy.
		///
		/// @pre        List what must be true before this function is called.
		/// @post       List what is guaranteed to be true after this function returns.
		///
		/// @exception  List exceptions this function may throw here.
		///
		PoolWorker::PoolWorker(PoolWorker const& rhs)
		{
			std::cout << __PRETTY_FUNCTION__ << std::endl;

			(void) rhs;
		}


		//////////////////////////////////////////////////////////////////////////////
		/// @details    Describe object assignment here.
		///
		/// @param      rhs  Object on the right-hand side of the assignment statement.
		/// @return     Object which has been assigned.
		///
		/// @pre        List what must be true before this function is called.
		/// @post       List what is guaranteed to be true after this function returns.
		///
		/// @exception  List exceptions this function may throw here.
		///
		PoolWorker& PoolWorker::operator=(PoolWorker const& rhs)
		{
			std::cout << __PRETTY_FUNCTION__ << std::endl;

			(void) rhs;
			return *this;
		}

	}   //  namespace src

}   //  namespace repo
