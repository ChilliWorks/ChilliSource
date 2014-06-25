//
//  NSNotificationAdapter.mm
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

#import <CSBackend/Platform/iOS/Core/Notification/NSNotificationAdapter.h>

#import <GameKit/GameKit.h>
#import <MediaPlayer/MediaPlayer.h>

NSNotificationAdapter* gpSharedInstance = nil;

@implementation NSNotificationAdapter

//-----------------------------------------------
/// Shared Instance
///
/// @return Singleton instance 
//-----------------------------------------------
+(NSNotificationAdapter*) sharedInstance
{
	if(!gpSharedInstance)
	{
		gpSharedInstance = [[NSNotificationAdapter alloc] init];
	}
	
	return gpSharedInstance;
}

//-----------------------------------------------
/// Begin Listening For MP Load State Changed
//-----------------------------------------------
-(void) BeginListeningForMPLoadStateChanged
{
    //Register that the load state changed (movie is ready)
    [[NSNotificationCenter defaultCenter] addObserver:gpSharedInstance 
                                             selector:@selector(OnMoviePlayerLoadStateChanged) 
                                                 name:MPMoviePlayerLoadStateDidChangeNotification 
                                               object:nil];
}
//-----------------------------------------------
/// Stop Listening For MP Load State Changed
//-----------------------------------------------
-(void) StopListeningForMPLoadStateChanged
{
    [[NSNotificationCenter defaultCenter] removeObserver:gpSharedInstance 
                                                    name:MPMoviePlayerLoadStateDidChangeNotification 
                                                  object:nil];
}

//-----------------------------------------------
/// Begin Listening For MP Playback Did Finish
//-----------------------------------------------
-(void) BeginListeningForMPPlaybackDidFinish
{
    //Register to receive a notification when the movie has finished playing. 
    [[NSNotificationCenter defaultCenter] addObserver:gpSharedInstance 
                                             selector:@selector(OnMoviePlayerPlaybackDidFinish) 
                                                 name:MPMoviePlayerPlaybackDidFinishNotification 
                                               object:nil];
}
//-----------------------------------------------
/// Stop Listening For MP Playback Did Finish
//-----------------------------------------------
-(void) StopListeningForMPPlaybackDidFinish
{
    [[NSNotificationCenter defaultCenter] removeObserver:gpSharedInstance 
                                                    name:MPMoviePlayerPlaybackDidFinishNotification 
                                                  object:nil];
}

//-----------------------------------------------
/// Get Movie Player Load State Change Event
///
/// @return Event Object
//-----------------------------------------------
-(CSCore::IConnectableEvent<NotificationEventDelegate>&) GetMPLoadStateChangeEvent
{
    return mMPLoadStateChangeEvent;
}
//-----------------------------------------------
/// Get Movie Player Playback Did Finish Event
///
/// @return Event Object
//-----------------------------------------------
-(CSCore::IConnectableEvent<NotificationEventDelegate>&) GetMPPlaybackDidFinishEvent
{
    return mMPPlaybackDidFinishEvent;
}
//-----------------------------------------------
/// On Movie Player Load State Changed
//-----------------------------------------------
-(void) OnMoviePlayerLoadStateChanged
{
    mMPLoadStateChangeEvent.NotifyConnections();
}
//-----------------------------------------------
/// On Movie Player Playback Did Finish
//-----------------------------------------------
-(void) OnMoviePlayerPlaybackDidFinish
{
    mMPPlaybackDidFinishEvent.NotifyConnections();
}

@end

#endif