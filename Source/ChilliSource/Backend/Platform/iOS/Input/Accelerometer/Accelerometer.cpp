/** 
 * File: Accelerometer.cpp
 * Date: 15/11/2010
 * Description:
 */

/** 
 *
 * Author: Stuart McGaw 
 * Version: 1.0 - MoFlow
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Backend/Platform/iOS/Input/Accelerometer/Accelerometer.h>
#import <CoreMotion/CoreMotion.h>

namespace ChilliSource
{
	namespace iOS
	{
        //----------------------------------------------------
        /// Constructor
        //----------------------------------------------------
        CAccelerometer::CAccelerometer()
        : mbIsUpdating(false)
        {
            m_motionManager = [[CMMotionManager alloc] init];
            m_motionManager.accelerometerUpdateInterval = 0.033;
        }
        //-------------------------------------------------------
        /// Supported By Device
        //-------------------------------------------------------
        bool CAccelerometer::SupportedByDevice()
        {
            if(NSClassFromString(@"CMMotionManager") == nil)
                return false;
            
            CMMotionManager* manager = [[CMMotionManager alloc] init];
            bool supported = manager.accelerometerAvailable;
            [manager release];
            
            return supported;
        }
        //----------------------------------------------------
        /// Is A
        //----------------------------------------------------
        bool CAccelerometer::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == IAccelerometer::InterfaceID || inInterfaceID == IUpdateable::InterfaceID;
        }
        //----------------------------------------------------
        /// Is Updating
        //----------------------------------------------------
        bool CAccelerometer::IsUpdating() const
        {
            return mbIsUpdating;
        }
        //----------------------------------------------------
        /// Start Updating
        //----------------------------------------------------
        void CAccelerometer::StartUpdating()
        {
            mbIsUpdating = true;
            [m_motionManager startAccelerometerUpdates];
        }
        //----------------------------------------------------
        /// Update
        //----------------------------------------------------
        void CAccelerometer::Update(f32 infDT)
        {
            if(mbIsUpdating)
            {
                mAccelerationUpdatedEvent.Invoke(GetAcceleration());
            }
        }
        //----------------------------------------------------
        /// Get Acceleration
        //----------------------------------------------------
        const Core::CVector3& CAccelerometer::GetAcceleration() const
        {
            mvAcceleration = Core::CVector3(m_motionManager.accelerometerData.acceleration.x, m_motionManager.accelerometerData.acceleration.y, m_motionManager.accelerometerData.acceleration.z);
            return mvAcceleration;
        }
        //----------------------------------------------------
        /// Stop Updating
        //----------------------------------------------------
        void CAccelerometer::StopUpdating()
        {
            mbIsUpdating = false;
            [m_motionManager stopAccelerometerUpdates];
        }
        //----------------------------------------------------
        /// Destructor
        //----------------------------------------------------
        CAccelerometer::~CAccelerometer()
        {
            [m_motionManager release];
        }
	}
}
