//
//  Gyroscope.mm
//  ChilliSource
//  Created by Jordan Brown on 19/01/2017
//
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

#ifdef CS_TARGETPLATFORM_IOS

#import <CSBackend/Platform/iOS/Input/Gyroscope/Gyroscope.h>

#import <ChilliSource/Core/Base/Application.h>
#import <ChilliSource/Core/Base/AppConfig.h>
#import <ChilliSource/Core/Math/MathUtils.h>
#import <ChilliSource/Core/Threading/TaskScheduler.h>

#import <CoreMotion/CoreMotion.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

namespace CSBackend
{
    namespace iOS
    {
        CS_DEFINE_NAMEDTYPE(Gyroscope);
        //----------------------------------------------------
        //----------------------------------------------------
        bool Gyroscope::IsSupportedByDevice()
        {
            if(NSClassFromString(@"CMMotionManager") == nil)
                return false;
            
            CMMotionManager* manager = [[CMMotionManager alloc] init];
            bool supported = manager.deviceMotionAvailable;
            [manager release];
            
            return supported;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Gyroscope::Gyroscope()
        : m_isUpdating(false), m_motionManager(nil)
        {
            CS_ASSERT(IsSupportedByDevice(), "Cannot create gyroscope on device that does not support it!");
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool Gyroscope::IsA(ChilliSource::InterfaceIDType in_interfaceID) const
        {
            return (in_interfaceID == ChilliSource::Gyroscope::InterfaceID || in_interfaceID == Gyroscope::InterfaceID);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool Gyroscope::IsUpdating() const
        {
            return m_isUpdating;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Gyroscope::StartUpdating()
        {
            CS_RELEASE_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Tried to start gyroscope updating from background thread.");
            if (m_isUpdating == false)
            {
                m_isUpdating = true;
                [m_motionManager startDeviceMotionUpdatesUsingReferenceFrame:CMAttitudeReferenceFrameXArbitraryZVertical];
            }
        }
        
        //----------------------------------------------------
        void Gyroscope::OnUpdate(f32 deltaSecs)
        {
            if(m_isUpdating == true)
            {
                OnDeviceMotionUpdated(m_motionManager.deviceMotion);
            }
        }
        
        //----------------------------------------------------
        //----------------------------------------------------
        ChilliSource::Quaternion Gyroscope::GetOrientation() const
        {
            CS_RELEASE_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Tried to get current gyro orientation from background thread.");
            return m_orientation;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        ChilliSource::IConnectableEvent<Gyroscope::OrientationUpdatedDelegate>& Gyroscope::GetOrientationUpdatedEvent()
        {
            return m_orientationUpdatedEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Gyroscope::StopUpdating()
        {
            CS_RELEASE_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Tried to stop gyroscope updating from background thread.");
            if (m_isUpdating == true)
            {
                m_isUpdating = false;
                ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
                {
                    [m_motionManager stopDeviceMotionUpdates];
                });
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Gyroscope::OnInit()
        {
            m_motionManager = [[CMMotionManager alloc] init];
            m_motionManager.gyroUpdateInterval = 1.0f / ChilliSource::Application::Get()->GetAppConfig()->GetPreferredFPS();
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Gyroscope::OnDeviceMotionUpdated(CMDeviceMotion* deviceMotionData) noexcept
        {
            ChilliSource::Quaternion newOrientation;
            newOrientation.x = deviceMotionData.attitude.quaternion.x;
            newOrientation.y = deviceMotionData.attitude.quaternion.y;
            newOrientation.z = deviceMotionData.attitude.quaternion.z;
            newOrientation.w = deviceMotionData.attitude.quaternion.w;
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
            {
                m_orientation = newOrientation;
                m_orientationUpdatedEvent.NotifyConnections(m_orientation);
            });
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Gyroscope::OnDestroy()
        {
            [m_motionManager release];
            m_motionManager = nil;
        }
    }
}

#endif
