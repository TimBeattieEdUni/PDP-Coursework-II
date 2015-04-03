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
		, m_ini_sqrls(0)
		, m_max_sqrls(0)
		, m_sim_len(0)
		, m_day_len(0.0)
	{		
		if (5 > argc)
		{
			throw std::runtime_error("usage: squirrels <cells> <squirrels> ");
		}
		
		std::stringstream converter;

		converter << argv[1] 
		          << " " << argv[2] 
		          << " " << argv[3]
		          << " " << argv[4]
				  << " " << argv[5];
		
		converter >> m_cells 
		          >> m_ini_sqrls 
				  >> m_max_sqrls 
				  >> m_sim_len
				  >> m_day_len;		
 	}


	//////////////////////////////////////////////////////////////////////////////
	/// @details    Describe object destruction here.
	///
	Config::~Config()
	{

	}
	
}   //  namespace Pdp
