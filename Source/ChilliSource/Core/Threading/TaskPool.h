//
//  TaskPool.h
//  ChilliSource
//  Created by Ian Copland on 06/04/2016.
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

#ifndef _CHILLISOURCE_CORE_THREADING_TASKPOOL_H_
#define _CHILLISOURCE_CORE_THREADING_TASKPOOL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Threading/TaskContext.h>

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// A collection of tasks which will be performed on one of the worker threads
    /// owned by the pool.
    ///
    /// Currently this does no task stealing or other clever tricks for performance;
    /// all tasks are drawn from a single global queue which is locked on every
    /// access.
    ///
    /// This is thread-safe.
    ///
    /// @author Ian Copland
    //------------------------------------------------------------------------------
    class TaskPool final
    {
    public:
        CS_DECLARE_NOCOPY(TaskPool);
        //------------------------------------------------------------------------------
        /// Constructs a new task pool with the given number of threads.
        ///
        /// @author Ian Copland
        ///
        /// @param in_taskType - The type of task this task pool is for. Only small and
        /// large can be specified here.
        /// @param in_numThreads - The number of threads that this task pool should
        /// create to run tasks on.
        //------------------------------------------------------------------------------
        TaskPool(TaskType in_taskType, u32 in_numThreads) noexcept;
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The number of threads that this task pool has to run tasks on.
        //------------------------------------------------------------------------------
        u32 GetNumThreads() const noexcept;
        //------------------------------------------------------------------------------
        /// Adds a series of tasks to the pool. These task will be executed as soon as a
        /// thread becomes free.
        ///
        /// @author Ian Copland
        ///
        /// @param in_tasks - The tasks to be added to the pool.
        //------------------------------------------------------------------------------
        void AddTasks(const std::vector<Task>& in_tasks) noexcept;
        //------------------------------------------------------------------------------
        /// Performs the given series of tasks and yields until they are finished. While
        /// yielding, other tasks will be processed.
        ///
        /// @author Ian Copland
        ///
        /// @param in_tasks - The tasks to be added to the pool.
        //------------------------------------------------------------------------------
        void AddTasksAndYield(const std::vector<Task>& in_tasks) noexcept;
        //------------------------------------------------------------------------------
        /// Waits for any currently running tasks to finish then joins all owned threads.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        ~TaskPool() noexcept;
        
    private:
        //------------------------------------------------------------------------------
        /// Performs a task from the task pool. This must be called from one of the
        /// threads owned by the task pool.
        ///
        /// A flag is provided which can be changed by other threads to notify that
        /// the current thread should continue regardless of whether there are any tasks
        /// available. AwakenAllThreads() can be used in conjunction with this flag.
        ///
        /// @author Ian Copland
        ///
        /// @param in_forceContinue - The force continue flag.
        //------------------------------------------------------------------------------
        void PerformTask(const std::atomic<bool>& in_forceContinue) noexcept;
        //------------------------------------------------------------------------------
        /// Continues to perform tasks until the task pool is deallocated. If there are
        /// no tasks currently available this will sleep until a task is added.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void ProcessTasks() noexcept;
        
        const u32 m_numThreads;
        const TaskContext m_taskContext;

        std::vector<std::thread> m_threads;
        
        std::atomic<u32> m_taskCountHeuristic;
        std::deque<Task> m_taskQueue;
        std::mutex m_taskQueueMutex;
        std::condition_variable m_emptyWaitCondition;
        
        std::atomic<bool> m_isFinished;
    };
}

#endif
