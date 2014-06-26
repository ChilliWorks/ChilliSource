//
//  NSNotificationAdapter.h
//  Chilli Source
//  Created by Scott Downie on 01/02/2011.
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

#ifdef CS_TARGETPLATFORM_IOS

#ifndef _NS_NOTIFICATION_ADAPTER_H_
#define _NS_NOTIFICATION_ADAPTER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>

#import <Foundation/Foundation.h>

#include <functional>

typedef std::function<void()> NotificationEventDelegate;

@interface NSNotificationAdapter : NSObject 
{    
    //---Media Player
    CSCore::Event<NotificationEventDelegate> mMPLoadStateChangeEvent;
    CSCore::Event<NotificationEventDelegate> mMPPlaybackDidFinishEvent;
}

//-----------------------------------------------
/// Shared Instance
///
/// @return Singleton instance 
//-----------------------------------------------
+(NSNotificationAdapter*) sharedInstance;

//-----------------------------------------------
/// Begin Listening For MP Load State Changed
//-----------------------------------------------
-(void) BeginListeningForMPLoadStateChanged;
//-----------------------------------------------
/// Stop Listening For MP Load State Changed
//-----------------------------------------------
-(void) StopListeningForMPLoadStateChanged;

//-----------------------------------------------
/// Begin Listening For MP Playback Did Finish
//-----------------------------------------------
-(void) BeginListeningForMPPlaybackDidFinish;
//-----------------------------------------------
/// Stop Listening For MP Playback Did Finish
//-----------------------------------------------
-(void) StopListeningForMPPlaybackDidFinish;

//-----------------------------------------------
/// Get Movie Player Load State Change Event
///
/// @return Event Object
//-----------------------------------------------
-(CSCore::IConnectableEvent<NotificationEventDelegate>&) GetMPLoadStateChangeEvent;
//-----------------------------------------------
/// Get Movie Player Playback Did Finish Event
///
/// @return Event Object
//-----------------------------------------------
-(CSCore::IConnectableEvent<NotificationEventDelegate>&) GetMPPlaybackDidFinishEvent;

//---Notification callbacks
//-----------------------------------------------
/// On Movie Player Load State Changed
//-----------------------------------------------
-(void) OnMoviePlayerLoadStateChanged;
//-----------------------------------------------
/// On Movie Player Playback Did Finish
//-----------------------------------------------
-(void) OnMoviePlayerPlaybackDidFinish;

@end

#endif

#endif