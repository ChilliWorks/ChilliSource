//
//  TaskPool.cpp
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

#include <ChilliSource/Core/Threading/TaskPool.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>

#include <sstream>

namespace ChilliSource
{
    namespace Core
    {
        namespace
        {
            //------------------------------------------------------------------------------
            /// Navigates the list of threads to query if one of the threads has the given
            /// thread id.
            ///
            /// @param in_threadList - The list of threads.
            /// @param in_id - The thread id to look for.
            ///
            ///
            /// @return Whether or not one of the given threads has the given id.
            //------------------------------------------------------------------------------
            bool ThreadListHasId(const std::vector<std::thread>& in_threadList, std::thread::id in_id)
            {
                for (const auto& thread : in_threadList)
                {
                    if (thread.get_id() == in_id)
                    {
                        return true;
                    }
                }
                
                return false;
            }
        }
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TaskPool::TaskPool(u32 in_numThreads) noexcept
            : m_numThreads(in_numThreads), m_isFinished(false)
        {
            for (u32 i=0; i < m_numThreads; ++i)
            {
                m_threads.push_back(std::thread(MakeDelegate(this, &TaskPool::ProcessTasks)));
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 TaskPool::GetNumThreads() const noexcept
        {
            return m_numThreads;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskPool::Add(const Task& in_task) noexcept
        {
            m_tasks.push(in_task);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskPool::PerformTaskOrWait() noexcept
        {
            CS_ASSERT(ThreadListHasId(m_threads, std::this_thread::get_id()), "Tasks can only be performed on a thread owned by the task pool.");

            Task task;
            if (m_tasks.pop_or_wait(task))
            {
                task();
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskPool::ProcessTasks() noexcept
        {
#ifdef CS_TARGETPLATFORM_ANDROID
            CSBackend::Android::JavaVirtualMachine::Get()->AttachCurrentThread();
#endif

            while (!m_isFinished)
            {
                PerformTaskOrWait();
            }
            
#ifdef CS_TARGETPLATFORM_ANDROID
            CSBackend::Android::JavaVirtualMachine::Get()->DetachCurrentThread();
#endif
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TaskPool::~TaskPool() noexcept
        {
            m_isFinished = true;
            m_tasks.abort();
            
            for (auto& thread : m_threads)
            {
                thread.join();
            }
        }
    }
}
