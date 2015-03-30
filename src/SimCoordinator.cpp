//////////////////////////////////////////////////////////////////////////////
/// @file     SimCoordinator.cpp
///
/// @brief    Implementation of class SimCoordinator.
///


//////////////////////////////////////////////////////////////////////////////
//  This class's header.
#include "SimCoordinator.hpp"


//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <iostream>
#include <unistd.h>


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
		SimCoordinator::SimCoordinator()
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
		SimCoordinator::~SimCoordinator()
		{
			std::cout << __PRETTY_FUNCTION__ << std::endl;
		}


		//////////////////////////////////////////////////////////////////////////////
		/// @details    Handles any new messages.  Sends out day ticks. In charge of 
		///             ending the simulation when appropriate.
		///
		void SimCoordinator::Update()
		{
			std::cout << __PRETTY_FUNCTION__ << std::endl;
			
			//  wait for a second to pass.
			usleep(1000000);
			
			//  send a message to all landscape cells at the end of each day.
			
			//  get stats back from landscape cells.

			//  detect end of simulation			
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
//		SimCoordinator::SimCoordinator(SimCoordinator const& rhs)
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
//		SimCoordinator& SimCoordinator::operator=(SimCoordinator const& rhs)
//		{
//			std::cout << __PRETTY_FUNCTION__ << std::endl;
//
//			(void) rhs;
//			return *this;
//		}

	}   //  namespace src

}   //  namespace repo
