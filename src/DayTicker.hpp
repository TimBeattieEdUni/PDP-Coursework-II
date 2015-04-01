//////////////////////////////////////////////////////////////////////////////
/// @file     DayTicker.hpp
///
/// @brief    Interface declaration for class DayTicker.
///


#ifndef BIOLOGY_DAYTICKER_HPP
#define BIOLOGY_DAYTICKER_HPP

	
namespace Biology
{
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Provides a simple day ticker using MPI_Wtime()
	///
	class DayTicker
	{
		public:
			DayTicker(double day_len);    ///< Constructor.
			~DayTicker();   ///< Destructor.

			unsigned int GetDay();  ///< Getter.
			unsigned int GetWeek(); ///< Getter.
			bool NewDay();   ///< Indicates whether a day has passed.

		private:
			DayTicker(DayTicker const& rhs);              ///< Copy constructor.
			DayTicker& operator=(DayTicker const& rhs);   ///< Assignment operator.
		
			double m_start_time;       ///< Time to measure days from.
			double m_day_len;          ///< Length of a day in seconds of wall time.
	};

}   //  namespace Biology


#endif  //  #ndef BIOLOGY_DAYTICKER_HPP
