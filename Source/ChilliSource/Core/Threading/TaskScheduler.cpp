//
//  TaskScheduler.cpp
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

#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/Threading/TaskContext.h>
#include <ChilliSource/Core/Threading/TaskType.h>

#ifdef CS_TARGETPLATFORM_ANDROID
#   include <CSBackend/Platform/Android/Main/JNI/Core/Threading/MainThreadId.h>
#endif

#include <algorithm>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(TaskScheduler);
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    TaskSchedulerUPtr TaskScheduler::Create() noexcept
    {
        return TaskSchedulerUPtr(new TaskScheduler());
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    TaskScheduler::TaskScheduler() noexcept
        : m_gameLogicTaskCount(0)
    {
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool TaskScheduler::IsA(InterfaceIDType in_interfaceId) const noexcept
    {
        return (TaskScheduler::InterfaceID == in_interfaceId);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool TaskScheduler::IsMainThread() const noexcept
    {
#ifdef CS_TARGETPLATFORM_ANDROID
        return CSBackend::Android::MainThreadId::Get()->GetId() == std::this_thread::get_id();
#else
        return m_mainThreadId == std::this_thread::get_id();
#endif
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TaskScheduler::ScheduleTask(TaskType in_taskType, const Task& in_task) noexcept
    {
        std::vector<Task> tasks = { in_task };
        ScheduleTasks(in_taskType, tasks);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TaskScheduler::ScheduleTasks(TaskType in_taskType, const std::vector<Task>& in_tasks) noexcept
    {
        switch (in_taskType)
        {
            case TaskType::k_small:
            {
                m_smallTaskPool->AddTasks(in_tasks);
                break;
            }
            case TaskType::k_large:
            {
                m_largeTaskPool->AddTasks(in_tasks);
                break;
            }
            case TaskType::k_mainThread:
            {
                m_mainThreadTaskPool->AddTasks(in_tasks);
                break;
            }
            case TaskType::k_gameLogic:
            {
                std::vector<Task> gameLogicTasks;
                gameLogicTasks.reserve(in_tasks.size());
                for (const auto& task : in_tasks)
                {
                    gameLogicTasks.push_back([=](const TaskContext&)
                    {
                        task(TaskContext(TaskType::k_gameLogic, m_smallTaskPool.get()));

                        if (--m_gameLogicTaskCount == 0)
                        {
                            m_gameLogicTaskCondition.notify_all();
                        }
                    });
                }

                m_gameLogicTaskCount += u32(gameLogicTasks.size());
                m_smallTaskPool->AddTasks(gameLogicTasks);
                break;
            }
            case TaskType::k_file:
            {
                std::unique_lock<std::mutex> lock(m_fileTaskMutex);

                m_fileTaskQueue.insert(m_fileTaskQueue.begin(), in_tasks.begin(), in_tasks.end());

                if (!m_isFileTaskRunning)
                {
                    m_isFileTaskRunning = true;
                    auto task = m_fileTaskQueue.front();
                    m_fileTaskQueue.pop_front();
                    lock.unlock();

                    StartNextFileTask(task);
                }
                break;
            }
        }
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TaskScheduler::ScheduleTasks(TaskType in_taskType, const std::vector<Task>& in_tasks, const Task& in_completionTask) noexcept
    {
        //TODO: This should be allocated from a pool to reduce memory fragmentation.
        auto taskCount = std::make_shared<std::atomic<u32>>(u32(in_tasks.size()));
        
        std::vector<Task> tasksWithCounter;
        tasksWithCounter.reserve(in_tasks.size());

        for (const auto& task : in_tasks)
        {
            tasksWithCounter.push_back([=](const TaskContext& in_taskContext)
            {
                task(in_taskContext);
                
                if (--(*taskCount) == 0)
                {
                    ScheduleTask(in_taskType, in_completionTask);
                }
            });
        }

        ScheduleTasks(in_taskType, tasksWithCounter);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TaskScheduler::ExecuteMainThreadTasks() noexcept
    {
        //wait on all game logic tasks completing.
        std::unique_lock<std::mutex> lock(m_gameLogicTaskMutex);
        
        while (m_gameLogicTaskCount != 0)
        {
            m_gameLogicTaskCondition.wait(lock);
        }
        
        lock.unlock();
        
        m_mainThreadTaskPool->PerformTasks();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TaskScheduler::StartNextFileTask(const Task& in_task) noexcept
    {
        std::vector<Task> tasks;
        tasks.reserve(1);

        tasks.push_back([=](const TaskContext& in_taskContext)
        {
            in_task(TaskContext(TaskType::k_file));
            
            std::unique_lock<std::mutex> lock(m_fileTaskMutex);
            
            if (m_fileTaskQueue.empty())
            {
                m_isFileTaskRunning = false;
                return;
            }
            
            auto task = m_fileTaskQueue.front();
            m_fileTaskQueue.pop_front();
            
            lock.unlock();
            
            StartNextFileTask(task);
        });

        m_largeTaskPool->AddTasks(tasks);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TaskScheduler::OnInit() noexcept
    {
        constexpr s32 k_minThreadsPerPool = 2;
        constexpr s32 k_namedThreads = 1; //The main thread.
        
        Device* device = Application::Get()->GetSystem<Device>();
        
        s32 numFreeCores = s32(device->GetNumberOfCPUCores()) - k_namedThreads;
        s32 threadsPerPool = std::max(k_minThreadsPerPool, numFreeCores);
        
        m_smallTaskPool = TaskPoolUPtr(new TaskPool(TaskType::k_small, threadsPerPool));
        m_largeTaskPool = TaskPoolUPtr(new TaskPool(TaskType::k_large, threadsPerPool));
        m_mainThreadTaskPool = MainThreadTaskPoolUPtr(new MainThreadTaskPool());
        
#ifndef CS_TARGETPLATFORM_ANDROID
        m_mainThreadId = std::this_thread::get_id();
#endif
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void TaskScheduler::Destroy() noexcept
    {
        m_smallTaskPool.reset();
        m_largeTaskPool.reset();
        m_mainThreadTaskPool.reset();
    }
}