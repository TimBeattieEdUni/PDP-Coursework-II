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
			Config(int argc, char* argv[]);    ///< Constructor.
			~Config();   ///< Destructor.
		
			size_t GetCells() const { return m_cells; }   ///< Getter.
			size_t GetSqrls() const { return m_sqrls; }   ///< Getter.

		private:
			Config(Config const& rhs);              ///< Copy constructor.
			Config& operator=(Config const& rhs);   ///< Assignment operator.
			
			size_t m_cells;   ///< Number of landscape cells.
			size_t m_sqrls;   ///< Initial number of squirrels. 
	};

}   //  namespace PDP


#endif  //  #ndef PDP_CONFIG_HPP
