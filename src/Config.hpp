//////////////////////////////////////////////////////////////////////////////
/// @file     Config.hpp
///
/// @brief    Interface declaration for class Config.
///


#ifndef PDP_CONFIG_HPP
#define PDP_CONFIG_HPP


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

		private:
			Config(Config const& rhs);              ///< Copy constructor.
			Config& operator=(Config const& rhs);   ///< Assignment operator.
			
			size_t m_cells;   ///< Number of landscape cells.
	};

}   //  namespace PDP


#endif  //  #ndef PDP_CONFIG_HPP
