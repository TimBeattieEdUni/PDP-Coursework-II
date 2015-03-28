//////////////////////////////////////////////////////////////////////////////
/// @file       Enums.hpp
///
/// @brief      Enumerated types for squirrel simulation.
///


#ifndef PDP_ENUMS_HPP
#define PDP_ENUMS_HPP


namespace Pdp 
{
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Tag values for MPI messages.
	///
	namespace EMpiMsgTag
	{
		enum EMpiMsgTag
		{
			ePoolPid  = 16383,   ///< Ensures our code won't use the same value.
			ePoolCtrl = 16384,   ///< Ensures our code won't use the same value.
			eAssignTask          ///< Tells a worker process what to do.
		};
	}
	
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Tasks for worker processes
	///
	namespace ETask
	{
		enum ETask
		{
			eSquirrel,   ///< Worker should be a squirrel.
			eCell        ///< Worker should be a landscape cell.
		};
	}
	
}   //  namespace Pdp


#endif  //  #ndef PDP_ENUMS_HPP
