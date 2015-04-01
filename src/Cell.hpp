//////////////////////////////////////////////////////////////////////////////
/// @file     Cell.hpp
///
/// @brief    Interface declaration for class Cell.
///


#ifndef BIOLOGY_CELL_HPP
#define BIOLOGY_CELL_HPP


//////////////////////////////////////////////////////////////////////////////
//  Local headers.
#include "Config.hpp"
#include "MpiCommunicator.hpp"
#include "DayTicker.hpp"


namespace Biology
{
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Represents a landscape cell.
	///
	class Cell
	{
		public:
			Cell(Mpi::Communicator const& comm, Pdp::Config const& config);    ///< Constructor.
			~Cell();   ///< Destructor.

		void Update();   ///< Driver.

		private:
			Cell(Cell const& rhs);              ///< Copy constructor.
			Cell& operator=(Cell const& rhs);   ///< Assignment operator.
		
			DayTicker m_ticker;             ///< Keeps track of simulation time.
			unsigned int m_cur_day;         ///< The current day.

	};

}   //  namespace Biology


#endif  //  #ndef BIOLOGY_CELL_HPP
