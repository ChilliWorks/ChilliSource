// ThreadPool.h
// ChilliSource
//
// Created by Scott Downie on 17/02/2014
// Copyright Tag Games 2014. All rights reserved.

#ifndef _CHILLISOURCE_CORE_THREADING_THREADPOOL_H_
#define _CHILLISOURCE_CORE_THREADING_THREADPOOL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/WorkerQueue.h>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace ChilliSource
{
    namespace Core
    {
        class ThreadPool
        {
        public:

            typedef std::function<void()> GenericTaskType;
            
            //------------------------------------------------
            /// Constructor
            ///
            /// @author Scott Downie
            ///
            /// @param Num threads in pool
            //------------------------------------------------
            ThreadPool(u32 in_numThreads);
            //----------------------------------------------
            /// Add a task to the queue to be serviced
            /// when a thread becomes available
            ///
            /// @author Scott Downie
            ///
            /// @param Task to execute.
            //----------------------------------------------
            void Schedule(const GenericTaskType& inTask);
            //----------------------------------------------
            /// @author Scott Downie
            ///
            /// @return Num pending tasks in the queue
            //----------------------------------------------
            u32 GetNumQueuedTasks() const;
            //----------------------------------------------
            /// Discard all pending tasks from the queue
            ///
            /// @author Scott Downie
            //----------------------------------------------
            void ClearQueuedTasks();
            //----------------------------------------------
            /// Destructor
            //----------------------------------------------
            ~ThreadPool();
        private:

            //----------------------------------------------
            /// Continues to perform tasks until there are
            /// none left to perform at which point this
            /// will sleep until there are new tasks.
            /// @return The task.
            //----------------------------------------------
            void DoTaskOrWait();

            typedef std::vector<std::thread> ThreadGroup;
            typedef WorkerQueue<GenericTaskType> TaskQueue;

            ThreadGroup m_threadGroup;
            TaskQueue m_tasks;
            
            std::mutex m_taskMutex;
            std::condition_variable m_awakeCondition;
            std::atomic<bool> m_isFinished;
        };
    }
}

#endif
