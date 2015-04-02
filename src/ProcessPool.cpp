//////////////////////////////////////////////////////////////////////////////
/// @file     ProcessPool.cpp
///
/// @brief    Implementation of class ProcessPool.
///


//////////////////////////////////////////////////////////////////////////////
//  This class's header.
#include "ProcessPool.hpp"


//////////////////////////////////////////////////////////////////////////////
//  Local headers.
extern "C"
{
	#include "pool.h"
}


//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <iostream>
#include <stdexcept>


namespace Mpi
{
	//////////////////////////////////////////////////////////////////////////////
	/// @details    Initialises the process pool.
	///
	/// @post       The pool is initialised and a valid process type obtained.
	///
	/// @exception  std::logic_error The pool code returned an undocumented value.
	///
	ProcessPool::ProcessPool()
		: m_type(static_cast<Type>(processPoolInit()))
	{		
		if ((m_type != eQuit) && (m_type != eWorker) && (m_type != eMaster))
		{
			throw std::logic_error("undocumented value returned by processPoolInit()");
		}
	}


	//////////////////////////////////////////////////////////////////////////////
	/// @details    Clean-up is automatic so far.
	///
	/// @pre        List what must be true before this function is called.
	/// @post       List what is guaranteed to be true after this function returns.
	///
	/// @exception  None; this is a destructor.
	///
	ProcessPool::~ProcessPool()
	{
		processPoolFinalise();
	}

/*
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
	ProcessPool::ProcessPool(ProcessPool const& rhs)
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
	ProcessPool& ProcessPool::operator=(ProcessPool const& rhs)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;

		(void) rhs;
		return *this;
	}
*/
	
}   //  namespace Mpi
