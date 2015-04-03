//////////////////////////////////////////////////////////////////////////////
/// @file     Config.hpp
///
/// @brief    Interface declaration for class Config.
///


#ifndef PDP_CONFIG_HPP
#define PDP_CONFIG_HPP


//////////////////////////////////////////////////////////////////////////////
//  Standard headers.
#include <cstddef>


namespace Pdp
{
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Command-line configuration.
	///
	class Config
	{
		public:
			Config(int argc, char* argv[]);   ///< Constructor.
			~Config();							///< Destructor.
		
			int GetCells()    const { return m_cells; }       ///< Getter.
			int GetIniSqrls() const { return m_ini_sqrls; }   ///< Getter.
			int GetMaxSqrls() const { return m_max_sqrls; }   ///< Getter.
			int GetSimLen()   const { return m_sim_len; }     ///< Getter.

			double GetDayLen() const { return m_day_len; }    ///< Getter.
		
		private:
			Config(Config const& rhs);              ///< Copy constructor.
			Config& operator=(Config const& rhs);   ///< Assignment operator.
			
			int m_cells;       ///< Number of landscape cells.
			int m_ini_sqrls;   ///< Initial number of squirrels. 
			int m_max_sqrls;   ///< Maximum number of squirrels.
			int m_sim_len;     ///< Maximum length of sim in days.
			
			double m_day_len;  ///< Length of a simulated day in seconds of wall time.
	};

}   //  namespace PDP


#endif  //  #ndef PDP_CONFIG_HPP
