/*
 *  TaskScheduler.cpp
 *  moFlow
 *
 *  Created by Scott Downie on 09/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace moFlo
{
	boost::threadpool::pool* CTaskScheduler::pThreadPool = NULL;
	
	CThread::RecursiveMutex CTaskScheduler::MainThreadTaskMutex;
	DYNAMIC_ARRAY<CTaskScheduler::GenericTaskType> CTaskScheduler::MainThreadTasks;

	//-------------------------------------------------
	/// Init
	///
	/// Called by the application to create a thread
	/// pool with the optimum number of threads for
	/// the hardware
	///
	/// @param Number of threads
	//------------------------------------------------
	void CTaskScheduler::Init(u32 udwNumThreads)
	{
		pThreadPool = new boost::threadpool::pool(udwNumThreads);
	}
	//----------------------------------------------------
	/// Get Num Active Tasks
	///
	/// @return Number of tasks currently being executed
	//----------------------------------------------------
	u32 CTaskScheduler::GetNumActiveTasks() 
	{
		return pThreadPool->active();
	}
	//----------------------------------------------------
	/// Get Num Pending Tasks
	///
	/// @return Number of tasks waiting to be executed
	//----------------------------------------------------
	u32 CTaskScheduler::GetNumPendingTasks() 
	{
		return pThreadPool->pending();
	}
	//----------------------------------------------------
	/// Clear
	///
	/// Unschedule all pending tasks
	//----------------------------------------------------
	void CTaskScheduler::Clear()
	{
		pThreadPool->clear();
	}
	//----------------------------------------------------
	/// Wait
	///
	/// This will cause the main thread to block
	/// until the queue is empty and all tasks have been
	/// processed
	//----------------------------------------------------
	void CTaskScheduler::Wait()
	{
		pThreadPool->wait();
	}
	//----------------------------------------------------
	/// Execute Main Thread Tasks
	///
	/// Execute any tasks that have been scehduled
	/// for the main thread
	//----------------------------------------------------
	void CTaskScheduler::ExecuteMainThreadTasks()
	{
		CThread::RecursiveScopedLock Lock(MainThreadTaskMutex);
		
		for (u32 i = 0; i < MainThreadTasks.size(); ++i)
		{
			GenericTaskType task = MainThreadTasks[i];
			task();
		}
		
		MainThreadTasks.clear();
	}
}

