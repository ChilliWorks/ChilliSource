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
#include <ChilliSource/Core/Threading/TaskType.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TaskContext::TaskContext(TaskType in_taskType) noexcept
        {
            //TODO: !? Implement
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TaskType TaskContext::GetType() const noexcept
        {
            //TODO: !? Implement
            return TaskType::k_small;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskContext::ProcessChildTask(const TaskSchedulerNew::Task& in_task) const noexcept
        {
            //TODO: !? Implement
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskContext::ProcessChildTask(const TaskSchedulerNew::SimpleTask& in_task) const noexcept
        {
            //TODO: !? Implement
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskContext::ProcessChildTasks(const std::vector<TaskSchedulerNew::Task>& in_task) const noexcept
        {
            //TODO: !? Implement
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskContext::ProcessChildTasks(const std::vector<TaskSchedulerNew::SimpleTask>& in_task) const noexcept
        {
            //TODO: !? Implement
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void TaskContext::Yield(TimeIntervalMs in_yieldTimeMs) const noexcept
        {
            //TODO: !? Implement
        }
    }
}