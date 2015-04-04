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
	
}   //  namespace Mpi
