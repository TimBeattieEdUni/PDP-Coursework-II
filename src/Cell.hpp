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
#include "Config.hpp"


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

			void BumpStatistics();              ///< Bumps daily influx and infection values.
			void ReceiveSquirrelStep();         ///< Receives a squirrel step event.
			void SendStatistics(int pid);       ///< Sends population and infection stats.
			
			Mpi::Communicator const& m_comm;    ///< MPI communcator for the pool.
			Pdp::Config const& m_config;        ///< App config.

			DayTicker m_ticker;            ///< Keeps track of simulation time.
			
			unsigned int m_cur_day;        ///< The current day.
			unsigned int m_cur_week;       ///< The current week.
//			unsigned int m_num_sq;         ///< Number of squirrels in this cell.
		
			unsigned int m_sq_steps1;      ///< Steps by squirrels in this cell today...
			unsigned int m_sq_steps2;      ///< ...and yesterday...
			unsigned int m_sq_steps3;      ///< ...and two days ago.
		
			unsigned int m_inf_steps1;     ///< Steps by infected squirrels in this cell today...
			unsigned int m_inf_steps2;     ///< ...and yesterday...
			unsigned int m_inf_steps3;     ///< ...and two days ago.
	};

}   //  namespace Biology


#endif  //  #ndef BIOLOGY_CELL_HPP
