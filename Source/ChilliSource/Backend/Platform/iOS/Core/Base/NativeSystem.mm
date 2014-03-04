//
//  iOS_3.X.mm
//  moFlo
//
//  Created by Scott Downie on 24/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#import <ChilliSource/Core/Base/PlatformSystem.h>
#import <ChilliSource/Backend/Platform/iOS/Core/Base/NativeSystem.h>

#import <ChilliSource/Core/Base/Application.h>

#import <mach/mach.h>
#import <mach/mach_time.h>
#import <QuartzCore/QuartzCore.h>

CNativeSystem* gpiOSPlatform = nil;
static mach_timebase_info_data_t gMachtimeBase;

@implementation CNativeSystem

//----------------------------------------------
/// Init
///
/// Initialises the run loop and other platform
/// specific API's
//----------------------------------------------
-(void) Init
{
	[super init];
	
	mach_timebase_info(&gMachtimeBase);
	
	//Get the current frame time
	mPreviousFrameTime = (mach_absolute_time() * gMachtimeBase.numer) / gMachtimeBase.denom;
	mAppStartTime = mPreviousFrameTime;
    
    ChilliSource::Core::Application::Get()->OnFrameBegin(0.0f, (f32)mPreviousFrameTime - (f32)mAppStartTime);
	
	//Begin the update loop
    [self SetMaxFPS:30];
}

//-----------------------------------------------
/// Invalidate Updater
///
/// Invalidate the display link
//-----------------------------------------------
-(void) InvalidateUpdater
{
	if(mDisplayLink)
	{
		[mDisplayLink invalidate];
		mDisplayLink = nil;
	}
}
//-----------------------------------------------
/// Set Max FPS
///
/// @param Max FPS
//-----------------------------------------------
-(void) SetMaxFPS:(u32) inudwFPS
{
    [self InvalidateUpdater];
    
    mDisplayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(Update:)];
	[mDisplayLink setFrameInterval:60.0f/(f32)inudwFPS];
	[mDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}
//-----------------------------------------------
/// Set Updater Active
///
/// Invalidate or begin the display link
//-----------------------------------------------
-(void) SetUpdaterActive: (BOOL) inbIsActive
{
	if(inbIsActive)
	{
        //We do not want a delta time between the updater being disabled and re-enabled so set the previous
        //frame time to now.
        TimeIntervalSecs CurrentTime = (mach_absolute_time() * gMachtimeBase.numer) / gMachtimeBase.denom;
        mPreviousFrameTime = CurrentTime;
        
		//Begin the update loop
		mDisplayLink.paused = false;
	}
	else
	{
		//Stop the update loop
		mDisplayLink.paused = true;
	}

}
//----------------------------------------------
/// Update
///
/// This function is fired by the display link
/// run loop; based on the update frequency.
///
/// This function will then dispatch a frame
/// began message to the message dispatch
/// subscriber
//----------------------------------------------
-(void) Update: (id) Sender
{
	//Calculate the time between frames and attach
	//it to the frame began message.
	TimeIntervalSecs CurrentTime = (mach_absolute_time() * gMachtimeBase.numer) / gMachtimeBase.denom;
	CFTimeInterval fDeltaTime = ((f32)(CurrentTime - mPreviousFrameTime))/ 1000000000.0f;
	
	mPreviousFrameTime = CurrentTime;
    
    ChilliSource::Core::Application::Get()->OnFrameBegin((f32)fDeltaTime, (f32)mPreviousFrameTime - (f32)mAppStartTime);
}
//------------------------------------------------
/// Get System Time
///
/// returns the system time in nano seconds.
//------------------------------------------------
-(u64) GetSystemTime
{
	return (mach_absolute_time() * gMachtimeBase.numer) / gMachtimeBase.denom;
}
//--------------------------------------------------------------------------------------------------
/// Show System Confirm Dialog
///
/// Display a system confirmation dialog with the given ID and delegate
///
/// @param ID
/// @param Title text
/// @param Message text
/// @param Confirm text
/// @param Cancel text
//--------------------------------------------------------------------------------------------------
-(void) ShowSystemConfirmDialogWithID:(u32)inudwID title:(NSString*)instrTitle message:(NSString*)instrMessage confirm:(NSString*)instrConfirm andCancel:(NSString*)instrCancel
{
    UIAlertView* pConfirm = [[UIAlertView alloc] initWithTitle:instrTitle message:instrMessage 
                                                    delegate:self cancelButtonTitle:instrCancel otherButtonTitles:instrConfirm, nil];
    
    pConfirm.tag = inudwID;
    [pConfirm show];
    [pConfirm release];
}
//--------------------------------------------------------------------------------------------------
/// Show System Dialog
///
/// Display a system dialog with the given ID and delegate
///
/// @param ID
/// @param Title text
/// @param Message text
/// @param Confirm text
//--------------------------------------------------------------------------------------------------
-(void) ShowSystemDialogWithID:(u32)inudwID title:(NSString*)instrTitle message:(NSString*)instrMessage confirm:(NSString*)instrConfirm
{
    UIAlertView* pConfirm = [[UIAlertView alloc] initWithTitle:instrTitle message:instrMessage
                                                      delegate:self cancelButtonTitle:instrConfirm otherButtonTitles:nil];
    
    pConfirm.tag = inudwID;
    [pConfirm show];
    [pConfirm release];
}
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex 
{
	// NO = 0, YES = 1
	if(buttonIndex == 0)
    {
        ChilliSource::Core::Application::Get()->OnSystemConfirmDialogResult(alertView.tag, ChilliSource::Core::SystemConfirmDialog::Result::k_cancel);
    }
    else 
    {
        ChilliSource::Core::Application::Get()->OnSystemConfirmDialogResult(alertView.tag, ChilliSource::Core::SystemConfirmDialog::Result::k_confirm);
    }
}
//----------------------------------------------
/// Dealloc
///
/// Destruct the objecy
//----------------------------------------------
-(void) dealloc
{
	[self InvalidateUpdater];
	
	[super dealloc];
}
@end

//==============================================================
/// Allows Obj-C functions to be called from the platform
/// interface.
//==============================================================
extern "C" 
{
	void iOSInit()
	{
		if(!gpiOSPlatform)
		{
			gpiOSPlatform = [CNativeSystem alloc];
		}
		
		[gpiOSPlatform Init];
	}
    void iOSSetMaxFPS(u32 inudwFPS)
    {
        [gpiOSPlatform SetMaxFPS: inudwFPS];
    }
	void iOSSetUpdaterActive(bool inbIsActive)
	{
		[gpiOSPlatform SetUpdaterActive: inbIsActive];
	}
    void iOSInvalidateUpdater()
	{
		[gpiOSPlatform InvalidateUpdater];
	}
	u64 GetSystemTimeInNanoSeconds()
	{
		return [gpiOSPlatform GetSystemTime];
	}
    void iOSShowSystemConfirmDialog(u32 inudwID, NSString* instrTitle, NSString* instrMessage, NSString* instrConfirm, NSString* instrCancel)
    {
        [gpiOSPlatform ShowSystemConfirmDialogWithID:inudwID title:instrTitle message:instrMessage confirm:instrConfirm andCancel:instrCancel];
    }
    void iOSShowSystemDialog(u32 inudwID, NSString* instrTitle, NSString* instrMessage, NSString* instrConfirm)
    {
        [gpiOSPlatform ShowSystemDialogWithID:inudwID title:instrTitle message:instrMessage confirm:instrConfirm];
    }
}
