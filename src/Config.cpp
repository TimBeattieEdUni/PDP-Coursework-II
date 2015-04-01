//////////////////////////////////////////////////////////////////////////////
/// @file     Config.cpp
///
/// @brief    Implementation of class Config.
///


//////////////////////////////////////////////////////////////////////////////
//  This class's header.
#include "Config.hpp"


//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <iostream>
#include <stdexcept>
#include <sstream>


namespace Pdp
{
	//////////////////////////////////////////////////////////////////////////////
	/// @details    Reads configuration from command-line arguments.
	///
	/// @param      argc Standard argument count.
	/// @param      argv Standard argument list.		
	///
	/// @exception  Runtime error if config could not be obtained from command line.
	///
	Config::Config(int argc, char* argv[])
		: m_cells(0)
		, m_sqrls(0)
		, m_day_len(0.0)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		
		if (4 > argc)
		{
			throw std::runtime_error("usage: squirrels <cells> <squirrels>");
		}
		
		std::stringstream converter;

		converter << argv[1] << " " << argv[2] << " " << argv[3];
		converter >> m_cells >> m_sqrls >> m_day_len;
	}


	//////////////////////////////////////////////////////////////////////////////
	/// @details    Describe object destruction here.
	///
	Config::~Config()
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
	Config::Config(Config const& rhs)
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
	Config& Config::operator=(Config const& rhs)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;

		(void) rhs;
		return *this;
	}

}   //  namespace Pdp
