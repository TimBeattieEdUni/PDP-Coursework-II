//////////////////////////////////////////////////////////////////////////////
/// @file     SimCoordinator.hpp
///
/// @brief    Interface declaration for class SimCoordinator.
///


#ifndef BIOLOGY_SIMCOORDINATOR_HPP
#define BIOLOGY_SIMCOORDINATOR_HPP


namespace Biology
{
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Manages the simulation.  Provides a day ticker to landscape cells.
	///
	/// @details
	///
	/// @invariant
	///
	class SimCoordinator
	{
		public:
			SimCoordinator();    ///< Constructor.
			~SimCoordinator();   ///< Destructor.

			void Update();       ///< Runs the Coordinator's part of the simulation.

		private:
			SimCoordinator(SimCoordinator const& rhs);              ///< Copy constructor.
			SimCoordinator& operator=(SimCoordinator const& rhs);   ///< Assignment operator.
	};

}   //  namespace Biology


#endif  //  #ndef BIOLOGY_SIMCOORDINATOR_HPP
