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
#include <sys/time.h>


namespace Biology
{
	//////////////////////////////////////////////////////////////////////////////
	/// @details    Stores the current wall time for reference.
	///
	/// @param      Length of a day in seconds.
	///
	///
	/// @note       Very oddly, and completely at random, MPI_Wtime() started always 
	///             returning the same value: 1.42809e+09.  No idea why, couldn't 
	///             fix, so modified this to use gettimeofday().  On both MORAR and
	///             several CPLabs machines.
	///
	DayTicker::DayTicker(double day_len)
//		: m_start_time(MPI_Wtime())
		: m_start_time(MyWtime())
		, m_day_len(day_len)
	{
//		std::cout << __PRETTY_FUNCTION__ << " : " << m_start_time << " " << m_day_len << std::endl;
	}


	//////////////////////////////////////////////////////////////////////////////
	/// @details    No cleanup necessary 
	///
	DayTicker::~DayTicker()
	{
//		std::cout << __PRETTY_FUNCTION__ << std::endl;
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
//		double dday = (MyWtime() - m_start_time) / double(m_day_len);
		
//		unsigned int day = dday;

//		return day;

		unsigned int day = (unsigned int) ((MPI_Wtime() - m_start_time) / (double)m_day_len);		
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

	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Replacement for MPI_Wtime(), which stopped working randomly.
	///
	double DayTicker::MyWtime()
	{
		struct timeval tv;
		struct timezone tz;

		gettimeofday(&tv, &tz);
		
		double wall_time = (double)tv.tv_usec / 1000000.0;

		return wall_time;		
	}
	
}   //  namespace Biology
