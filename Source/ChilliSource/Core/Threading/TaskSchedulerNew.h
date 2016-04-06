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

#ifndef _CHILLISOURCE_CORE_THREADING_TASKSCHEDULER_H_New
#define _CHILLISOURCE_CORE_THREADING_TASKSCHEDULER_H_New

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Core/Threading/ThreadPool.h>

#include <atomic>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------------
        /// A system for executing task on the main thread or background threads.
        /// Several types of task are provided, each of which are processed in different
        /// ways. See TaskType.h for more information on these types.
        ///
        /// The task scheduler can be used during the OnInit and OnDestroy application
        /// lifecycle events.
        ///
        /// This is thread-safe.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class TaskSchedulerNew final : public AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(TaskSchedulerNew);
            //------------------------------------------------------------------------------
            /// A delegate describing a single task. A context is provided which provides
            /// information on the task and provides the ability to launch child tasks or
            /// yeild.
            ///
            /// @author Ian Copland
            ///
            /// @param in_taskContext - The task context.
            //------------------------------------------------------------------------------
            using Task = std::function<void(const TaskContext& in_taskContext) noexcept>;
            //------------------------------------------------------------------------------
            /// A delegate describing a single task with no context.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            using SimpleTask = std::function<void() noexcept>;
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
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const noexcept override;
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
            /// A task context is provided for launching child tasks or yeilding.
            ///
            /// @author Ian Copland
            ///
            /// @param in_taskType - The type of task.
            /// @param in_task - The task which should be scheduled.
            //------------------------------------------------------------------------------
            void ScheduleTask(TaskType in_taskType, const Task& in_task) noexcept;
            //------------------------------------------------------------------------------
            /// A convenience method for scheduling a single simple task which will be
            /// executed in a manner dependant on the task type.
            ///
            /// A simple task doesn't provide a task context for scheduling children or
            /// yeilding.
            ///
            /// @author Ian Copland
            ///
            /// @param in_taskType - The type of task.
            /// @param in_task - The simple task which should be scheduled.
            //------------------------------------------------------------------------------
            void ScheduleTask(TaskType in_taskType, const SimpleTask& in_task) noexcept;
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
            /// @param in_tasks - The tasks which should be scheduled.
            /// @param in_completionTask - [Optional] A task which is scheduled when the
            /// other tasks have all completed.
            //------------------------------------------------------------------------------
            void ScheduleTasks(TaskType in_taskType, const std::vector<Task>& in_tasks, const Task& in_completionTask = nullptr) noexcept;
            //------------------------------------------------------------------------------
            /// A convience method for scheduling a batch of simple tasks which will be
            /// executed in a manner dependant on the task type. Once all tasks have
            /// finished executing a completion task will be scheduled.
            ///
            /// All tasks, including the completion task, will be of the same type.
            ///
            /// A simple task doesn't provide a task context for scheduling children or
            /// yeilding.
            ///
            /// @author Ian Copland
            ///
            /// @param in_taskType - The type of task.
            /// @param in_tasks - The tasks which should be scheduled.
            /// @param in_completionTask - [Optional] A task which is scheduled when the
            /// other tasks have all completed.
            //------------------------------------------------------------------------------
            void ScheduleTasks(TaskType in_taskType, const std::vector<SimpleTask>& in_tasks, const SimpleTask& in_completionTask = nullptr) noexcept;
            
        private:
            friend class Application;
            //------------------------------------------------------------------------------
            /// A factory method for creating new instances of the task scheduler.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance of the system.
            //------------------------------------------------------------------------------
            static TaskSchedulerNewUPtr Create() noexcept;
            //------------------------------------------------------------------------------
            /// Constructor. Declared private to ensure the system is created through
            /// Application::CreateSystem<TaskScheduler>().
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            TaskSchedulerNew() noexcept;
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
            /// TODO: !?
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void StartNextFileTask(const Task& in_task) noexcept;
            //------------------------------------------------------------------------------
            /// Initialises the Task Scheduler, creating all threads.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() noexcept override;
            //------------------------------------------------------------------------------
            /// Cleans up the Task Scheduler, joining on all existing threads and then
            /// destroying them.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy() noexcept override;
            
            ThreadPoolUPtr m_smallTaskThreadPool;
            ThreadPoolUPtr m_largeTaskThreadPool;
        
            std::recursive_mutex m_mainThreadTaskMutex;
            std::vector<Task> m_mainThreadTasks;
            
            std::atomic<u32> m_gameLogicTaskCount;
            std::condition_variable_any m_gameLogicTaskCondition;
            
            std::mutex m_fileTaskMutex;
            bool m_isFileTaskRunning = false;
            std::queue<Task> m_fileTaskQueue;
            
#ifndef CS_TARGETPLATFORM_ANDROID
            std::thread::id m_mainThreadId;
#endif
        };
    }
}

#endif
