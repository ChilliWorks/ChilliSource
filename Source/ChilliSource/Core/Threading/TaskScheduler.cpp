//
//  TaskScheduler.cpp
//  Chilli Source
//  Created by Scott Downie on 09/08/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

namespace ChilliSource
{
    namespace Core
    {
		CS_DEFINE_NAMEDTYPE(TaskScheduler);

		//-------------------------------------------------
		//-------------------------------------------------
		TaskSchedulerUPtr TaskScheduler::Create()
		{
			return TaskSchedulerUPtr(new TaskScheduler());
		}
		//-------------------------------------------------
		//-------------------------------------------------
		TaskScheduler::TaskScheduler()
		{
        
		}
		//------------------------------------------------
		//------------------------------------------------
		bool TaskScheduler::IsA(InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == TaskScheduler::InterfaceID;
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void TaskScheduler::OnInit()
		{
            Device* device = Core::Application::Get()->GetSystem<Device>();
			m_threadPool = ThreadPoolUPtr(new Core::ThreadPool(device->GetNumberOfCPUCores() * 2));
            
            m_mainThreadId = std::this_thread::get_id();
		}
        //------------------------------------------------
        //------------------------------------------------
        bool TaskScheduler::IsMainThread() const
        {
            return m_mainThreadId == std::this_thread::get_id();
        }
		//------------------------------------------------
		//------------------------------------------------
		void TaskScheduler::ScheduleTask(const GenericTaskType& in_task)
		{
			m_threadPool->Schedule(in_task);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void TaskScheduler::ScheduleMainThreadTask(const GenericTaskType& in_task)
		{
			std::unique_lock<std::recursive_mutex> lock(m_mainThreadQueueMutex);
			m_mainThreadTasks.push_back(in_task);
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void TaskScheduler::ExecuteMainThreadTasks()
        {
            std::unique_lock<std::recursive_mutex> lock(m_mainThreadQueueMutex);
            
            for (u32 i = 0; i < m_mainThreadTasks.size(); ++i)
            {
				m_mainThreadTasks[i]();
            }
            
			m_mainThreadTasks.clear();
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void TaskScheduler::Destroy()
		{
			m_threadPool.reset();
			m_mainThreadTasks.clear();
		}
    }
}

