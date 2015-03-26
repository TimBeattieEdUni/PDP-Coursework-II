//////////////////////////////////////////////////////////////////////////////
/// @file     Cell.hpp
///
/// @brief    Interface declaration for class Cell.
///


#ifndef BIOLOGY_CELL_HPP
#define BIOLOGY_CELL_HPP


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

		private:
			Cell(Cell const& rhs);              ///< Copy constructor.
			Cell& operator=(Cell const& rhs);   ///< Assignment operator.
	};

}   //  namespace Biology


#endif  //  #ndef BIOLOGY_CELL_HPP