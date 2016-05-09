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
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    MainThreadTaskPool::MainThreadTaskPool()
        : m_taskContext(TaskType::k_mainThread)
    {
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void MainThreadTaskPool::AddTasks(const std::vector<Task>& in_tasks) noexcept
    {
        std::unique_lock<std::mutex> lock(m_taskQueueMutex);
        m_taskQueue.insert(m_taskQueue.end(), in_tasks.begin(), in_tasks.end());
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void MainThreadTaskPool::PerformTasks() noexcept
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread(), "Main thread tasks cannot be performed on a background thread.");
        
        std::unique_lock<std::mutex> lock(m_taskQueueMutex);
        auto localTaskQueue = m_taskQueue;
        m_taskQueue.clear();
        lock.unlock();

        for (const auto& task : localTaskQueue)
        {
            task(m_taskContext);
        }
    }
}
