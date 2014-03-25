//
//  Accelerometer.mm
//  Chilli Source
//
//  Created by S Downie on 15/11/2010
//  Copyright ©2010 Tag Games Limited. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Input/Accelerometer/Accelerometer.h>

#import <CoreMotion/CoreMotion.h>

namespace ChilliSource
{
	namespace iOS
	{
        namespace
        {
            //----------------------------------------------------
            /// @author I Copland
            ///
            /// @return Whether or not the accelerometer is supported
            /// on this device.
            //----------------------------------------------------
            bool SupportedByDevice()
            {
                if(NSClassFromString(@"CMMotionManager") == nil)
                    return false;
                
                CMMotionManager* manager = [[CMMotionManager alloc] init];
                bool supported = manager.accelerometerAvailable;
                [manager release];
                
                return supported;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(Accelerometer);
        //----------------------------------------------------
        //----------------------------------------------------
        Accelerometer::Accelerometer()
            : m_isUpdating(false), m_motionManager(nil)
        {
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool Accelerometer::IsA(Core::InterfaceIDType in_interfaceID) const
        {
            return (in_interfaceID == Input::Accelerometer::InterfaceID || in_interfaceID == Accelerometer::InterfaceID);
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
            if (m_motionManager != nil)
            {
                m_isUpdating = true;
                [m_motionManager startAccelerometerUpdates];
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::Vector3 Accelerometer::GetAcceleration() const
        {
            if (m_motionManager != nil)
            {
                return Core::Vector3(m_motionManager.accelerometerData.acceleration.x, m_motionManager.accelerometerData.acceleration.y, m_motionManager.accelerometerData.acceleration.z);
            }
            return Core::Vector3::ZERO;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<Accelerometer::AccelerationUpdatedDelegate>& Accelerometer::GetAccelerationUpdatedEvent()
        {
            return m_accelerationUpdatedEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Accelerometer::StopUpdating()
        {
            m_isUpdating = false;
            [m_motionManager stopAccelerometerUpdates];
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Accelerometer::OnInit()
        {
            if (SupportedByDevice() == true)
            {
                m_motionManager = [[CMMotionManager alloc] init];
                m_motionManager.accelerometerUpdateInterval = 0.033;
            }
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
            if (m_motionManager != nil)
            {
                [m_motionManager release];
                m_motionManager = nil;
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Accelerometer::~Accelerometer()
        {
            OnDestroy();
        }
	}
}
