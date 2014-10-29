//
//  ThreadPool.h
//  Chilli Source
//  Created by Scott Downie on 17/02/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef _CHILLISOURCE_CORE_THREADING_THREADPOOL_H_
#define _CHILLISOURCE_CORE_THREADING_THREADPOOL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/concurrent_worker_queue.h>

#include <atomic>
#include <thread>
#include <vector>

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
            void Schedule(const GenericTaskType& in_task);
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

			using ThreadGroup = std::vector<std::thread>;
			using TaskQueue = concurrent_worker_queue<GenericTaskType>;
			using TaskQueueUPtr = std::unique_ptr<TaskQueue>;


            ThreadGroup m_threadGroup;
			TaskQueueUPtr m_tasks;
            
            std::atomic<bool> m_isFinished;
        };
    }
}

#endif
