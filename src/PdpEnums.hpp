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
			eAssignTask,         ///< Tells a worker process what type of actor to be.
			eCellStats,          ///< Squirrel statistics for coordinator.
			eSquirrelLifetime,   ///< Squirrel birth/death event.
			eSquirrelStep        ///< Squirrel step event.
		};
	}
	
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Tasks for worker processes.
	///
	namespace ETask
	{
		enum ETask
		{
			eCoordinator,   ///< Worker should be the coordinator.
			eSquirrel,      ///< Worker should be a squirrel.
			eCell           ///< Worker should be a landscape cell.
		};
	}
	
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Squirrels steps relative to landscape cells.
	///
	namespace ESquirrelStep
	{
		enum ESquirrelStep
		{
			eIn,      ///< Squirrel arriving in a cell.
			eOut,     ///< Squirrel leaving a cell.
			eWithin   ///< Squirrel moving within a cell.
		};
	}
}   //  namespace Pdp


#endif  //  #ndef PDP_ENUMS_HPP
