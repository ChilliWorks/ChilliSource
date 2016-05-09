//
//  MainThreadTaskPool.h
//  ChilliSource
//  Created by Ian Copland on 07/04/2016.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_THREADING_MAINTHREADTASKPOOL_H_
#define _CHILLISOURCE_CORE_THREADING_MAINTHREADTASKPOOL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Threading/TaskContext.h>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// A collection of tasks which will be performed on the main thread.
    ///
    /// Adding tasks is thread safe, but the perform task methods must be called on
    /// the main thread.
    ///
    /// @author Ian Copland
    //------------------------------------------------------------------------------
    class MainThreadTaskPool final
    {
    public:
        CS_DECLARE_NOCOPY(MainThreadTaskPool);
        //------------------------------------------------------------------------------
        /// Constructs a new main thread task pool.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        MainThreadTaskPool();
        //------------------------------------------------------------------------------
        /// Adds a series of tasks to the pool. The tasks will be executed when 
        /// PerformTasks() is called.
        ///
        /// @author Ian Copland
        ///
        /// @param in_tasks - The tasks to be added to the pool.
        //------------------------------------------------------------------------------
        void AddTasks(const std::vector<Task>& in_tasks) noexcept;
        //------------------------------------------------------------------------------
        /// Performs all tasks in the task pool. The task queue is copied locally and
        /// cleared before processing all tasks. This means that any tasks queued while
        /// performing main thread tasks will be perfomed during the next call to
        /// PerformTasks() rather than the current one.
        ///
        /// This must be called from the main thread.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void PerformTasks() noexcept;
        
        const TaskContext m_taskContext;
        
        std::vector<Task> m_taskQueue;
        std::mutex m_taskQueueMutex;
    };
}

#endif
