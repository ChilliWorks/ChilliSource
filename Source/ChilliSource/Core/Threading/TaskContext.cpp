//
//  TaskContext.cpp
//  ChilliSource
//  Created by Ian Copland on 05/04/2016.
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

#include <ChilliSource/Core/Threading/TaskContext.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskType.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TaskContext::TaskContext(TaskType in_taskType, TaskPool* in_taskPool) noexcept
            : m_taskType(in_taskType), m_taskPool(in_taskPool)
        {
            if (m_taskType == TaskType::k_mainThread || m_taskType == TaskType::k_file)
            {
                CS_ASSERT(!m_taskPool, "Main thread and file task contexts should not have a task pool.");
            }
            else
            {
                CS_ASSERT(m_taskPool, "Small, large and game logic task contexts must have a task pool.");
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TaskType TaskContext::GetType() const noexcept
        {
            return m_taskType;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskContext::ProcessChildTask(const Task& in_task) const noexcept
        {
            std::vector<Task> tasks;
            tasks.push_back(in_task);
            
            ProcessChildTasks(tasks);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskContext::ProcessChildTasks(const std::vector<Task>& in_tasks) const noexcept
        {
            if (m_taskType == TaskType::k_mainThread || m_taskType == TaskType::k_file)
            {
                ProcessChildTasksInSeries(in_tasks);
            }
            else
            {
                ProcessChildTasksInParallel(in_tasks);
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskContext::ProcessChildTasksInParallel(const std::vector<Task>& in_tasks) const noexcept
        {
            CS_ASSERT(m_taskType == TaskType::k_small || m_taskType == TaskType::k_large || m_taskType == TaskType::k_gameLogic, "Invalid task type.");
            
            auto taskScheduler = Application::Get()->GetTaskScheduler();
            
            std::atomic<u32> taskCount(u32(in_tasks.size()));
            std::atomic<bool> finished(false);
            
            for (const auto& task : in_tasks)
            {
                taskScheduler->ScheduleTask(m_taskType, [=, &taskCount, &finished](const TaskContext&) noexcept
                {
                    task(*this);

                    if (--taskCount == 0)
                    {
                        finished = true;
                        if (m_taskPool)
                        {
                            m_taskPool->AwakenAllThreads();
                        }
                    }
                });
            }
            
            while (!finished)
            {
                m_taskPool->PerformTask(finished);
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskContext::ProcessChildTasksInSeries(const std::vector<Task>& in_tasks) const noexcept
        {
            CS_ASSERT(m_taskType == TaskType::k_mainThread || m_taskType == TaskType::k_file, "Invalid task type.");
            
            for (const auto& task : in_tasks)
            {
                task(*this);
            }
        }
    }
}