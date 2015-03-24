//////////////////////////////////////////////////////////////////////////////
/// @file     PoolWorker.hpp
///
/// @brief    Interface declaration for class PoolWorker.
///


#ifndef REPO_SRC_POOLWORKER_HPP
#define REPO_SRC_POOLWORKER_HPP


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
		class PoolWorker
		{
			public:
				PoolWorker();    ///< Constructor.
				~PoolWorker();   ///< Destructor.

			private:
				PoolWorker(PoolWorker const& rhs);              ///< Copy constructor.
				PoolWorker& operator=(PoolWorker const& rhs);   ///< Assignment operator.
		};

	}   //  namespace src

}   //  namespace repo


#endif  //  #ndef REPO_SRC_POOLWORKER_HPP
