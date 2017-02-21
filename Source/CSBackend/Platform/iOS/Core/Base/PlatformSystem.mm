//
//  PlatformSystem.mm
//  ChilliSource
//  Created by Scott Downie on 24/11/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_IOS

#import <CSBackend/Platform/iOS/Core/Base/PlatformSystem.h>

#import <CSBackend/Platform/iOS/Core/Base/CSAppDelegate.h>
#import <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>
#import <ChilliSource/Core/Base/Application.h>
#import <ChilliSource/Core/Image/PVRImageProvider.h>
#import <ChilliSource/Core/Threading/TaskScheduler.h>

namespace CSBackend 
{
	namespace iOS
	{
        CS_DEFINE_NAMEDTYPE(PlatformSystem);
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool PlatformSystem::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
        {
            return (ChilliSource::PlatformSystem::InterfaceID == in_interfaceId || PlatformSystem::InterfaceID == in_interfaceId);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
		void PlatformSystem::CreateDefaultSystems(ChilliSource::Application* in_application)
		{
            in_application->CreateSystem<ChilliSource::PVRImageProvider>();
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        void PlatformSystem::SetPreferredFPS(u32 in_fps)
        {
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& in_taskContext)
            {
                [[CSAppDelegate sharedInstance] setPreferredFPS:in_fps];
            });
        }
	}
}

#endif
