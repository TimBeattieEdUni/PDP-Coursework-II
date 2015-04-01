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

			bool Update();   ///< Updates the cell.

		private:
			Cell(Cell const& rhs);              ///< Copy constructor.
			Cell& operator=(Cell const& rhs);   ///< Assignment operator.
			Mpi::Communicator const& m_comm;    ///< MPI communcator for the pool.

			DayTicker m_ticker;             ///< Keeps track of simulation time.
			unsigned int m_cur_day;         ///< The current day.
			unsigned int m_num_sq;			///< Number of squirrels in this cell.

	};

}   //  namespace Biology


#endif  //  #ndef BIOLOGY_CELL_HPP
