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


namespace Biology
{
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Represents a landscape cell.
	///
	class Cell
	{
		public:
			Cell();    ///< Constructor.
			~Cell();   ///< Destructor.

		void Update();   ///< Driver.

		private:
			Cell(Cell const& rhs);              ///< Copy constructor.
			Cell& operator=(Cell const& rhs);   ///< Assignment operator.
	};

}   //  namespace Biology


#endif  //  #ndef BIOLOGY_CELL_HPP
