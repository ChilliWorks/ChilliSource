//
//  MainThreadTaskPool.cpp
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

#include <ChilliSource/Core/Threading/MainThreadTaskPool.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskSchedulerNew.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void MainThreadTaskPool::Add(const Task& in_task) noexcept
        {
            std::unique_lock<std::mutex> lock(m_globalTaskQueueMutex);
            m_globalTaskQueue.push_back(in_task);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void MainThreadTaskPool::PerformTasks() noexcept
        {
            CS_ASSERT(Application::Get()->GetTaskSchedulerNew()->IsMainThread(), "Main thread tasks cannot be performed on a background thread.");
            
            Task task;
            while (TryGetNextTask(task))
            {
                task();
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool MainThreadTaskPool::TryGetNextTask(Task& out_task)
        {
            CS_ASSERT(Application::Get()->GetTaskSchedulerNew()->IsMainThread(), "Main thread tasks cannot be performed on a background thread.");
            
            if (m_localTaskQueue.empty())
            {
                std::unique_lock<std::mutex> lock(m_globalTaskQueueMutex);
                
                m_localTaskQueue = m_globalTaskQueue;
                m_globalTaskQueue.clear();
            }
            
            if (m_localTaskQueue.empty())
            {
                return false;
            }
            
            out_task = m_localTaskQueue.front();
            m_localTaskQueue.pop_front();
            
            return true;
        }
    }
}
