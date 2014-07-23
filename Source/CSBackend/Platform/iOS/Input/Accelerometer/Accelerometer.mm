//
//  Accelerometer.mm
//  Chilli Source
//  Created by Scott Downie on 15/11/2010
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

#import <CSBackend/Platform/iOS/Input/Accelerometer/Accelerometer.h>

#import <ChilliSource/Core/Math/MathUtils.h>

#import <CoreMotion/CoreMotion.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

namespace CSBackend
{
	namespace iOS
	{
        CS_DEFINE_NAMEDTYPE(Accelerometer);
        //----------------------------------------------------
        //----------------------------------------------------
        bool Accelerometer::IsSupportedByDevice()
        {
            if(NSClassFromString(@"CMMotionManager") == nil)
                return false;
            
            CMMotionManager* manager = [[CMMotionManager alloc] init];
            bool supported = manager.accelerometerAvailable;
            [manager release];
            
            return supported;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Accelerometer::Accelerometer()
            : m_isUpdating(false), m_motionManager(nil)
        {
            CS_ASSERT(IsSupportedByDevice(), "Cannot create accelerometer on device that does not support it!");
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool Accelerometer::IsA(CSCore::InterfaceIDType in_interfaceID) const
        {
            return (in_interfaceID == CSInput::Accelerometer::InterfaceID || in_interfaceID == Accelerometer::InterfaceID);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool Accelerometer::IsUpdating() const
        {
            return m_isUpdating;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Accelerometer::StartUpdating()
        {
            if (m_isUpdating == false)
            {
                m_isUpdating = true;
                [m_motionManager startAccelerometerUpdates];
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        CSCore::Vector3 Accelerometer::GetAcceleration() const
        {
            CSCore::Matrix4 orientationTransform;
            switch ([UIApplication sharedApplication].keyWindow.rootViewController.interfaceOrientation)
            {
                case UIInterfaceOrientationPortrait:
                    orientationTransform = CSCore::Matrix4::k_identity;
                    break;
                case UIInterfaceOrientationLandscapeLeft:
                    orientationTransform = CSCore::Matrix4::CreateRotationZ(CSCore::MathUtils::k_pi * 1.5f);
                    break;
                case UIInterfaceOrientationLandscapeRight:
                    orientationTransform = CSCore::Matrix4::CreateRotationZ(CSCore::MathUtils::k_pi * 0.5f);
                    break;
                case UIInterfaceOrientationPortraitUpsideDown:
                    orientationTransform = CSCore::Matrix4::CreateRotationZ(CSCore::MathUtils::k_pi);
                    break;
                default:
                    CS_LOG_ERROR("Unknown orientation!");
                    orientationTransform = CSCore::Matrix4::k_identity;
                    break;
            }
            
            CMAcceleration acceleration(m_motionManager.accelerometerData.acceleration);
            CSCore::Vector3 deviceSpaceAcceleration(acceleration.x, acceleration.y, acceleration.z);
            deviceSpaceAcceleration.Transform3x4(orientationTransform);
            return deviceSpaceAcceleration;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        CSCore::IConnectableEvent<Accelerometer::AccelerationUpdatedDelegate>& Accelerometer::GetAccelerationUpdatedEvent()
        {
            return m_accelerationUpdatedEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Accelerometer::StopUpdating()
        {
            if (m_isUpdating == true)
            {
                m_isUpdating = false;
                [m_motionManager stopAccelerometerUpdates];
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Accelerometer::OnInit()
        {
            m_motionManager = [[CMMotionManager alloc] init];
            m_motionManager.accelerometerUpdateInterval = 0.033;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Accelerometer::OnUpdate(f32 in_deltaTime)
        {
            if(m_isUpdating)
            {
                m_accelerationUpdatedEvent.NotifyConnections(GetAcceleration());
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Accelerometer::OnDestroy()
        {
            [m_motionManager release];
            m_motionManager = nil;
        }
	}
}

#endif