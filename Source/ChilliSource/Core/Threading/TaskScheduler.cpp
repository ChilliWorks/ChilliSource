/*
 *  TaskScheduler.cpp
 *  moFlow
 *
 *  Created by Scott Downie on 09/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace ChilliSource
{
    namespace Core
    {
        Core::ThreadPool* TaskScheduler::pThreadPool = nullptr;
        
        std::recursive_mutex TaskScheduler::MainThreadTaskMutex;
        std::vector<TaskScheduler::GenericTaskType> TaskScheduler::MainThreadTasks;

        //-------------------------------------------------
        /// Init
        ///
        /// Called by the application to create a thread
        /// pool with the optimum number of threads for
        /// the hardware
        ///
        /// @param Number of threads
        //------------------------------------------------
        void TaskScheduler::Init(u32 udwNumThreads)
        {
            pThreadPool = new Core::ThreadPool(udwNumThreads);
        }
        //----------------------------------------------------
        /// Get Num Pending Tasks
        ///
        /// @return Number of tasks waiting to be executed
        //----------------------------------------------------
        u32 TaskScheduler::GetNumPendingTasks() 
        {
            return pThreadPool->GetNumQueuedTasks();
        }
        //----------------------------------------------------
        /// Clear
        ///
        /// Unschedule all pending tasks
        //----------------------------------------------------
        void TaskScheduler::Clear()
        {
            pThreadPool->ClearQueuedTasks();
        }
        //----------------------------------------------------
        /// Execute Main Thread Tasks
        ///
        /// Execute any tasks that have been scehduled
        /// for the main thread
        //----------------------------------------------------
        void TaskScheduler::ExecuteMainThreadTasks()
        {
            std::unique_lock<std::recursive_mutex> Lock(MainThreadTaskMutex);
            
            for (u32 i = 0; i < MainThreadTasks.size(); ++i)
            {
                GenericTaskType task = MainThreadTasks[i];
                task();
            }
            
            MainThreadTasks.clear();
        }
    }
}

