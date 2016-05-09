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
#include <ChilliSource/Core/Threading/Task.h>

#include <vector>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// Provides information on a task and the means for it to execute child tasks.
    ///
    /// This is immutable and thread-safe.
    ///
    /// @author Ian Copland
    //------------------------------------------------------------------------------
    class TaskContext final
    {
    public:
        //------------------------------------------------------------------------------
        /// Constructs a task context of the given type.
        ///
        /// @author Ian Copland
        ///
        /// @param in_taskType - The type of task this context represents.
        /// @param in_taskPool - The task pool that this task and its children should be
        /// run within. This should not be provided for main thread and file task types.
        //------------------------------------------------------------------------------
        TaskContext(TaskType in_taskType, TaskPool* in_taskPool = nullptr) noexcept;
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The type of task this context represents.
        //------------------------------------------------------------------------------
        TaskType GetType() const noexcept;
        //------------------------------------------------------------------------------
        /// Schedules the given child tasks and yields until they have completed. Child
        /// tasks must be of the same type as the parent.
        ///
        /// Child tasks are provided with a task context.
        ///
        /// @author Ian Copland
        ///
        /// @param in_tasks - The tasks to be processed.
        //------------------------------------------------------------------------------
        void ProcessChildTasks(const std::vector<Task>& in_tasks) const noexcept;
        
        TaskType m_taskType;
        TaskPool* m_taskPool = nullptr;
    };
}

#endif
