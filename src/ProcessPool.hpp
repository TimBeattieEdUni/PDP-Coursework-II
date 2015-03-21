//////////////////////////////////////////////////////////////////////////////
/// @file     ProcessPool.hpp
///
/// @brief    Interface declaration for class ProcessPool.
///


#ifndef PDP_PROCESSPOOL_HPP
#define PDP_PROCESSPOOL_HPP


namespace Mpi
{
	//////////////////////////////////////////////////////////////////////////////
	/// @brief      Wrapper for process pool code.
	///
	/// @details    Wraps the provided process pool code so it can be replaced
	///             later if there's time.
	///
	class ProcessPool
	{
		public:
			ProcessPool();    ///< Constructor.
			~ProcessPool();   ///< Destructor.
			
		enum Type
		{
			eQuit    = 0,
			eWorker  = 1,
			eMaster  = 2
		};
		
		Type GetType() { return m_type; };   ///< Getter.

		private:
			ProcessPool(ProcessPool const& rhs);              ///< Copy constructor.
			ProcessPool& operator=(ProcessPool const& rhs);   ///< Assignment operator.
		
			Type m_type;       ///< Master/worker/abort indicator.
	};

}   //  namespace Mpi


#endif  //  #ndef PDP_PROCESSPOOL_HPP
