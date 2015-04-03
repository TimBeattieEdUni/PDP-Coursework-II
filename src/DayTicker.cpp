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
		std::cout << __PRETTY_FUNCTION__ << " : " << m_start_time << " " << m_day_len << std::endl;
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
		double wtime = MyWtime();
		std::cout << "MyWtime(): " << wtime << std::endl;
		
		double dday = (MyWtime() - m_start_time) / double(m_day_len);
		std::cout << "dday: " << dday << std::endl;
		
		unsigned int day = dday;
		std::cout << "day: " << day << std::endl;

//		unsigned int day = (unsigned int) ((MPI_Wtime() - m_start_time) / (double)m_day_len);
	
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

	
	//////////////////////////////////////////////////////////////////////////////
	/// @details      Replacement for MPI_Wtime(), which stopped working randomly.
	///
	double DayTicker::MyWtime()
	{
		struct timeval tv;
		struct timezone tz;

		gettimeofday(&tv, &tz);
		
		double wt = (double)tv.tv_usec/1000000.;

		return (wt);		
	}
	
}   //  namespace Biology
