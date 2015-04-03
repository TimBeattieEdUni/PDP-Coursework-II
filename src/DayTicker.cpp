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
		std::cout << __PRETTY_FUNCTION__ << " : " << m_start_time << std::endl;
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
		std::cout << __PRETTY_FUNCTION__ << std::endl;

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
//		double wtime = MPI_Wtime();
//		std::cout << wtime << std::endl;
		
		unsigned int day = (unsigned int) ((MPI_Wtime() - m_start_time) / (double)m_day_len);
	
//		std::cout << wtime << " - " << m_start_time << " / " << m_day_len << " = " << day << std::endl;
		
		return day;
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
