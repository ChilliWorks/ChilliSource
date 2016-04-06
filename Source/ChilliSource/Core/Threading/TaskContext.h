//
//  TaskContext.h
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

#ifndef _CHILLISOURCE_CORE_THREADING_TASKCONTEXT_H_
#define _CHILLISOURCE_CORE_THREADING_TASKCONTEXT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Threading/TaskSchedulerNew.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------------
        /// Provides information on a task and the means for it to execute child tasks
        /// and yeild.
        ///
        /// This is immutable and thread-safe.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class TaskContext final
        {
        public:
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_taskType - The type of task this context represents.
            //------------------------------------------------------------------------------
            TaskContext(TaskType in_taskType) noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The type of task this context represents.
            //------------------------------------------------------------------------------
            TaskType GetType() const noexcept;
            //------------------------------------------------------------------------------
            /// Schedules the given child task and yields until it has completed. Child
            /// tasks must be of the same type as the parent.
            ///
            /// Child tasks are provided with a task context.
            ///
            /// @author Ian Copland
            ///
            /// @param in_task - The task which should be processed.
            //------------------------------------------------------------------------------
            void ProcessChildTask(const TaskSchedulerNew::Task& in_task) const noexcept;
            //------------------------------------------------------------------------------
            /// Schedules the given child simple task and yields until it has completed.
            /// Child tasks must be of the same type as the parent.
            ///
            /// Child simple tasks are not provided with a task context.
            ///
            /// @author Ian Copland
            ///
            /// @param in_task - The task which should be processed.
            //------------------------------------------------------------------------------
            void ProcessChildTask(const TaskSchedulerNew::SimpleTask& in_task) const noexcept;
            //------------------------------------------------------------------------------
            /// Schedules the given child tasks and yields until they have completed. Child
            /// tasks must be of the same type as the parent.
            ///
            /// Child tasks are provided with a task context.
            ///
            /// @author Ian Copland
            ///
            /// @param in_tasks - The tasks which should be processed.
            //------------------------------------------------------------------------------
            void ProcessChildTasks(const std::vector<TaskSchedulerNew::Task>& in_task) const noexcept;
            //------------------------------------------------------------------------------
            /// Schedules the given child simple tasks and yields until they have completed.
            /// Child tasks must be of the same type as the parent.
            ///
            /// Child simple tasks are not provided with a task context.
            ///
            /// @author Ian Copland
            ///
            /// @param in_tasks - The tasks which should be processed.
            //------------------------------------------------------------------------------
            void ProcessChildTasks(const std::vector<TaskSchedulerNew::SimpleTask>& in_task) const noexcept;
            //------------------------------------------------------------------------------
            /// Yeilds for the given time, allowing other tasks to be processed. The given
            /// yield time is minimum; the actual time yeilded may be greater.
            ///
            /// @author Ian Copland
            ///
            /// @param in_yieldTimeMs
            //------------------------------------------------------------------------------
            void Yield(TimeIntervalMs in_yieldTimeMs) const noexcept;
            
        private:
            TaskType m_taskType;
        };
    }
}

#endif
