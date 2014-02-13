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

#include <ChilliSource/Platform/iOS/Input/Accelerometer.h>
#import <UIKit/UIAccelerometer.h>

moFloAccelerometerDelegate* gpDelegateInstance = nil;
moFlo::iOSPlatform::CAccelerometer* gpAccelerometerInstance = NULL;

namespace moFlo
{
	namespace iOSPlatform
	{
        const f32 kfUpdateFrequency = 30.0f;
        
        //----------------------------------------------------
        /// Constructor
        //----------------------------------------------------
        CAccelerometer::CAccelerometer()
            : mbIsUpdating(false)
        {
            InitAccelerometer();
			gpAccelerometerInstance = this;
        }
        //----------------------------------------------------
        /// Is A
        //----------------------------------------------------
        bool CAccelerometer::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == IAccelerometer::InterfaceID;
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
            if (false == mbIsUpdating)
            {
                mbIsUpdating = true;
                SetAccelerometerUpdateFrequency(kfUpdateFrequency);
            }
        }
        //----------------------------------------------------
        /// Get Acceleration
        //----------------------------------------------------
        const Core::CVector3& CAccelerometer::GetAcceleration() const
        {
            return mvAcceleration;
        }
        //----------------------------------------------------
        /// Stop Updating
        //----------------------------------------------------
        void CAccelerometer::StopUpdating()
        {
            if (true == mbIsUpdating)
            {
                SetAccelerometerUpdateFrequency(0.0f);
                mbIsUpdating = false;
            }
        }
        //----------------------------------------------------
        /// Stop Updating
        //----------------------------------------------------
        void CAccelerometer::SetAcceleration(const Core::CVector3& invDirection)
        {
            mvAcceleration = invDirection;
            mAccelerationUpdatedEvent.Invoke(mvAcceleration);
        }
        //----------------------------------------------------
        /// Destructor
        //----------------------------------------------------
        CAccelerometer::~CAccelerometer()
        {
        }
	}
}

extern "C"
{
	void InitAccelerometer()
	{
		if (gpDelegateInstance == NULL || gpDelegateInstance == nil)
        {
			gpDelegateInstance = [[moFloAccelerometerDelegate alloc] init];
		}
	}
	void SetAccelerometerUpdateFrequency(f32 infFrequency)
	{
		[gpDelegateInstance setUpdateFrequency:infFrequency];
	}
}

@implementation moFloAccelerometerDelegate
- (id) init
{
	if ((self = [super init]))
    {
		[[UIAccelerometer sharedAccelerometer] setDelegate:self];
	}
	return self;
}
- (void)setUpdateFrequency:(f32) frequency
{
	if (frequency <= 0.0f)
    {
		[[UIAccelerometer sharedAccelerometer] setUpdateInterval: 0.0f];
	}
    else
    {
		[[UIAccelerometer sharedAccelerometer] setUpdateInterval: 1.0f / frequency];
	}
	
}
- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
	gpAccelerometerInstance->SetAcceleration(moFlo::Core::CVector3((f32)acceleration.x,(f32)acceleration.y,(f32)acceleration.z));
}

@end
