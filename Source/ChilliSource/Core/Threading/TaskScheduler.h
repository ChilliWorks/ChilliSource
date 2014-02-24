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

#include <ChilliSource/Core/Threading/ThreadPool.h>
#include <ChilliSource/Core/Threading/Tasks.hpp>

namespace ChilliSource
{
	class CTaskScheduler
	{
	public:
	
		typedef std::function<void()> GenericTaskType;
		
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
			pThreadPool->Schedule(insTask);
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
				pThreadPool->Schedule(Task1<TArg>(inpSender, func, Value, inpWaitCondition));
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
				pThreadPool->Schedule(Task1<TArg>(inpSender, func, Value, inpWaitCondition));
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
			std::unique_lock<std::recursive_mutex> Lock(MainThreadTaskMutex);
			MainThreadTasks.push_back(insTask);
		}
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
		/// Execute Main Thread Tasks
		///
		/// Execute any tasks that have been scehduled
		/// for the main thread
		//----------------------------------------------------
		static void ExecuteMainThreadTasks();
		
	private:
	
		static Core::ThreadPool* pThreadPool;
		
		static std::recursive_mutex MainThreadTaskMutex;
		static DYNAMIC_ARRAY<GenericTaskType> MainThreadTasks;
	};
}

#endif
