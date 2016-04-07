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

#include <ChilliSource/Core/Threading/TaskSchedulerNew.h>

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
    namespace Core
    {
        CS_DEFINE_NAMEDTYPE(TaskSchedulerNew);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TaskSchedulerNewUPtr TaskSchedulerNew::Create() noexcept
        {
            return TaskSchedulerNewUPtr(new TaskSchedulerNew());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TaskSchedulerNew::TaskSchedulerNew() noexcept
            : m_gameLogicTaskCount(0)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool TaskSchedulerNew::IsA(CSCore::InterfaceIDType in_interfaceId) const noexcept
        {
            return (TaskSchedulerNew::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool TaskSchedulerNew::IsMainThread() const noexcept
        {
#ifdef CS_TARGETPLATFORM_ANDROID
            return CSBackend::Android::MainThreadId::Get()->GetId() == std::this_thread::get_id();
#else
            return m_mainThreadId == std::this_thread::get_id();
#endif
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskSchedulerNew::ScheduleTask(TaskType in_taskType, const Task& in_task) noexcept
        {
            switch (in_taskType)
            {
                case TaskType::k_small:
                {
                    m_smallTaskPool->Add(std::bind(in_task, TaskContext(in_taskType, m_smallTaskPool.get())));
                    break;
                }
                case TaskType::k_large:
                {
                    m_largeTaskPool->Add(std::bind(in_task, TaskContext(in_taskType, m_largeTaskPool.get())));
                    break;
                }
                case TaskType::k_mainThread:
                {
                    m_mainThreadTaskPool->Add(std::bind(in_task, TaskContext(in_taskType)));
                    break;
                }
                case TaskType::k_gameLogic:
                {
                    ++m_gameLogicTaskCount;
                    m_smallTaskPool->Add([=]()
                    {
                        in_task(TaskContext(in_taskType, m_smallTaskPool.get()));
                        
                        if (--m_gameLogicTaskCount == 0)
                        {
                            m_gameLogicTaskCondition.notify_all();
                        }
                    });
                    break;
                }
                case TaskType::k_file:
                {
                    std::unique_lock<std::mutex> lock(m_fileTaskMutex);
                    
                    if (m_isFileTaskRunning)
                    {
                        m_fileTaskQueue.push(in_task);
                        break;
                    }
                    
                    m_isFileTaskRunning = true;
                    lock.unlock();
                    
                    StartNextFileTask(in_task);
                    break;
                }
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskSchedulerNew::ScheduleTask(TaskType in_taskType, const SimpleTask& in_task) noexcept
        {
            ScheduleTask(in_taskType, [=](const TaskContext&)
            {
                in_task();
            });
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskSchedulerNew::ScheduleTasks(TaskType in_taskType, const std::vector<Task>& in_tasks, const Task& in_completionTask) noexcept
        {
            //TODO: Ideally this should be allocated from a pool.
            auto taskCount = std::make_shared<std::atomic<u32>>(in_tasks.size());
            
            for (const auto& task : in_tasks)
            {
                ScheduleTask(in_taskType, [=](const TaskContext& in_taskContext)
                {
                    task(in_taskContext);
                    
                    if (--(*taskCount) == 0)
                    {
                        ScheduleTask(in_taskType, in_completionTask);
                    }
                });
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskSchedulerNew::ScheduleTasks(TaskType in_taskType, const std::vector<SimpleTask>& in_tasks, const SimpleTask& in_completionTask) noexcept
        {
            std::vector<Task> nonSimpleTasks;
            
            for (const auto& simpleTask : in_tasks)
            {
                nonSimpleTasks.push_back([=](const TaskContext&)
                {
                    simpleTask();
                });
            }
            
            Task nonSimpleCompletionTask = nullptr;
            if (in_completionTask)
            {
                nonSimpleCompletionTask = [=](const TaskContext&)
                {
                    in_completionTask();
                };
            }
            
            ScheduleTasks(in_taskType, nonSimpleTasks, nonSimpleCompletionTask);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskSchedulerNew::ExecuteMainThreadTasks() noexcept
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
        void TaskSchedulerNew::StartNextFileTask(const Task& in_task) noexcept
        {
            m_largeTaskPool->Add([=]()
            {
                in_task(TaskContext(TaskType::k_file));
                
                std::unique_lock<std::mutex> lock(m_fileTaskMutex);
                
                if (m_fileTaskQueue.empty())
                {
                    m_isFileTaskRunning = false;
                    return;
                }
                
                auto task = m_fileTaskQueue.front();
                m_fileTaskQueue.pop();
                
                lock.unlock();
                
                StartNextFileTask(task);
            });
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskSchedulerNew::OnInit() noexcept
        {
            Device* device = Core::Application::Get()->GetSystem<Device>();
            
            constexpr s32 k_minThreadsPerPool = 2;
            constexpr s32 k_namedThreads = 2; //The main thread and the render thread.
            
            s32 numFreeCores = s32(device->GetNumberOfCPUCores()) - k_namedThreads;
            s32 threadsPerPool = std::max(k_minThreadsPerPool, numFreeCores);
            
            m_smallTaskPool = TaskPoolUPtr(new TaskPool(threadsPerPool));
            m_largeTaskPool = TaskPoolUPtr(new TaskPool(threadsPerPool));
            m_mainThreadTaskPool = MainThreadTaskPoolUPtr(new MainThreadTaskPool());
            
#ifndef CS_TARGETPLATFORM_ANDROID
            m_mainThreadId = std::this_thread::get_id();
#endif
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskSchedulerNew::OnDestroy() noexcept
        {
            m_smallTaskPool.reset();
            m_largeTaskPool.reset();
            m_mainThreadTaskPool.reset();
        }
    }
}