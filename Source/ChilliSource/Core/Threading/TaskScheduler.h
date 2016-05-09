//
//  TaskScheduler.h
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

#ifndef _CHILLISOURCE_CORE_THREADING_TASKSCHEDULER_H_
#define _CHILLISOURCE_CORE_THREADING_TASKSCHEDULER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Core/Threading/MainThreadTaskPool.h>
#include <ChilliSource/Core/Threading/Task.h>
#include <ChilliSource/Core/Threading/TaskPool.h>
#include <ChilliSource/Core/Threading/TaskType.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// A system for executing task on the main thread or background threads.
    /// Several types of task are provided, each of which are processed in different
    /// ways. See TaskType.h for more information on these types.
    ///
    /// The task scheduler can be used during the application OnInit lifecycle event,
    /// but it should not be used during the OnDestroy event.
    ///
    /// This is thread-safe.
    ///
    /// @author Ian Copland
    //------------------------------------------------------------------------------
    class TaskScheduler final : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(TaskScheduler);
        //------------------------------------------------------------------------------
        /// Allows querying of whether or not this system implements the interface
        /// described by the given interface Id. Typically this is not called directly
        /// as the templated equivalent IsA<Interface>() is preferred.
        ///
        /// @author Ian Copland
        ///
        /// @param in_interfaceId - The interface Id.
        ///
        /// @return Whether or not the interface is implemented.
        //------------------------------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const noexcept override;
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return Whether or not the current thread is the main thread.
        //------------------------------------------------------------------------------
        bool IsMainThread() const noexcept;
        //------------------------------------------------------------------------------
        /// Schedules a single task which will be executed in a manner dependant on the
        /// task type.
        ///
        /// A task context is provided for launching child tasks.
        ///
        /// @author Ian Copland
        ///
        /// @param in_taskType - The type of task.
        /// @param in_task - The task to be scheduled.
        //------------------------------------------------------------------------------
        void ScheduleTask(TaskType in_taskType, const Task& in_task) noexcept;
        //------------------------------------------------------------------------------
        /// Schedules a batch of tasks which will be executed in a manner dependant on
        /// the task type.
        ///
        /// Task contexts are provided for launching child tasks.
        ///
        /// @author Ian Copland
        ///
        /// @param in_taskType - The type of task.
        /// @param in_tasks - The tasks to be scheduled.
        //------------------------------------------------------------------------------
        void ScheduleTasks(TaskType in_taskType, const std::vector<Task>& in_tasks) noexcept;
        //------------------------------------------------------------------------------
        /// Schedules a batch of tasks which will be executed in a manner dependant on
        /// the task type. Once all tasks have finished executing a completion task will
        /// be scheduled.
        ///
        /// All tasks, including the completion task, will be of the same type.
        ///
        /// Task contexts are provided for launching child tasks or yeilding.
        ///
        /// @author Ian Copland
        ///
        /// @param in_taskType - The type of task.
        /// @param in_tasks - The tasks to be scheduled.
        /// @param in_completionTask - A task which is scheduled when the other tasks 
        /// have all completed.
        //------------------------------------------------------------------------------
        void ScheduleTasks(TaskType in_taskType, const std::vector<Task>& in_tasks, const Task& in_completionTask) noexcept;
        
    private:
        friend class Application;
        //------------------------------------------------------------------------------
        /// A factory method for creating new instances of the task scheduler.
        ///
        /// @author Ian Copland
        ///
        /// @return The new instance of the system.
        //------------------------------------------------------------------------------
        static TaskSchedulerUPtr Create() noexcept;
        //------------------------------------------------------------------------------
        /// Constructor. Declared private to ensure the system is created through
        /// Application::CreateSystem<TaskScheduler>().
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        TaskScheduler() noexcept;
        //------------------------------------------------------------------------------
        /// Executes all main thread tasks. Prior to running them, this will wait on all
        /// game logic tasks completing.
        ///
        /// This must be called from the main thread.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void ExecuteMainThreadTasks() noexcept;
        //------------------------------------------------------------------------------
        /// Adds the given task to the large task pool. Once the task is complete then
        /// this is called again for the next task in the file queue. If the file queue
        /// is empty then flag indication whether or not file tasks are running is set
        /// to false.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void StartNextFileTask(const Task& in_task) noexcept;
        //------------------------------------------------------------------------------
        /// Cleans up the Task Scheduler, joining on all existing threads and then
        /// destroying them.
        ///
        /// This should be called prior to the Application OnDestroy lifecycle event is
        /// sent to each system.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void Destroy() noexcept;
        //------------------------------------------------------------------------------
        /// Initialises the Task Scheduler, creating all threads.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void OnInit() noexcept override;
        
        TaskPoolUPtr m_smallTaskPool;
        TaskPoolUPtr m_largeTaskPool;
        MainThreadTaskPoolUPtr m_mainThreadTaskPool;
        
        std::atomic<u32> m_gameLogicTaskCount;
        std::condition_variable m_gameLogicTaskCondition;
        std::mutex m_gameLogicTaskMutex;
        
        std::mutex m_fileTaskMutex;
        bool m_isFileTaskRunning = false;
        std::deque<Task> m_fileTaskQueue;
        
#ifndef CS_TARGETPLATFORM_ANDROID
        std::thread::id m_mainThreadId;
#endif
    };
}

#endif
