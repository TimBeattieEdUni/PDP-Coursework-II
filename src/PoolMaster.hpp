//////////////////////////////////////////////////////////////////////////////
/// @file     PoolMaster.hpp
///
/// @brief    Interface declaration for class PoolMaster.
///


#ifndef REPO_SRC_POOLMASTER_HPP
#define REPO_SRC_POOLMASTER_HPP


namespace repo
{
	namespace src
	{
		//////////////////////////////////////////////////////////////////////////////
		/// @brief
		///
		/// @details
		///
		/// @invariant
		///
		class PoolMaster
		{
			public:
				PoolMaster();    ///< Constructor.
				~PoolMaster();   ///< Destructor.

			private:
				PoolMaster(PoolMaster const& rhs);              ///< Copy constructor.
				PoolMaster& operator=(PoolMaster const& rhs);   ///< Assignment operator.
		};

	}   //  namespace src

}   //  namespace repo


#endif  //  #ndef REPO_SRC_POOLMASTER_HPP
