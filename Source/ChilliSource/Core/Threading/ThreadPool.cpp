// ThreadPool.cpp
// ChilliSource
//
// Created by Scott Downie on 17/02/2014
// Copyright Tag Games 2014. All rights reserved.

#include <ChilliSource/Core/Threading/ThreadPool.h>

#include <ChilliSource/Core/Base/MakeDelegate.h>

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
        void ThreadPool::Schedule(const GenericTaskType& inTask)
        {
            m_tasks.push(inTask);
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
            while(m_tasks.empty() == false)
            {
                m_tasks.pop();
            }
        }
        //----------------------------------------------
        //----------------------------------------------
        void ThreadPool::DoTaskOrWait()
        {
            while (m_isFinished == false)
            {
                GenericTaskType task = m_tasks.front();
                m_tasks.pop();
                task();
            }
        }
        //----------------------------------------------
        //----------------------------------------------
        ThreadPool::~ThreadPool()
        {
            m_isFinished = true;
            
            m_awakeCondition.notify_all();
            
            //join all threads.
            for (u32 i=0; i<m_threadGroup.size(); ++i)
            {
                m_threadGroup[i].join();
            }
        }
    }
}