//////////////////////////////////////////////////////////////////////////////
/// @file     DayTicker.cpp
///
/// @brief    Implementation of class DayTicker.
///


//////////////////////////////////////////////////////////////////////////////
//  This class's header.
#include "DayTicker.hpp"


//////////////////////////////////////////////////////////////////////////////
//  Framework headers.
#include "mpi.h"


//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <iostream>


namespace Biology
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
	DayTicker::DayTicker(double day_len)
		: m_start_time(MPI_Wtime())
		, m_day_len(day_len)
	{

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
	DayTicker::~DayTicker()
	{

	}


	//////////////////////////////////////////////////////////////////////////////
	/// @details      
	///
	/// @param        
	/// @return       
	///
	/// @pre          
	/// @post         
	///
	/// @exception    
	///
	unsigned int DayTicker::GetDay()
	{
		return (MPI_Wtime() - m_start_time) / m_day_len;		
	}

	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      
	///
	/// @param        
	/// @return       
	///
	/// @pre          
	/// @post         
	///
	/// @exception    
	///
	unsigned int DayTicker::GetWeek()
	{		
		return GetDay() / 7;
	}

	
//		//////////////////////////////////////////////////////////////////////////////
//		/// @details    Describe copy construction here.
//		///
//		/// @param      rhs  Object to copy.
//		///
//		/// @pre        List what must be true before this function is called.
//		/// @post       List what is guaranteed to be true after this function returns.
//		///
//		/// @exception  List exceptions this function may throw here.
//		///
//		DayTicker::DayTicker(DayTicker const& rhs)
//		{
//			std::cout << __PRETTY_FUNCTION__ << std::endl;
//
//			(void) rhs;
//		}
//
//
//		//////////////////////////////////////////////////////////////////////////////
//		/// @details    Describe object assignment here.
//		///
//		/// @param      rhs  Object on the right-hand side of the assignment statement.
//		/// @return     Object which has been assigned.
//		///
//		/// @pre        List what must be true before this function is called.
//		/// @post       List what is guaranteed to be true after this function returns.
//		///
//		/// @exception  List exceptions this function may throw here.
//		///
//		DayTicker& DayTicker::operator=(DayTicker const& rhs)
//		{
//			std::cout << __PRETTY_FUNCTION__ << std::endl;
//
//			(void) rhs;
//			return *this;
//		}

}   //  namespace Biology
