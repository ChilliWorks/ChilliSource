/*
 *  TaskScheduler.h
 *  moFlow
 *
 *  Created by Scott Downie on 09/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_TASK_SCHEDULER_H_
#define _MO_FLO_CORE_TASK_SCHEDULER_H_

#include <moFlo/Core/boost/threadpool.hpp>

#include <moFlo/Core/Tasks.hpp>
#include <moFlo/Core/Thread.h>

namespace moFlo
{
	class CTaskScheduler
	{
	public:
	
		typedef boost::threadpool::task_func GenericTaskType;
		
		//-------------------------------------------------
		/// Init
		///
		/// Called by the application to create a thread
		/// pool with the optimum number of threads for
		/// the hardware
		///
		/// @param Number of threads
		//------------------------------------------------
		static void Init(u32 udwNumThreads);
		//------------------------------------------------
		/// Schedule Task
		///
		/// The task delegate will be placed into the
		/// task queue and be performed when a thread
		/// becomes available
		///
		/// @param Task delegate
		//------------------------------------------------
		static void ScheduleTask(const GenericTaskType& insTask)
		{
			pThreadPool->schedule(insTask);
		}
		//------------------------------------------------
		/// For Each
		///
		/// The given function will be applied to the
		/// value of each element in parallel. 
		///
		/// @param Iterator pointing to beginning
		/// @param Iterator pointing to end
		/// @param Task delegate callee
		/// @param Delegate
		/// @param Optional wait condition
		//------------------------------------------------
		template <typename TSender, typename TDelegate, typename TIterator, typename TArg>
		static void ForEach(TIterator inIt, TIterator inItEnd, TSender* inpSender, void (TDelegate::*func)(TArg inArg1), CWaitCondition* inpWaitCondition = NULL)
		{
			//Unlike usual tasks the wait condition is shared between each scheduled task
			//and threfore the condition isn't one but the number of iterations
			for(; inIt != inItEnd; ++inIt)
			{
				TArg Value = (*inIt);
				pThreadPool->schedule(Task1<TArg>(inpSender, func, Value, inpWaitCondition));
			}
		}
		//------------------------------------------------
		/// For Each (Const)
		///
		/// The given function will be applied to the
		/// value of each element in parallel. 
		///
		/// @param Iterator pointing to beginning
		/// @param Iterator pointing to end
		/// @param Task delegate callee
		/// @param Delegate
		/// @param Optional wait condition
		//------------------------------------------------
		template <typename TSender, typename TDelegate, typename TIterator, typename TArg>
		static void ForEach(TIterator inIt, TIterator inItEnd, TSender* inpSender, void (TDelegate::*func)(TArg inArg1) const, CWaitCondition* inpWaitCondition = NULL)
		{
			//Unlike usual tasks the wait condition is shared between each scheduled task
			//and threfore the condition isn't one but the number of iterations
			for(; inIt != inItEnd; ++inIt)
			{
				TArg Value = (*inIt);
				pThreadPool->schedule(Task1<TArg>(inpSender, func, Value, inpWaitCondition));
			}
		}
		//----------------------------------------------------
		/// Schedule Main Thread Message
		///
		/// Schedule a delegate to be executed by the main
		/// thread
		//----------------------------------------------------
		static void ScheduleMainThreadTask(const GenericTaskType& insTask)
		{
			CThread::RecursiveScopedLock Lock(MainThreadTaskMutex);
			MainThreadTasks.push_back(insTask);
		}
		//----------------------------------------------------
		/// Get Num Active Tasks
		///
		/// @return Number of tasks currently being executed
		//----------------------------------------------------
		static u32 GetNumActiveTasks();
		//----------------------------------------------------
		/// Get Num Pending Tasks
		///
		/// @return Number of tasks waiting to be executed
		//----------------------------------------------------
		static u32 GetNumPendingTasks();
		//----------------------------------------------------
		/// Clear
		///
		/// Unschedule all pending tasks
		//----------------------------------------------------
		static void Clear();
		//----------------------------------------------------
		/// Wait
		///
		/// This will cause the main thread to block
		/// until the queue is empty and all tasks have been
		/// processed
		//----------------------------------------------------
		static void Wait();
		//----------------------------------------------------
		/// Execute Main Thread Tasks
		///
		/// Execute any tasks that have been scehduled
		/// for the main thread
		//----------------------------------------------------
		static void ExecuteMainThreadTasks();
		
	private:
	
		static boost::threadpool::pool* pThreadPool;
		
		static CThread::RecursiveMutex MainThreadTaskMutex;
		static DYNAMIC_ARRAY<GenericTaskType> MainThreadTasks;
	};
}

#endif
