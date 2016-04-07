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
#include <ChilliSource/Core/System/AppSystem.h>

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

namespace ChilliSource
{
    namespace Core
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
            /// A delegate describing a single task which can be added to the task pool.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            using Task = std::function<void() noexcept>;
            //------------------------------------------------------------------------------
            /// The default constructor.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            MainThreadTaskPool() = default;
            //------------------------------------------------------------------------------
            /// Adds a task to the pool. The task will be executed when PerformTasks() is
            /// called.
            ///
            /// @author Ian Copland
            ///
            /// @param in_task - The task which should be added to the pool.
            //------------------------------------------------------------------------------
            void Add(const Task& in_task) noexcept;
            //------------------------------------------------------------------------------
            /// Performs all tasks in the task pool.
            ///
            /// This must be called from the main thread.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void PerformTasks() noexcept;
            
        private:
            //------------------------------------------------------------------------------
            /// Tries to get the next task from the local task queue. If the queue is empty
            /// it will take all tasks from the global queue. If the queue is still empty it
            /// will return a failure.
            ///
            /// This must be called on the main thread.
            ///
            /// @author Ian Copland
            ///
            /// @param out_task - [Out] The output task.
            ///
            /// @return Whether or not a task was successfully retreived.
            //------------------------------------------------------------------------------
            bool TryGetNextTask(Task& out_task);
            
            std::deque<Task> m_globalTaskQueue;
            std::mutex m_globalTaskQueueMutex;
            
            std::deque<Task> m_localTaskQueue;
        };
    }
}

#endif
