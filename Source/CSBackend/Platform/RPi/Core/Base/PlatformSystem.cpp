//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#include <CSBackend/Platform/RPi/Core/Base/PlatformSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <CSBackend/Platform/RPi/Core/Base/DispmanWindow.h>
#include <ChilliSource/UI/Base/CursorSystem.h>

namespace CSBackend
{
	namespace RPi
	{
		CS_DEFINE_NAMEDTYPE(PlatformSystem);

		//---------------------------------------------------------------------------------
		bool PlatformSystem::IsA(ChilliSource::InterfaceIDType interfaceId) const
		{
			return (ChilliSource::PlatformSystem::InterfaceID == interfaceId || PlatformSystem::InterfaceID == interfaceId);
		}

		//---------------------------------------------------------------------------------
		void PlatformSystem::CreateDefaultSystems(ChilliSource::Application* application)
		{
			application->CreateSystem<ChilliSource::CursorSystem>();
		}

		//---------------------------------------------------------------------------------
		void PlatformSystem::SetPreferredFPS(u32 fps)
		{
			ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& in_taskContext)
			{
				DispmanWindow::Get()->SetPreferredFPS(fps);
			});
		}

		//---------------------------------------------------------------------------------
		void PlatformSystem::Quit()
		{
			ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& in_taskContext)
			{
				DispmanWindow::Get()->ScheduleQuit();
			});
		}
	}
}

#endif
