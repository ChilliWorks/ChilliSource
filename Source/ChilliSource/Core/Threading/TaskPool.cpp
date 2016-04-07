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
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TaskPool::TaskPool(u32 in_numThreads) noexcept
            : m_numThreads(in_numThreads), m_isFinished(false), m_taskCountHeuristic(0)
        {
            for (u32 i = 0; i < m_numThreads; ++i)
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
            std::unique_lock<std::mutex> queueLock(m_taskQueueMutex);
            CS_ASSERT(m_isFinished == false, "Task is being pushed after finishing.");
            
            m_taskQueue.push(in_task);
            ++m_taskCountHeuristic;
            
            queueLock.unlock();
            
            m_emptyWaitCondition.notify_one();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskPool::PerformTask(const std::atomic<bool>& in_forceContinue) noexcept
        {
            std::unique_lock<std::mutex> queueLock(m_taskQueueMutex);
            CS_ASSERT(m_isFinished == false, "Task is being pushed after finishing.");
            
            if (m_taskQueue.empty() && !in_forceContinue)
            {
                m_emptyWaitCondition.wait(queueLock);
            }
            
            if (m_taskQueue.empty())
            {
                return;
            }
            
            Task task = m_taskQueue.front();
            m_taskQueue.pop();
            --m_taskCountHeuristic;
                
            queueLock.unlock();
            
            task();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskPool::AwakenAllThreads() noexcept
        {
            m_emptyWaitCondition.notify_all();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskPool::ProcessTasks() noexcept
        {
#ifdef CS_TARGETPLATFORM_ANDROID
            CSBackend::Android::JavaVirtualMachine::Get()->AttachCurrentThread();
#endif

            while (!m_isFinished || m_taskCountHeuristic > 0)
            {
                PerformTask(m_isFinished);
            }
            
#ifdef CS_TARGETPLATFORM_ANDROID
            CSBackend::Android::JavaVirtualMachine::Get()->DetachCurrentThread();
#endif
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TaskPool::~TaskPool() noexcept
        {
            std::unique_lock<std::mutex> queueLock(m_taskQueueMutex);
            m_isFinished = true;
            queueLock.unlock();
            
            m_emptyWaitCondition.notify_all();
            for (auto& thread : m_threads)
            {
                thread.join();
            }
        }
    }
}
