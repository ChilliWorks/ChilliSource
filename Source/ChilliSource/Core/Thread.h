/*
 * File: Thread.h
 * Date: 02/11/2010 2010 
 * Description: Handles the spawning and updating of threads. Invokes callbacks on completion. Wraps around boost threads for the time being
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef MOFLO_CORE_THREAD_H_
#define MOFLO_CORE_THREAD_H_

#include <ChilliSource/Core/Tasks.hpp>

#include <ChilliSource/Core/boost/thread.hpp>
#include <ChilliSource/Core/boost/thread/condition.hpp>

namespace moFlo
{
	class CThread
	{
	public:
		
		typedef boost::thread Thread;
		typedef boost::mutex::scoped_lock ScopedLock;
		typedef boost::mutex Mutex;
        typedef boost::recursive_mutex::scoped_lock RecursiveScopedLock;
        typedef boost::recursive_mutex RecursiveMutex;
		typedef boost::condition Condition;
		
		CThread();
		virtual ~CThread();
		
		//----------------------------------------------------
		/// Start
		///
		/// Launch the thread. Which will update the threaded
		/// function
		///
		/// @param Threaded function
		//----------------------------------------------------
		template <typename T> void Start(const T& inThreadedFunc)
		{
			mpThread = new CThread::Thread(inThreadedFunc);
		}
		//----------------------------------------------------
		/// Stop
		///
		/// Force the thread to suspend and rejoin the main
		/// thread. This will also destroy the thread
		//----------------------------------------------------
		void Stop();
		//----------------------------------------------------
		/// Yield
		///
		/// Causes the thread to give up its timeslice
		//----------------------------------------------------
		static void Yield();
        //----------------------------------------------------
		/// Sleep
		///
		/// Causes the thread to suspend for a specified
        /// time
        ///
        /// @param Time to suspend in milliseconds
		//----------------------------------------------------
		static void Sleep(const u32 inudwMS);
		//----------------------------------------------------
		/// Is Active
		///
		/// @return Whether the thread is in progress
		//----------------------------------------------------
		bool IsActive() const;
		
	private:

		Thread* mpThread;
	};
}

#endif
