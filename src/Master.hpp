//////////////////////////////////////////////////////////////////////////////
/// @file     Master.hpp
///
/// @brief    Interface declaration for class Master.
///


#ifndef REPO_SRC_MASTER_HPP
#define REPO_SRC_MASTER_HPP


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
		class Master
		{
			public:
				Master();    ///< Constructor.
				~Master();   ///< Destructor.

			private:
				Master(Master const& rhs);              ///< Copy constructor.
				Master& operator=(Master const& rhs);   ///< Assignment operator.
		};

	}   //  namespace src

}   //  namespace repo


#endif  //  #ndef REPO_SRC_MASTER_HPP
