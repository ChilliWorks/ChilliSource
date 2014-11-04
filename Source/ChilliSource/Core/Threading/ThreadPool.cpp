//
//  ThreadPool.cpp
//  Chilli Source
//  Created by Scott Downie on 17/02/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/Core/Threading/ThreadPool.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------
        //----------------------------------------------
		ThreadPool::ThreadPool(u32 in_numThreads)
			: m_isFinished(false)
        {
            //create the threads
            for (u32 i=0; i<in_numThreads; ++i)
            {  
                m_threadGroup.push_back(std::thread(Core::MakeDelegate(this, &ThreadPool::DoTaskOrWait)));
            }
        }
        //----------------------------------------------
        //----------------------------------------------
        void ThreadPool::Schedule(const GenericTaskType& in_task)
        {
            m_tasks.push(std::move(in_task));
        }
        //----------------------------------------------
        //----------------------------------------------
        u32 ThreadPool::GetNumQueuedTasks() const
        {
            return m_tasks.size();
        }
        //----------------------------------------------
        //----------------------------------------------
        void ThreadPool::ClearQueuedTasks()
        {
			m_tasks.clear();
        }
        //----------------------------------------------
        //----------------------------------------------
        void ThreadPool::DoTaskOrWait()
        {
			while (m_isFinished == false)
			{
				GenericTaskType task;
				if (m_tasks.pop_or_wait(task) == true)
				{
					task();
				}
            }
        }
        //----------------------------------------------
        //----------------------------------------------
        ThreadPool::~ThreadPool()
        {
			m_isFinished = true;
			m_tasks.abort();
            
            //join all threads.
            for (u32 i=0; i<m_threadGroup.size(); ++i)
            {
                m_threadGroup[i].join();
            }
        }
    }
}