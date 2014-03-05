//
//  iOS_3.X.h
//  moFlo
//
//  Created by Scott Downie on 24/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _MOFLO_PLATFORM_IOS_NATIVE_SYSTEM_H_
#define _MOFLO_PLATFORM_IOS_NATIVE_SYSTEM_H_

#include <ChilliSource/ChilliSource.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//==========================================================
/// Description:
///
/// Platform independant code for iOS 3.1 and above.
/// 
/// Creates the run loop that updates the engine
//==========================================================
@interface CNativeSystem : NSObject<UIAlertViewDelegate> 
{
	CADisplayLink* mDisplayLink;		//The CA display link for the updater
		
	TimeIntervalSecs mPreviousFrameTime;
	TimeIntervalSecs mAppStartTime;
}

-(void) Init; 
-(void) Update: (id) Sender;
-(void) SetMaxFPS:(u32) inudwFPS;
-(void) InvalidateUpdater;
-(void) SetUpdaterActive: (BOOL) inbIsActive;
-(void) dealloc;
-(u64) GetSystemTime;
@end

extern "C" 
{
	void iOSInit();
    void iOSSetMaxFPS(u32 inudwFPS);
	void iOSSetUpdaterActive(bool inbIsActive);
    void iOSInvalidateUpdater();
	u64 GetSystemTimeInNanoSeconds();
    void iOSShowSystemConfirmDialog(u32 inudwID, NSString* instrTitle, NSString* instrMessage, NSString* instrConfirm, NSString* instrCancel);
    void iOSShowSystemDialog(u32 inudwID, NSString* instrTitle, NSString* instrMessage, NSString* instrConfirm);
}

#endif
